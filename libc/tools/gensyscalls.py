#!/usr/bin/env python3

# This tool is used to generate the assembler system call stubs,
# the header files listing all available system calls, and the
# makefiles used to build all the stubs.

import atexit
import filecmp
import glob
import re
import shutil
import stat
import string
import sys
import tempfile


SupportedArchitectures = [ "arm", "arm64", "riscv64", "x86", "x86_64" ]

syscall_stub_header = \
"""
ENTRY(%(func)s)
"""


#
# ARM assembler templates for each syscall stub
#

arm_eabi_call_default = syscall_stub_header + """\
    mov     ip, r7
    .cfi_register r7, ip
    ldr     r7, =%(__NR_name)s
    swi     #0
    mov     r7, ip
    .cfi_restore r7
    cmn     r0, #(MAX_ERRNO + 1)
    bxls    lr
    neg     r0, r0
    b       __set_errno_internal
END(%(func)s)
"""

arm_eabi_call_long = syscall_stub_header + """\
    mov     ip, sp
    stmfd   sp!, {r4, r5, r6, r7}
    .cfi_def_cfa_offset 16
    .cfi_rel_offset r4, 0
    .cfi_rel_offset r5, 4
    .cfi_rel_offset r6, 8
    .cfi_rel_offset r7, 12
    ldmfd   ip, {r4, r5, r6}
    ldr     r7, =%(__NR_name)s
    swi     #0
    ldmfd   sp!, {r4, r5, r6, r7}
    .cfi_def_cfa_offset 0
    cmn     r0, #(MAX_ERRNO + 1)
    bxls    lr
    neg     r0, r0
    b       __set_errno_internal
END(%(func)s)
"""


#
# Arm64 assembler template for each syscall stub
#

arm64_call = syscall_stub_header + """\
    mov     x8, %(__NR_name)s
    svc     #0

    cmn     x0, #(MAX_ERRNO + 1)
    cneg    x0, x0, hi
    b.hi    __set_errno_internal

    ret
END(%(func)s)
"""


#
# RISC-V64 assembler templates for each syscall stub
#

riscv64_call = syscall_stub_header + """\
    li      a7, %(__NR_name)s
    ecall

    li      a7, -MAX_ERRNO
    bgeu    a0, a7, 1f

    ret
1:
    neg     a0, a0
    tail    __set_errno_internal
END(%(func)s)
"""

#
# x86 assembler templates for each syscall stub
#

x86_registers = [ "ebx", "ecx", "edx", "esi", "edi", "ebp" ]

x86_call_prepare = """\

    call    __kernel_syscall
    pushl   %eax
    .cfi_adjust_cfa_offset 4
    .cfi_rel_offset eax, 0

"""

x86_call = """\
    movl    $%(__NR_name)s, %%eax
    call    *(%%esp)
    addl    $4, %%esp

    cmpl    $-MAX_ERRNO, %%eax
    jb      1f
    negl    %%eax
    pushl   %%eax
    call    __set_errno_internal
    addl    $4, %%esp
1:
"""

x86_return = """\
    ret
END(%(func)s)
"""


#
# x86_64 assembler template for each syscall stub
#

x86_64_call = """\
    movl    $%(__NR_name)s, %%eax
    syscall
    cmpq    $-MAX_ERRNO, %%rax
    jb      1f
    negl    %%eax
    movl    %%eax, %%edi
    call    __set_errno_internal
1:
    ret
END(%(func)s)
"""


def param_uses_64bits(param):
    """Returns True iff a syscall parameter description corresponds
       to a 64-bit type."""
    param = param.strip()
    # First, check that the param type begins with one of the known
    # 64-bit types.
    if not ( \
       param.startswith("int64_t") or param.startswith("uint64_t") or \
       param.startswith("loff_t") or param.startswith("off64_t") or \
       param.startswith("long long") or param.startswith("unsigned long long") or
       param.startswith("signed long long") ):
           return False

    # Second, check that there is no pointer type here
    if param.find("*") >= 0:
            return False

    # Ok
    return True


def count_arm_param_registers(params):
    """This function is used to count the number of register used
       to pass parameters when invoking an ARM system call.
       This is because the ARM EABI mandates that 64-bit quantities
       must be passed in an even+odd register pair. So, for example,
       something like:

             foo(int fd, off64_t pos)

       would actually need 4 registers:
             r0 -> int
             r1 -> unused
             r2-r3 -> pos
   """
    count = 0
    for param in params:
        if param_uses_64bits(param):
            if (count & 1) != 0:
                count += 1
            count += 2
        else:
            count += 1
    return count


