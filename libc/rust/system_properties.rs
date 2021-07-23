/*
 * Copyright (C) 2021 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//! This crate provides the PropertyWatcher type, which watches for changes
//! in Android system properties.

use anyhow::{anyhow, Context, Result as AnyhowResult};
use system_properties_bindgen::prop_info as PropInfo;
use std::os::raw::c_char;
use std::ptr::null;
use std::{
    ffi::{c_void, CStr, CString},
    str::Utf8Error,
};
use thiserror::Error;

/// Errors this crate can generate
#[derive(Error, Debug)]
pub enum PropertyWatcherError {
    /// We can't watch for a property whose name contains a NUL character.
    #[error("Cannot convert name to C string")]
    BadNameError(#[from] std::ffi::NulError),
    /// We can only watch for properties that exist when the watcher is created.
    #[error("System property is absent")]
    SystemPropertyAbsent,
    /// __system_property_wait timed out despite being given no timeout.
    #[error("Wait failed")]
    WaitFailed,
    /// read callback was not called
    #[error("__system_property_read_callback did not call callback")]
    ReadCallbackNotCalled,
    /// read callback gave us a NULL pointer
    #[error("__system_property_read_callback gave us a NULL pointer instead of a string")]
    MissingCString,
    /// read callback gave us a bad C string
    #[error("__system_property_read_callback gave us a non-UTF8 C string")]
    BadCString(#[from] Utf8Error),
    /// read callback returned an error
    #[error("Callback failed")]
    CallbackError(#[from] anyhow::Error),
    /// Failure in setting the system property
    #[error("__system_property_set failed.")]
    SetPropertyFailed,
}

/// Result type specific for this crate.
pub type Result<T> = std::result::Result<T, PropertyWatcherError>;

/// PropertyWatcher takes the name of an Android system property such
/// as `keystore.boot_level`; it can report the current value of this
/// property, or wait for it to change.
pub struct PropertyWatcher {
    prop_name: CString,
    prop_info: *const PropInfo,
    serial: system_properties_bindgen::__uint32_t,
}

impl PropertyWatcher {
    /// Create a PropertyWatcher for the named system property.
    pub fn new(name: &str) -> Result<Self> {
        Ok(Self { prop_name: CString::new(name)?, prop_info: null(), serial: 0 })
    }

    // Lazy-initializing accessor for self.prop_info.
    fn get_prop_info(&mut self) -> Option<*const PropInfo> {
        if self.prop_info.is_null() {
            // Unsafe required for FFI call. Input and output are both const.
            // The returned pointer is valid for the lifetime of the program.
            self.prop_info = unsafe {
                system_properties_bindgen::__system_property_find(self.prop_name.as_ptr())
            };
        }
        if self.prop_info.is_null() {
            None
        } else {
            Some(self.prop_info)
        }
    }

    fn read_raw(prop_info: *const PropInfo, mut f: impl FnOnce(Option<&CStr>, Option<&CStr>)) {
        // Unsafe function converts values passed to us by
        // __system_property_read_callback to Rust form
        // and pass them to inner callback.
        unsafe extern "C" fn callback(
            res_p: *mut c_void,
            name: *const c_char,
            value: *const c_char,
            _: system_properties_bindgen::__uint32_t,
        ) {
            let name = if name.is_null() { None } else { Some(CStr::from_ptr(name)) };
            let value = if value.is_null() { None } else { Some(CStr::from_ptr(value)) };
            let f = &mut *res_p.cast::<&mut dyn FnMut(Option<&CStr>, Option<&CStr>)>();
            f(name, value);
        }

        let mut f: &mut dyn FnOnce(Option<&CStr>, Option<&CStr>) = &mut f;

        // Unsafe block for FFI call. We convert the FnOnce
        // to a void pointer, and unwrap it in our callback.
        unsafe {
            system_properties_bindgen::__system_property_read_callback(
                prop_info,
                Some(callback),
                &mut f as *mut _ as *mut c_void,
            )
        }
    }

    /// Call the passed function, passing it the name and current value
    /// of this system property. See documentation for
    /// `__system_property_read_callback` for details.
    /// Returns an error if the property is empty or doesn't exist.
    pub fn read<T, F>(&mut self, f: F) -> Result<T>
    where
        F: FnOnce(&str, &str) -> anyhow::Result<T>,
    {
        let prop_info = self.get_prop_info().ok_or(PropertyWatcherError::SystemPropertyAbsent)?;
        let mut result = Err(PropertyWatcherError::ReadCallbackNotCalled);
        Self::read_raw(prop_info, |name, value| {
            // use a wrapping closure as an erzatz try block.
            result = (|| {
                let name = name.ok_or(PropertyWatcherError::MissingCString)?.to_str()?;
                let value = value.ok_or(PropertyWatcherError::MissingCString)?.to_str()?;
                f(name, value).map_err(PropertyWatcherError::CallbackError)
            })()
        });
        result
    }

    // Waits for the property that self is watching to be created. Returns immediately if the
    // property already exists.
    fn wait_for_property_creation(&mut self) -> Result<()> {
        let mut global_serial = 0;
        loop {
            match self.get_prop_info() {
                Some(_) => return Ok(()),
                None => {
                    // Unsafe call for FFI. The function modifies only global_serial, and has
                    // no side-effects.
                    if !unsafe {
                        // Wait for a global serial number change, then try again. On success,
                        // the function will update global_serial with the last version seen.
                        system_properties_bindgen::__system_property_wait(
                            null(),
                            global_serial,
                            &mut global_serial,
                            null(),
                        )
                    } {
                        return Err(PropertyWatcherError::WaitFailed);
                    }
                }
            }
        }
    }

    /// Wait for the system property to change. This
    /// records the serial number of the last change, so
    /// race conditions are avoided.
    pub fn wait(&mut self) -> Result<()> {
        // If the property is null, then wait for it to be created. Subsequent waits will
        // skip this step and wait for our specific property to change.
        if self.prop_info.is_null() {
            return self.wait_for_property_creation();
        }

        let mut new_serial = self.serial;
        // Unsafe block to call __system_property_wait.
        // All arguments are private to PropertyWatcher so we
        // can be confident they are valid.
        if !unsafe {
            system_properties_bindgen::__system_property_wait(
                self.prop_info,
                self.serial,
                &mut new_serial,
                null(),
            )
        } {
            return Err(PropertyWatcherError::WaitFailed);
        }
        self.serial = new_serial;
        Ok(())
    }
}

/// Writes a system property.
pub fn write(name: &str, value: &str) -> AnyhowResult<()> {
    if
    // Unsafe required for FFI call. Input and output are both const and valid strings.
    unsafe {
        // If successful, __system_property_set returns 0, otherwise, returns -1.
        system_properties_bindgen::__system_property_set(
            CString::new(name)
                .context("In keystore2::system_property::write: Construction CString from name.")?
                .as_ptr(),
            CString::new(value)
                .context("In keystore2::system_property::write: Constructing CString from value.")?
                .as_ptr(),
        )
    } == 0
    {
        Ok(())
    } else {
        Err(anyhow!(PropertyWatcherError::SetPropertyFailed))
    }
}
