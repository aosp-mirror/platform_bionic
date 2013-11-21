#include <asm/unistd.h>
#include <machine/asm.h>

// pid_t __bionic_clone(int flags, void* child_stack, pid_t* parent_tid, void* tls, pid_t* child_tid, int (*fn)(void*), void* arg);
ENTRY(__bionic_clone)
        pushl   %ebx
        pushl   %esi
        pushl   %edi

        # insert arguments onto the child stack
        movl    20(%esp), %ecx
        andl    $~15, %ecx
        movl    36(%esp), %eax
        movl    %eax, -16(%ecx)
        movl    40(%esp), %eax
        movl    %eax, -12(%ecx)

        subl    $16, %ecx
        movl    16(%esp), %ebx
        movl    24(%esp), %edx
        movl    32(%esp), %esi
        movl    28(%esp), %edi

        # make system call
        movl    $__NR_clone, %eax
        int     $0x80

        cmpl    $0, %eax
        je      bc_child
        jg      bc_parent

        # an error occurred, set errno and return -1
        negl    %eax
        pushl   %eax
        call    __set_errno
        addl    $4, %esp
        orl     $-1, %eax
        jmp     bc_return

bc_child:
        # we're in the child now, call __bionic_clone_entry
        # with the appropriate arguments on the child stack
        # we already placed most of them
        call    __bionic_clone_entry
        hlt

bc_parent:
        # we're the parent; nothing to do.
bc_return:
        popl    %edi
        popl    %esi
        popl    %ebx
        ret
END(__bionic_clone)