def count_generic_param_registers(params):
    count = 0
    for param in params:
        if param_uses_64bits(param):
            count += 2
        else:
            count += 1
    return count


def count_generic_param_registers64(params):
    count = 0
    for param in params:
        count += 1
    return count


# This lets us support regular system calls like __NR_write and also weird
# ones like __ARM_NR_cacheflush, where the NR doesn't come at the start.
def make__NR_name(name):
    if name.startswith("__ARM_NR_"):
        return name
    else:
        return "__NR_%s" % (name)


def add_footer(pointer_length, stub, syscall):
    # Add any aliases for this syscall.
    aliases = syscall["aliases"]
    for alias in aliases:
        stub += "\nALIAS_SYMBOL(%s, %s)\n" % (alias, syscall["func"])
    return stub


def arm_eabi_genstub(syscall):
    num_regs = count_arm_param_registers(syscall["params"])
    if num_regs > 4:
        return arm_eabi_call_long % syscall
    return arm_eabi_call_default % syscall


def arm64_genstub(syscall):
    return arm64_call % syscall


def riscv64_genstub(syscall):
    return riscv64_call % syscall


def x86_genstub(syscall):
    result     = syscall_stub_header % syscall

    numparams = count_generic_param_registers(syscall["params"])
    stack_bias = numparams*4 + 8
    offset = 0
    mov_result = ""
    first_push = True
    for register in x86_registers[:numparams]:
        result     += "    pushl   %%%s\n" % register
        if first_push:
          result   += "    .cfi_def_cfa_offset 8\n"
          result   += "    .cfi_rel_offset %s, 0\n" % register
          first_push = False
        else:
          result   += "    .cfi_adjust_cfa_offset 4\n"
          result   += "    .cfi_rel_offset %s, 0\n" % register
        mov_result += "    mov     %d(%%esp), %%%s\n" % (stack_bias+offset, register)
        offset += 4

    result += x86_call_prepare
    result += mov_result
    result += x86_call % syscall

    for register in reversed(x86_registers[:numparams]):
        result += "    popl    %%%s\n" % register

    result += x86_return % syscall
    return result


def x86_genstub_socketcall(syscall):
    #   %ebx <--- Argument 1 - The call id of the needed vectored
    #                          syscall (socket, bind, recv, etc)
    #   %ecx <--- Argument 2 - Pointer to the rest of the arguments
    #                          from the original function called (socket())

    result = syscall_stub_header % syscall

    # save the regs we need
    result += "    pushl   %ebx\n"
    result += "    .cfi_def_cfa_offset 8\n"
    result += "    .cfi_rel_offset ebx, 0\n"
    result += "    pushl   %ecx\n"
    result += "    .cfi_adjust_cfa_offset 4\n"
    result += "    .cfi_rel_offset ecx, 0\n"
    stack_bias = 16

    result += x86_call_prepare

    # set the call id (%ebx)
    result += "    mov     $%d, %%ebx\n" % syscall["socketcall_id"]

    # set the pointer to the rest of the args into %ecx
    result += "    mov     %esp, %ecx\n"
    result += "    addl    $%d, %%ecx\n" % (stack_bias)

    # now do the syscall code itself
    result += x86_call % syscall

    # now restore the saved regs
    result += "    popl    %ecx\n"
    result += "    popl    %ebx\n"

    # epilog
    result += x86_return % syscall
    return result


def x86_64_genstub(syscall):
    result = syscall_stub_header % syscall
    num_regs = count_generic_param_registers64(syscall["params"])
    if (num_regs > 3):
        # rcx is used as 4th argument. Kernel wants it at r10.
        result += "    movq    %rcx, %r10\n"

    result += x86_64_call % syscall
    return result


