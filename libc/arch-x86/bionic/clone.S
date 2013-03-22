#include <asm/unistd.h>
#include <machine/asm.h>

// int  __pthread_clone(void* (*fn)(void*), void* tls, int flags, void* arg);
ENTRY(__pthread_clone)
        pushl   %ebx
        pushl   %ecx
        movl    16(%esp), %ecx

        # save tls
        movl    %ecx, %ebx
        # 16-byte alignment on child stack
        andl    $~15, %ecx

        # insert arguments onto the child stack
        movl    12(%esp), %eax
        movl    %eax, -16(%ecx)
        movl    24(%esp), %eax
        movl    %eax, -12(%ecx)
        movl    %ebx, -8(%ecx)

        subl    $16, %ecx
        movl    20(%esp), %ebx

        # make system call
        movl    $__NR_clone, %eax
        int     $0x80

        cmpl    $0, %eax
        je      pc_child
        jg      pc_parent

        # an error occurred, set errno and return -1
        negl    %eax
        pushl   %eax
        call    __set_errno
        addl    $4, %esp
        orl     $-1, %eax
        jmp     pc_return

pc_child:
        # we're in the child thread now, call __thread_entry
        # with the appropriate arguments on the child stack
        # we already placed most of them
        call    __thread_entry
        hlt

pc_parent:
        # we're the parent; nothing to do.
pc_return:
        popl    %ecx
        popl    %ebx
        ret
END(__pthread_clone)


/*
 * int  __bionic_clone(unsigned long clone_flags,
 *                     void*         newsp,
 *                     int           *parent_tidptr,
 *                     void          *new_tls,
 *                     int           *child_tidptr,
 *                     int           (*fn)(void *),
 *                     void          *arg);
 */
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
