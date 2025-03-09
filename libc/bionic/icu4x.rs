// Copyright (C) 2025 The Android Open Source Project
// SPDX-License-Identifier: Apache-2.0

#![allow(missing_docs)] // Not particularly useful to document these thin wrappers

//! This is a thin wrapper around ICU4X for use in Bionic

use icu_casemap::CaseMapper;
use icu_collections::codepointtrie::TrieValue;
use icu_properties::props::*;
use icu_properties::{CodePointMapData, CodePointSetData};

#[no_mangle]
pub extern "C" fn __icu4x_bionic_general_category(ch: u32) -> u8 {
    CodePointMapData::<GeneralCategory>::new().get32(ch) as u8
}

#[no_mangle]
pub extern "C" fn __icu4x_bionic_east_asian_width(ch: u32) -> u8 {
    CodePointMapData::<EastAsianWidth>::new().get32(ch).to_u32() as u8
}

#[no_mangle]
pub extern "C" fn __icu4x_bionic_hangul_syllable_type(ch: u32) -> u8 {
    CodePointMapData::<HangulSyllableType>::new().get32(ch).to_u32() as u8
}

#[no_mangle]
pub extern "C" fn __icu4x_bionic_is_alphabetic(ch: u32) -> bool {
    CodePointSetData::new::<Alphabetic>().contains32(ch)
}

#[no_mangle]
pub extern "C" fn __icu4x_bionic_is_default_ignorable_code_point(ch: u32) -> bool {
    CodePointSetData::new::<DefaultIgnorableCodePoint>().contains32(ch)
}

#[no_mangle]
pub extern "C" fn __icu4x_bionic_is_lowercase(ch: u32) -> bool {
    CodePointSetData::new::<Lowercase>().contains32(ch)
}

#[no_mangle]
pub extern "C" fn __icu4x_bionic_is_alnum(ch: u32) -> bool {
    CodePointSetData::new::<Alnum>().contains32(ch)
}

#[no_mangle]
pub extern "C" fn __icu4x_bionic_is_blank(ch: u32) -> bool {
    CodePointSetData::new::<Blank>().contains32(ch)
}

#[no_mangle]
pub extern "C" fn __icu4x_bionic_is_graph(ch: u32) -> bool {
    CodePointSetData::new::<Graph>().contains32(ch)
}

#[no_mangle]
pub extern "C" fn __icu4x_bionic_is_print(ch: u32) -> bool {
    CodePointSetData::new::<Print>().contains32(ch)
}

#[no_mangle]
pub extern "C" fn __icu4x_bionic_is_xdigit(ch: u32) -> bool {
    CodePointSetData::new::<Xdigit>().contains32(ch)
}

#[no_mangle]
pub extern "C" fn __icu4x_bionic_is_white_space(ch: u32) -> bool {
    CodePointSetData::new::<WhiteSpace>().contains32(ch)
}

#[no_mangle]
pub extern "C" fn __icu4x_bionic_is_uppercase(ch: u32) -> bool {
    CodePointSetData::new::<Uppercase>().contains32(ch)
}

/// Convert a code point to uppercase
#[no_mangle]
pub extern "C" fn __icu4x_bionic_to_upper(ch: u32) -> u32 {
    let Ok(ch) = char::try_from(ch) else {
        return ch;
    };
    let cm = CaseMapper::new();

    cm.simple_uppercase(ch) as u32
}

/// Convert a code point to lowercase
#[no_mangle]
pub extern "C" fn __icu4x_bionic_to_lower(ch: u32) -> u32 {
    let Ok(ch) = char::try_from(ch) else {
        return ch;
    };
    let cm = CaseMapper::new();

    cm.simple_lowercase(ch) as u32
}
