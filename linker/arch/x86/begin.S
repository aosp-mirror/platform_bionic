.text
.align 4
.type _start, @function
.globl _start

_start:
        /* save the elfdata ptr to %eax, AND push it onto the stack */
        mov    %esp, %eax
        pushl  %esp

        pushl  %eax
        call   __linker_init

        /* linker init returns (%eax) the _entry address in the main image */
        /* entry point expects sp to point to elfdata */
        popl   %esp
        jmp    *%eax

.section .ctors, "wa"
.globl __CTOR_LIST__

__CTOR_LIST__:
        .long -1
