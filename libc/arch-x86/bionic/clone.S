#include <sys/linux-syscalls.h>

.text

/*
 * int  __pthread_clone(int (*fn)(void*), void *tls, int flags,
 *                      void *arg);
 */
.globl __pthread_clone
.type __pthread_clone, @function
.align 4
__pthread_clone:
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
        movl    $__NR_clone, %eax
        int     $0x80
        test    %eax, %eax
        jns     1f

        # an error occurred, set errno and return -1
        negl    %eax
        call    __set_errno
        orl     $-1, %eax
        jmp     2f

1:
        jnz     2f

        # we're in the child thread now, call __thread_entry
        # with the appropriate arguments on the child stack
        # we already placed most of them
        call    __thread_entry
        hlt

2:
        popl    %ecx
        popl    %ebx
        ret


/*
 * int  __bionic_clone(unsigned long clone_flags,
 *                     void*         newsp,
 *                     int           *parent_tidptr,
 *                     void          *new_tls,
 *                     int           *child_tidptr,
 *                     int           (*fn)(void *),
 *                     void          *arg);
 */
.text
.globl __bionic_clone
.type __bionic_clone, @function
.align 4
__bionic_clone:
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
        movl    $__NR_clone, %eax
        int     $0x80
        test    %eax, %eax
        jns     1f

        # an error occurred, set errno and return -1
        negl    %eax
        call    __set_errno
        orl     $-1, %eax
        jmp     2f

1:
        jnz     2f

        # we're in the child now, call __bionic_clone_entry
        # with the appropriate arguments on the child stack
        # we already placed most of them
        call    __bionic_clone_entry
        hlt

2:
        popl    %edi
        popl    %esi
        popl    %ebx
        ret
