/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// TODO: This file should go away once unwinder migration to libc++.so is complete.

extern char _Unwind_Backtrace __attribute((visibility("protected")));
extern char __gnu_Unwind_Find_exidx __attribute((visibility("protected")));
extern char __gnu_Unwind_Restore_VFP_D __attribute((visibility("protected")));
extern char __gnu_Unwind_Restore_VFP __attribute((visibility("protected")));
extern char __gnu_Unwind_Restore_VFP_D_16_to_31 __attribute((visibility("protected")));
extern char __gnu_Unwind_Restore_WMMXD __attribute((visibility("protected")));
extern char __gnu_Unwind_Restore_WMMXC __attribute((visibility("protected")));
extern char _Unwind_GetCFA __attribute((visibility("protected")));
extern char __gnu_Unwind_RaiseException __attribute((visibility("protected")));
extern char __gnu_Unwind_ForcedUnwind __attribute((visibility("protected")));
extern char __gnu_Unwind_Resume __attribute((visibility("protected")));
extern char __gnu_Unwind_Resume_or_Rethrow __attribute((visibility("protected")));
extern char _Unwind_Complete __attribute((visibility("protected")));
extern char _Unwind_DeleteException __attribute((visibility("protected")));
extern char _Unwind_VRS_Get __attribute((visibility("protected")));
extern char _Unwind_VRS_Set __attribute((visibility("protected")));
extern char __gnu_Unwind_Backtrace __attribute((visibility("protected")));
extern char _Unwind_VRS_Pop __attribute((visibility("protected")));
extern char __gnu_Unwind_Save_VFP_D __attribute((visibility("protected")));
extern char __gnu_Unwind_Save_VFP __attribute((visibility("protected")));
extern char __gnu_Unwind_Save_VFP_D_16_to_31 __attribute((visibility("protected")));
extern char __gnu_Unwind_Save_WMMXD __attribute((visibility("protected")));
extern char __gnu_Unwind_Save_WMMXC __attribute((visibility("protected")));
extern char ___Unwind_RaiseException __attribute((visibility("protected")));
extern char _Unwind_RaiseException __attribute((visibility("protected")));
extern char ___Unwind_Resume __attribute((visibility("protected")));
extern char _Unwind_Resume __attribute((visibility("protected")));
extern char ___Unwind_Resume_or_Rethrow __attribute((visibility("protected")));
extern char _Unwind_Resume_or_Rethrow __attribute((visibility("protected")));
extern char ___Unwind_ForcedUnwind __attribute((visibility("protected")));
extern char _Unwind_ForcedUnwind __attribute((visibility("protected")));
extern char ___Unwind_Backtrace __attribute((visibility("protected")));
extern char _Unwind_GetRegionStart __attribute((visibility("protected")));
extern char _Unwind_GetLanguageSpecificData __attribute((visibility("protected")));
extern char _Unwind_GetDataRelBase __attribute((visibility("protected")));
extern char _Unwind_GetTextRelBase __attribute((visibility("protected")));

void* __bionic_libgcc_unwind_symbols[] = {
    &_Unwind_Backtrace,
    &__gnu_Unwind_Find_exidx,
    &__gnu_Unwind_Restore_VFP_D,
    &__gnu_Unwind_Restore_VFP,
    &__gnu_Unwind_Restore_VFP_D_16_to_31,
    &__gnu_Unwind_Restore_WMMXD,
    &__gnu_Unwind_Restore_WMMXC,
    &_Unwind_GetCFA,
    &__gnu_Unwind_RaiseException,
    &__gnu_Unwind_ForcedUnwind,
    &__gnu_Unwind_Resume,
    &__gnu_Unwind_Resume_or_Rethrow,
    &_Unwind_Complete,
    &_Unwind_DeleteException,
    &_Unwind_VRS_Get,
    &_Unwind_VRS_Set,
    &__gnu_Unwind_Backtrace,
    &_Unwind_VRS_Pop,
    &__gnu_Unwind_Save_VFP_D,
    &__gnu_Unwind_Save_VFP,
    &__gnu_Unwind_Save_VFP_D_16_to_31,
    &__gnu_Unwind_Save_WMMXD,
    &__gnu_Unwind_Save_WMMXC,
    &___Unwind_RaiseException,
    &_Unwind_RaiseException,
    &___Unwind_Resume,
    &_Unwind_Resume,
    &___Unwind_Resume_or_Rethrow,
    &_Unwind_Resume_or_Rethrow,
    &___Unwind_ForcedUnwind,
    &_Unwind_ForcedUnwind,
    &___Unwind_Backtrace,
    &_Unwind_GetRegionStart,
    &_Unwind_GetLanguageSpecificData,
    &_Unwind_GetDataRelBase,
    &_Unwind_GetTextRelBase,
};
