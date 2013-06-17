# common python utility routines for the Bionic tool scripts

import sys, os, commands, string, commands

# basic debugging trace support
# call D_setlevel to set the verbosity level
# and D(), D2(), D3(), D4() to add traces
#
verbose = 0

def panic(msg):
    sys.stderr.write( find_program_name() + ": error: " )
    sys.stderr.write( msg )
    sys.exit(1)

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


#  other stuff
#
#
def find_program_name():
    return os.path.basename(sys.argv[0])

def find_program_dir():
    return os.path.dirname(sys.argv[0])

class StringOutput:
    def __init__(self):
        self.line = ""

    def write(self,msg):
        self.line += msg
        D2("write '%s'" % msg)

    def get(self):
        return self.line


def create_file_path(path):
    dirs = []
    while 1:
        parent = os.path.dirname(path)
        #print "parent: %s <- %s" % (parent, path)
        if parent == "/" or parent == "":
            break
        dirs.append(parent)
        path = parent

    dirs.reverse()
    for dir in dirs:
        #print "dir %s" % dir
        if os.path.isdir(dir):
            continue
        os.mkdir(dir)

def walk_source_files(paths,callback,args,excludes=[]):
    """recursively walk a list of paths and files, only keeping the source files in directories"""
    for path in paths:
        if len(path) > 0 and path[0] == '@':
            # this is the name of another file, include it and parse it
            path = path[1:]
            if os.path.exists(path):
                for line in open(path):
                    if len(line) > 0 and line[-1] == '\n':
                        line = line[:-1]
                    walk_source_files([line],callback,args,excludes)
            continue
        if not os.path.isdir(path):
            callback(path,args)
        else:
            for root, dirs, files in os.walk(path):
                #print "w-- %s (ex: %s)" % (repr((root,dirs)), repr(excludes))
                if len(excludes):
                    for d in dirs[:]:
                        if os.path.join(root,d) in excludes:
                            dirs.remove(d)
                for f in files:
                    r, ext = os.path.splitext(f)
                    if ext in [ ".h", ".c", ".cpp", ".S" ]:
                        callback( "%s/%s" % (root,f), args )

def cleanup_dir(path):
    """create a directory if needed, and ensure that it is totally empty
       by removing any existing content in it"""
    if not os.path.exists(path):
        os.mkdir(path)
    else:
        for root, dirs, files in os.walk(path, topdown=False):
            if root.endswith("kernel_headers/"):
                # skip 'kernel_headers'
                continue
            for name in files:
                os.remove(os.path.join(root, name))
            for name in dirs:
                os.rmdir(os.path.join(root, name))


class BatchFileUpdater:
    """a class used to edit several files at once"""
    def __init__(self):
        self.old_files = set()
        self.new_files = set()
        self.new_data  = {}

    def readFile(self,path):
        #path = os.path.realpath(path)
        if os.path.exists(path):
            self.old_files.add(path)

    def readDir(self,path):
        #path = os.path.realpath(path)
        for root, dirs, files in os.walk(path):
            for f in files:
                dst = "%s/%s" % (root,f)
                self.old_files.add(dst)

    def editFile(self,dst,data):
        """edit a destination file. if the file is not mapped from a source,
           it will be added. return 0 if the file content wasn't changed,
           1 if it was edited, or 2 if the file is new"""
        #dst = os.path.realpath(dst)
        result = 1
        if os.path.exists(dst):
            f = open(dst, "r")
            olddata = f.read()
            f.close()
            if olddata == data:
                self.old_files.remove(dst)
                return 0
        else:
            result = 2

        self.new_data[dst] = data
        self.new_files.add(dst)
        return result

    def getChanges(self):
        """determine changes, returns (adds, deletes, edits)"""
        adds    = set()
        edits   = set()
        deletes = set()

        for dst in self.new_files:
            if not (dst in self.old_files):
                adds.add(dst)
            else:
                edits.add(dst)

        for dst in self.old_files:
            if not dst in self.new_files:
                deletes.add(dst)

        return (adds, deletes, edits)

    def _writeFile(self,dst):
        if not os.path.exists(os.path.dirname(dst)):
            create_file_path(dst)
        f = open(dst, "w")
        f.write(self.new_data[dst])
        f.close()

    def updateFiles(self):
        adds, deletes, edits = self.getChanges()

        for dst in sorted(adds):
            self._writeFile(dst)

        for dst in sorted(edits):
            self._writeFile(dst)

        for dst in sorted(deletes):
            os.remove(dst)

    def updateGitFiles(self):
        adds, deletes, edits = self.getChanges()

        if adds:
            for dst in sorted(adds):
                self._writeFile(dst)
            commands.getoutput("git add " + " ".join(adds))

        if deletes:
            commands.getoutput("git rm " + " ".join(deletes))

        if edits:
            for dst in sorted(edits):
                self._writeFile(dst)
            commands.getoutput("git add " + " ".join(edits))
