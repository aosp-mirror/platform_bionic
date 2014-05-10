#include <private/bionic_asm.h>

// pid_t __bionic_clone(int flags, void* child_stack, pid_t* parent_tid, void* tls, pid_t* child_tid, int (*fn)(void*), void* arg);
ENTRY(__bionic_clone)
        pushl   %ebx
        pushl   %esi
        pushl   %edi

        # Load system call arguments into registers.
        movl    16(%esp), %ebx   # flags
        movl    20(%esp), %ecx   # child_stack
        movl    24(%esp), %edx   # parent_tid
        movl    28(%esp), %esi   # tls
        movl    32(%esp), %edi   # child_tid

        # Copy 'fn' and 'arg' onto the child stack
        movl    36(%esp), %eax   # Read 'fn'.
        movl    %eax, -16(%ecx)  # Write 'fn'.
        movl    40(%esp), %eax   # Read 'arg'.
        movl    %eax, -12(%ecx)  # Write 'arg'.
        subl    $16, %ecx

        # Make the system call.
        movl    $__NR_clone, %eax
        int     $0x80

        # Check result.
        cmpl    $0, %eax
        je      .L_bc_child
        jg      .L_bc_parent

        # An error occurred, so set errno and return -1.
        negl    %eax
        pushl   %eax
        call    __set_errno
        addl    $4, %esp
        orl     $-1, %eax
        jmp     .L_bc_return

.L_bc_child:
        call    __bionic_clone_entry
        hlt

.L_bc_parent:
        # we're the parent; nothing to do.
.L_bc_return:
        popl    %edi
        popl    %esi
        popl    %ebx
        ret
END(__bionic_clone)
.hidden __bionic_clone
