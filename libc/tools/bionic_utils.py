# common python utility routines for the Bionic tool scripts

import sys, os, commands, string

# support Bionic architectures, add new ones as appropriate
#
bionic_archs = [ "arm", "x86", "mips" ]

# basic debugging trace support
# call D_setlevel to set the verbosity level
# and D(), D2(), D3(), D4() to add traces
#
verbose = 0

def D(msg):
    global verbose
    if verbose > 0:
        print msg

def D2(msg):
    global verbose
    if verbose >= 2:
        print msg

def D3(msg):
    global verbose
    if verbose >= 3:
        print msg

def D4(msg):
    global verbose
    if verbose >= 4:
        print msg

def D_setlevel(level):
    global verbose
    verbose = level


# parser for the SYSCALLS.TXT file
#
class SysCallsTxtParser:
    def __init__(self):
        self.syscalls = []
        self.lineno   = 0

    def E(self, msg):
        print "%d: %s" % (self.lineno, msg)

    def parse_line(self, line):
        """ parse a syscall spec line.

        line processing, format is
           return type    func_name[:syscall_name[:call_id]] ( [paramlist] )   (syscall_number[,syscall_number_x86])|stub
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
        return_type  = string.join(return_type[:-1],' ')
        call_id = -1

        pos_colon = syscall_func.find(':')
        if pos_colon < 0:
            syscall_name = syscall_func
        else:
            if pos_colon == 0 or pos_colon+1 >= len(syscall_func):
                E("misplaced colon in '%s'" % line)
                return

            # now find if there is a call_id for a dispatch-type syscall
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
                call_id = int(syscall_func[pos_colon2+1:])
                syscall_func = syscall_func[:pos_colon]

        if pos_rparen > pos_lparen+1:
            syscall_params = line[pos_lparen+1:pos_rparen].split(',')
            params         = string.join(syscall_params,',')
        else:
            syscall_params = []
            params         = "void"

        number = line[pos_rparen+1:].strip()
        if number == "stub":
            syscall_common = -1
            syscall_arm  = -1
            syscall_x86 = -1
            syscall_mips = -1
        else:
            try:
                if number[0] == '#':
                    number = number[1:].strip()
                numbers = string.split(number,',')
                if len(numbers) == 1:
                    syscall_common = int(numbers[0])
                    syscall_arm = -1
                    syscall_x86 = -1
                    syscall_mips = -1
                else:
                    if len(numbers) == 3:
                        syscall_common = -1
                        syscall_arm  = int(numbers[0])
                        syscall_x86 = int(numbers[1])
                        syscall_mips = int(numbers[2])
                    else:
                        E("invalid syscall number format in '%s'" % line)
                        return
            except:
                E("invalid syscall number in '%s'" % line)
                return

        global verbose
        if verbose >= 2:
            if call_id == -1:
                if syscall_common == -1:
                    print "%s: %d,%d,%d" % (syscall_name, syscall_arm, syscall_x86, syscall_mips)
                else:
                    print "%s: %d" % (syscall_name, syscall_common)
            else:
                if syscall_common == -1:
                    print "%s(%d): %d,%d,%d" % (syscall_name, call_id, syscall_arm, syscall_x86, syscall_mips)
                else:
                    print "%s(%d): %d" % (syscall_name, call_id, syscall_common)

        t = { "armid"  : syscall_arm,
              "x86id"  : syscall_x86,
              "mipsid" : syscall_mips,
              "common" : syscall_common,
              "cid"    : call_id,
              "name"   : syscall_name,
              "func"   : syscall_func,
              "params" : syscall_params,
              "decl"   : "%-15s  %s (%s);" % (return_type, syscall_func, params) }
        self.syscalls.append(t)

    def parse_file(self, file_path):
        D2("parse_file: %s" % file_path)
        fp = open(file_path)
        for line in fp.xreadlines():
            self.lineno += 1
            line = line.strip()
            if not line: continue
            if line[0] == '#': continue
            self.parse_line(line)

        fp.close()


class StringOutput:
    def __init__(self):
        self.line = ""

    def write(self,msg):
        self.line += msg
        D2("write '%s'" % msg)

    def get(self):
        return self.line
