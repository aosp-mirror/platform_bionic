# This function is to be called when the shared library
# is unloaded through dlclose()
_on_dlclose:
    lea __dso_handle, %eax
    call __cxa_finalize
    ret

.section .init_array, "aw"
.align 4
.type __INIT_ARRAY__, @object
.globl __INIT_ARRAY__
__INIT_ARRAY__:
    .long -1

.section .fini_array, "aw"
.align 4
.type __FINI_ARRAY__, @object
.globl __FINI_ARRAY__
__FINI_ARRAY__:
    .long -1
    .long _on_dlclose

#include "__dso_handle.S"