class SysCallsTxtParser:
    def __init__(self):
        self.syscalls = []
        self.lineno = 0
        self.errors = False

    def E(self, msg):
        print("%d: %s" % (self.lineno, msg))
        self.errors = True

    def parse_line(self, line):
        """ parse a syscall spec line.

        line processing, format is
           return type    func_name[|alias_list][:syscall_name[:socketcall_id]] ( [paramlist] ) architecture_list
        """
        pos_lparen = line.find('(')
        E          = self.E
        if pos_lparen < 0:
            E("missing left parenthesis in '%s'" % line)
            return

        pos_rparen = line.rfind(')')
        if pos_rparen < 0 or pos_rparen <= pos_lparen:
            E("missing or misplaced right parenthesis in '%s'" % line)
            return

        return_type = line[:pos_lparen].strip().split()
        if len(return_type) < 2:
            E("missing return type in '%s'" % line)
            return

        syscall_func = return_type[-1]
        return_type  = ' '.join(return_type[:-1])
        socketcall_id = -1

        pos_colon = syscall_func.find(':')
        if pos_colon < 0:
            syscall_name = syscall_func
        else:
            if pos_colon == 0 or pos_colon+1 >= len(syscall_func):
                E("misplaced colon in '%s'" % line)
                return

            # now find if there is a socketcall_id for a dispatch-type syscall
            # after the optional 2nd colon
            pos_colon2 = syscall_func.find(':', pos_colon + 1)
            if pos_colon2 < 0:
                syscall_name = syscall_func[pos_colon+1:]
                syscall_func = syscall_func[:pos_colon]
            else:
                if pos_colon2+1 >= len(syscall_func):
                    E("misplaced colon2 in '%s'" % line)
                    return
                syscall_name = syscall_func[(pos_colon+1):pos_colon2]
                socketcall_id = int(syscall_func[pos_colon2+1:])
                syscall_func = syscall_func[:pos_colon]

        alias_delim = syscall_func.find('|')
        if alias_delim > 0:
            alias_list = syscall_func[alias_delim+1:].strip()
            syscall_func = syscall_func[:alias_delim]
            alias_delim = syscall_name.find('|')
            if alias_delim > 0:
                syscall_name = syscall_name[:alias_delim]
            syscall_aliases = alias_list.split(',')
        else:
            syscall_aliases = []

        if pos_rparen > pos_lparen+1:
            syscall_params = line[pos_lparen+1:pos_rparen].split(',')
            params         = ','.join(syscall_params)
        else:
            syscall_params = []
            params         = "void"

        t = {
              "name"    : syscall_name,
              "func"    : syscall_func,
              "aliases" : syscall_aliases,
              "params"  : syscall_params,
              "decl"    : "%-15s  %s (%s);" % (return_type, syscall_func, params),
              "socketcall_id" : socketcall_id
        }

        # Parse the architecture list.
        arch_list = line[pos_rparen+1:].strip()
        if arch_list == "all":
            for arch in SupportedArchitectures:
                t[arch] = True
        else:
            for arch in arch_list.split(','):
                if arch == "lp32":
                    for arch in SupportedArchitectures:
                        if "64" not in arch:
                          t[arch] = True
                elif arch == "lp64":
                    for arch in SupportedArchitectures:
                        if "64" in arch:
                            t[arch] = True
                elif arch in SupportedArchitectures:
                    t[arch] = True
                else:
                    E("invalid syscall architecture '%s' in '%s'" % (arch, line))
                    return

        self.syscalls.append(t)

    def parse_open_file(self, fp):
        for line in fp:
            self.lineno += 1
            line = line.strip()
            if not line: continue
            if line[0] == '#': continue
            self.parse_line(line)
        if self.errors:
            sys.exit(1)

    def parse_file(self, file_path):
        with open(file_path) as fp:
            self.parse_open_file(fp)


def main(arch, syscall_file):
    parser = SysCallsTxtParser()
    parser.parse_file(syscall_file)

    for syscall in parser.syscalls:
        syscall["__NR_name"] = make__NR_name(syscall["name"])

        if "arm" in syscall:
            syscall["asm-arm"] = add_footer(32, arm_eabi_genstub(syscall), syscall)

        if "arm64" in syscall:
            syscall["asm-arm64"] = add_footer(64, arm64_genstub(syscall), syscall)

        if "riscv64" in syscall:
            syscall["asm-riscv64"] = add_footer(64, riscv64_genstub(syscall), syscall)

        if "x86" in syscall:
            if syscall["socketcall_id"] >= 0:
                syscall["asm-x86"] = add_footer(32, x86_genstub_socketcall(syscall), syscall)
            else:
                syscall["asm-x86"] = add_footer(32, x86_genstub(syscall), syscall)
        elif syscall["socketcall_id"] >= 0:
            E("socketcall_id for dispatch syscalls is only supported for x86 in '%s'" % t)
            return

        if "x86_64" in syscall:
            syscall["asm-x86_64"] = add_footer(64, x86_64_genstub(syscall), syscall)

    print("/* Generated by gensyscalls.py. Do not edit. */\n")
    print("#include <private/bionic_asm.h>\n")
    for syscall in parser.syscalls:
        if ("asm-%s" % arch) in syscall:
            print(syscall["asm-%s" % arch])

    if arch == 'arm64':
        print('\nNOTE_GNU_PROPERTY()\n')

if __name__ == "__main__":
    if len(sys.argv) < 2:
      print("Usage: gensyscalls.py ARCH SOURCE_FILE")
      sys.exit(1)

    arch = sys.argv[1]
    syscall_file = sys.argv[2]
    main(arch, syscall_file)
