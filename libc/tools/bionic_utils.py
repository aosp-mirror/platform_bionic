# common python utility routines for the Bionic tool scripts

import sys, os, commands, string

all_arches = [ "arm", "arm64", "mips", "mips64", "x86", "x86_64" ]

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
        return_type  = string.join(return_type[:-1],' ')
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
            syscall_aliases = string.split(alias_list, ',')
        else:
            syscall_aliases = []

        if pos_rparen > pos_lparen+1:
            syscall_params = line[pos_lparen+1:pos_rparen].split(',')
            params         = string.join(syscall_params,',')
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
            for arch in all_arches:
                t[arch] = True
        else:
            for arch in string.split(arch_list, ','):
                if arch in all_arches:
                    t[arch] = True
                else:
                    E("invalid syscall architecture '%s' in '%s'" % (arch, line))
                    return

        self.syscalls.append(t)

        global verbose
        if verbose >= 2:
            print t


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
