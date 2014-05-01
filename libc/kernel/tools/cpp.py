# a glorified C pre-processor parser

import sys, re, string
from utils import *
from defaults import *

debugTokens             = False
debugDirectiveTokenizer = False
debugLineParsing        = False
debugCppExpr            = False
debugOptimIf01          = False

#####################################################################################
#####################################################################################
#####                                                                           #####
#####           C P P   T O K E N S                                             #####
#####                                                                           #####
#####################################################################################
#####################################################################################

# the list of supported C-preprocessor tokens
# plus a couple of C tokens as well
tokEOF       = "\0"
tokLN        = "\n"
tokSTRINGIFY = "#"
tokCONCAT    = "##"
tokLOGICAND  = "&&"
tokLOGICOR   = "||"
tokSHL       = "<<"
tokSHR       = ">>"
tokEQUAL     = "=="
tokNEQUAL    = "!="
tokLT        = "<"
tokLTE       = "<="
tokGT        = ">"
tokGTE       = ">="
tokELLIPSIS  = "..."
tokSPACE     = " "
tokDEFINED   = "defined"
tokLPAREN    = "("
tokRPAREN    = ")"
tokNOT       = "!"
tokPLUS      = "+"
tokMINUS     = "-"
tokMULTIPLY  = "*"
tokDIVIDE    = "/"
tokMODULUS   = "%"
tokBINAND    = "&"
tokBINOR     = "|"
tokBINXOR    = "^"
tokCOMMA     = ","
tokLBRACE    = "{"
tokRBRACE    = "}"
tokARROW     = "->"
tokINCREMENT = "++"
tokDECREMENT = "--"
tokNUMBER    = "<number>"
tokIDENT     = "<ident>"
tokSTRING    = "<string>"

class Token:
    """a simple class to hold information about a given token.
       each token has a position in the source code, as well as
       an 'id' and a 'value'. the id is a string that identifies
       the token's class, while the value is the string of the
       original token itself.

       for example, the tokenizer concatenates a series of spaces
       and tabs as a single tokSPACE id, whose value if the original
       spaces+tabs sequence."""

    def __init__(self):
        self.id     = None
        self.value  = None
        self.lineno = 0
        self.colno  = 0

    def set(self,id,val=None):
        self.id = id
        if val:
            self.value = val
        else:
            self.value = id
        return None

    def copyFrom(self,src):
        self.id     = src.id
        self.value  = src.value
        self.lineno = src.lineno
        self.colno  = src.colno

    def __repr__(self):
        if self.id == tokIDENT:
            return "(ident %s)" % self.value
        if self.id == tokNUMBER:
            return "(number %s)" % self.value
        if self.id == tokSTRING:
            return "(string '%s')" % self.value
        if self.id == tokLN:
            return "<LN>"
        if self.id == tokEOF:
            return "<EOF>"
        if self.id == tokSPACE and self.value == "\\":
            # this corresponds to a trailing \ that was transformed into a tokSPACE
            return "<\\>"

        return self.id

    def __str__(self):
        if self.id == tokIDENT:
            return self.value
        if self.id == tokNUMBER:
            return self.value
        if self.id == tokSTRING:
            return self.value
        if self.id == tokEOF:
            return "<EOF>"
        if self.id == tokSPACE:
            if self.value == "\\":  # trailing \
                return "\\\n"
            else:
                return self.value

        return self.id

class BadExpectedToken(Exception):
    def __init__(self,msg):
        print msg


#####################################################################################
#####################################################################################
#####                                                                           #####
#####           C P P   T O K E N I Z E R                                       #####
#####                                                                           #####
#####################################################################################
#####################################################################################

# list of long symbols, i.e. those that take more than one characters
cppLongSymbols = [ tokCONCAT, tokLOGICAND, tokLOGICOR, tokSHL, tokSHR, tokELLIPSIS, tokEQUAL,\
                   tokNEQUAL, tokLTE, tokGTE, tokARROW, tokINCREMENT, tokDECREMENT ]

class CppTokenizer:
    """an abstract class used to convert some input text into a list
       of tokens. real implementations follow and differ in the format
       of the input text only"""

    def __init__(self):
        """initialize a new CppTokenizer object"""
        self.eof  = False  # end of file reached ?
        self.text = None   # content of current line, with final \n stripped
        self.line = 0      # number of current line
        self.pos  = 0      # current character position in current line
        self.len  = 0      # length of current line text
        self.held = Token()

    def setLineText(self,line):
        """set the content of the (next) current line. should be called
           by fillLineText() in derived classes"""
        self.text = line
        self.len  = len(line)
        self.pos  = 0

    def fillLineText(self):
        """refresh the content of 'line' with a new line of input"""
        # to be overriden
        self.eof = True

    def markPos(self,tok):
        """mark the position of the current token in the source file"""
        if self.eof or self.pos > self.len:
            tok.lineno = self.line + 1
            tok.colno  = 0
        else:
            tok.lineno = self.line
            tok.colno  = self.pos

    def peekChar(self):
        """return the current token under the cursor without moving it"""
        if self.eof:
            return tokEOF

        if self.pos > self.len:
            self.pos   = 0
            self.line += 1
            self.fillLineText()
            if self.eof:
                return tokEOF

        if self.pos == self.len:
            return tokLN
        else:
            return self.text[self.pos]

    def peekNChar(self,n):
        """try to peek the next n chars on the same line"""
        if self.pos + n > self.len:
            return None
        return self.text[self.pos:self.pos+n]

    def skipChar(self):
        """increment the token cursor position"""
        if not self.eof:
            self.pos += 1

    def skipNChars(self,n):
        if self.pos + n <= self.len:
            self.pos += n
        else:
            while n > 0:
                self.skipChar()
                n -= 1

    def nextChar(self):
        """retrieve the token at the current cursor position, then skip it"""
        result = self.peekChar()
        self.skipChar()
        return  result

    def getEscape(self):
        # try to get all characters after a backslash (\)
        result = self.nextChar()
        if result == "0":
            # octal number ?
            num = self.peekNChar(3)
            if num != None:
                isOctal = True
                for d in num:
                    if not d in "01234567":
                        isOctal = False
                        break
                if isOctal:
                    result += num
                    self.skipNChars(3)
        elif result == "x" or result == "X":
            # hex number ?
            num = self.peekNChar(2)
            if num != None:
                isHex = True
                for d in num:
                    if not d in "012345678abcdefABCDEF":
                        isHex = False
                        break
                if isHex:
                    result += num
                    self.skipNChars(2)
        elif result == "u" or result == "U":
            # unicode char ?
            num = self.peekNChar(4)
            if num != None:
                isHex = True
                for d in num:
                    if not d in "012345678abcdefABCDEF":
                        isHex = False
                        break
                if isHex:
                    result += num
                    self.skipNChars(4)

        return result

    def nextRealToken(self,tok):
        """return next CPP token, used internally by nextToken()"""
        c = self.nextChar()
        if c == tokEOF or c == tokLN:
            return tok.set(c)

        if c == '/':
            c = self.peekChar()
            if c == '/':   # C++ comment line
                self.skipChar()
                while 1:
                    c = self.nextChar()
                    if c == tokEOF or c == tokLN:
                        break
                return tok.set(tokLN)
            if c == '*':   # C comment start
                self.skipChar()
                value = "/*"
                prev_c = None
                while 1:
                    c = self.nextChar()
                    if c == tokEOF:
                        return tok.set(tokEOF,value)
                    if c == '/' and prev_c == '*':
                        break
                    prev_c = c
                    value += c

                value += "/"
                return tok.set(tokSPACE,value)
            c = '/'

        if c.isspace():
            while 1:
                c2 = self.peekChar()
                if c2 == tokLN or not c2.isspace():
                    break
                c += c2
                self.skipChar()
            return tok.set(tokSPACE,c)

        if c == '\\':
            if debugTokens:
                print "nextRealToken: \\ found, next token is '%s'" % repr(self.peekChar())
            if self.peekChar() == tokLN:   # trailing \
                # eat the tokLN
                self.skipChar()
                # we replace a trailing \ by a tokSPACE whose value is
                # simply "\\". this allows us to detect them later when
                # needed.
                return tok.set(tokSPACE,"\\")
            else:
                # treat as a single token here ?
                c +=self.getEscape()
                return tok.set(c)

        if c == "'":  # chars
            c2 = self.nextChar()
            c += c2
            if c2 == '\\':
                c += self.getEscape()

            while 1:
                c2 = self.nextChar()
                if c2 == tokEOF:
                    break
                c += c2
                if c2 == "'":
                    break

            return tok.set(tokSTRING, c)

        if c == '"':  # strings
            quote = 0
            while 1:
                c2  = self.nextChar()
                if c2 == tokEOF:
                    return tok.set(tokSTRING,c)

                c += c2
                if not quote:
                    if c2 == '"':
                        return tok.set(tokSTRING,c)
                    if c2 == "\\":
                        quote = 1
                else:
                    quote = 0

        if c >= "0" and c <= "9":  # integers ?
            while 1:
                c2 = self.peekChar()
                if c2 == tokLN or (not c2.isalnum() and c2 != "_"):
                    break
                c += c2
                self.skipChar()
            return tok.set(tokNUMBER,c)

        if c.isalnum() or c == "_":  # identifiers ?
            while 1:
                c2 = self.peekChar()
                if c2 == tokLN or (not c2.isalnum() and c2 != "_"):
                    break
                c += c2
                self.skipChar()
            if c == tokDEFINED:
                return tok.set(tokDEFINED)
            else:
                return tok.set(tokIDENT,c)

        # check special symbols
        for sk in cppLongSymbols:
            if c == sk[0]:
                sklen = len(sk[1:])
                if self.pos + sklen <= self.len and \
                   self.text[self.pos:self.pos+sklen] == sk[1:]:
                    self.pos += sklen
                    return tok.set(sk)

        return tok.set(c)

    def nextToken(self,tok):
        """return the next token from the input text. this function
           really updates 'tok', and does not return a new one"""
        self.markPos(tok)
        self.nextRealToken(tok)

    def getToken(self):
        tok = Token()
        self.nextToken(tok)
        if debugTokens:
            print "getTokens: %s" % repr(tok)
        return tok

    def toTokenList(self):
        """convert the input text of a CppTokenizer into a direct
           list of token objects. tokEOF is stripped from the result"""
        result = []
        while 1:
            tok = Token()
            self.nextToken(tok)
            if tok.id == tokEOF:
                break
            result.append(tok)
        return result

class CppLineTokenizer(CppTokenizer):
    """a CppTokenizer derived class that accepts a single line of text as input"""
    def __init__(self,line,lineno=1):
        CppTokenizer.__init__(self)
        self.line = lineno
        self.setLineText(line)


class CppLinesTokenizer(CppTokenizer):
    """a CppTokenizer derived class that accepts a list of texdt lines as input.
       the lines must not have a trailing \n"""
    def __init__(self,lines=[],lineno=1):
        """initialize a CppLinesTokenizer. you can later add lines using addLines()"""
        CppTokenizer.__init__(self)
        self.line  = lineno
        self.lines = lines
        self.index = 0
        self.count = len(lines)

        if self.count > 0:
            self.fillLineText()
        else:
            self.eof = True

    def addLine(self,line):
        """add a line to a CppLinesTokenizer. this can be done after tokenization
           happens"""
        if self.count == 0:
            self.setLineText(line)
            self.index = 1
        self.lines.append(line)
        self.count += 1
        self.eof    = False

    def fillLineText(self):
        if self.index < self.count:
            self.setLineText(self.lines[self.index])
            self.index += 1
        else:
            self.eof = True


class CppFileTokenizer(CppTokenizer):
    def __init__(self,file,lineno=1):
        CppTokenizer.__init__(self)
        self.file = file
        self.line = lineno

    def fillLineText(self):
        line = self.file.readline()
        if len(line) > 0:
            if line[-1] == '\n':
                line = line[:-1]
            if len(line) > 0 and line[-1] == "\r":
                line = line[:-1]
            self.setLineText(line)
        else:
            self.eof = True

# Unit testing
#
class CppTokenizerTester:
    """a class used to test CppTokenizer classes"""
    def __init__(self,tokenizer=None):
        self.tokenizer = tokenizer
        self.token     = Token()

    def setTokenizer(self,tokenizer):
        self.tokenizer = tokenizer

    def expect(self,id):
        self.tokenizer.nextToken(self.token)
        tokid = self.token.id
        if tokid == id:
            return
        if self.token.value == id and (tokid == tokIDENT or tokid == tokNUMBER):
            return
        raise BadExpectedToken, "###  BAD TOKEN: '%s' expecting '%s'" % (self.token.id,id)

    def expectToken(self,id,line,col):
        self.expect(id)
        if self.token.lineno != line:
            raise BadExpectedToken, "###  BAD LINENO: token '%s' got '%d' expecting '%d'" % (id,self.token.lineno,line)
        if self.token.colno != col:
            raise BadExpectedToken, "###  BAD COLNO: '%d' expecting '%d'" % (self.token.colno,col)

    def expectTokenVal(self,id,value,line,col):
        self.expectToken(id,line,col)
        if self.token.value != value:
            raise BadExpectedToken, "###  BAD VALUE: '%s' expecting '%s'" % (self.token.value,value)

    def expectList(self,list):
        for item in list:
            self.expect(item)

def test_CppTokenizer():
    tester = CppTokenizerTester()

    tester.setTokenizer( CppLineTokenizer("#an/example  && (01923_xy)") )
    tester.expectList( ["#", "an", "/", "example", tokSPACE, tokLOGICAND, tokSPACE, tokLPAREN, "01923_xy", \
                       tokRPAREN, tokLN, tokEOF] )

    tester.setTokenizer( CppLineTokenizer("FOO(BAR) && defined(BAZ)") )
    tester.expectList( ["FOO", tokLPAREN, "BAR", tokRPAREN, tokSPACE, tokLOGICAND, tokSPACE,
                        tokDEFINED, tokLPAREN, "BAZ", tokRPAREN, tokLN, tokEOF] )

    tester.setTokenizer( CppLinesTokenizer( ["/*", "#", "*/"] ) )
    tester.expectList( [ tokSPACE, tokLN, tokEOF ] )

    tester.setTokenizer( CppLinesTokenizer( ["first", "second"] ) )
    tester.expectList( [ "first", tokLN, "second", tokLN, tokEOF ] )

    tester.setTokenizer( CppLinesTokenizer( ["first second", "  third"] ) )
    tester.expectToken( "first", 1, 0 )
    tester.expectToken( tokSPACE, 1, 5 )
    tester.expectToken( "second", 1, 6 )
    tester.expectToken( tokLN, 1, 12 )
    tester.expectToken( tokSPACE, 2, 0 )
    tester.expectToken( "third", 2, 2 )

    tester.setTokenizer( CppLinesTokenizer( [ "boo /* what the", "hell */" ] ) )
    tester.expectList( [ "boo", tokSPACE ] )
    tester.expectTokenVal( tokSPACE, "/* what the\nhell */", 1, 4 )
    tester.expectList( [ tokLN, tokEOF ] )

    tester.setTokenizer( CppLinesTokenizer( [ "an \\", " example" ] ) )
    tester.expectToken( "an", 1, 0 )
    tester.expectToken( tokSPACE, 1, 2 )
    tester.expectTokenVal( tokSPACE, "\\", 1, 3 )
    tester.expectToken( tokSPACE, 2, 0 )
    tester.expectToken( "example", 2, 1 )
    tester.expectToken( tokLN, 2, 8 )

    return True


#####################################################################################
#####################################################################################
#####                                                                           #####
#####           C P P   E X P R E S S I O N S                                   #####
#####                                                                           #####
#####################################################################################
#####################################################################################

class CppExpr:
    """a class that models the condition of #if directives into
        an expression tree. each node in the tree is of the form (op,arg) or (op,arg1,arg2)
        where "op" is a string describing the operation"""

    unaries  = [ "!", "~" ]
    binaries = [ "+", "-", "<", "<=", ">=", ">", "&&", "||", "*", "/", "%", "&", "|", "^", "<<", ">>", "==", "!=", "?", ":" ]
    precedences = {
        "?": 1, ":": 1,
        "||": 2,
        "&&": 3,
        "|": 4,
        "^": 5,
        "&": 6,
        "==": 7, "!=": 7,
        "<": 8, "<=": 8, ">": 8, ">=": 8,
        "<<": 9, ">>": 9,
        "+": 10, "-": 10,
        "*": 11, "/": 11, "%": 11,
        "!": 12, "~": 12
    }

    re_cpp_constant = re.compile(r"((\d|\w|_)+)")

    def __init__(self, tokens):
        """initialize a CppExpr. 'tokens' must be a CppToken list"""
        self.tok  = tokens
        self.n    = len(tokens)
        self.i    = 0
        if debugCppExpr:
            print "CppExpr: trying to parse %s" % repr(tokens)
        self.expr = self.parseExpression(0)
        if debugCppExpr:
            print "CppExpr: got " + repr(self.expr)
        if self.i != self.n:
            print 'crap at end of input (%d != %d): %s' % (self.i, self.n, repr(tokens))
            raise


    def throw(self, exception, msg):
        if self.i < self.n:
            tok = self.tok[self.i]
            print "%d:%d: %s" % (tok.lineno,tok.colno,msg)
        else:
            print "EOF: %s" % msg
        raise exception(msg)


    def skip_spaces(self):
        """skip spaces in input token list"""
        while self.i < self.n:
            t = self.tok[self.i]
            if t.id != tokSPACE and t.id != tokLN:
                break
            self.i += 1


    def expectId(self, id):
        """check that a given token id is at the current position, then skip over it"""
        self.skip_spaces()
        if self.i >= self.n or self.tok[self.i].id != id:
            self.throw(BadExpectedToken,self.i,"### expecting '%s' in expression, got '%s'" % (id, self.tok[self.i].id))
        self.i += 1


    def expectIdent(self):
        self.skip_spaces()
        if self.i >= self.n or self.tok[self.i].id != tokIDENT:
            self.throw(BadExpectedToken, self.i,"### expecting identifier in expression, got '%s'" % (id, self.tok[self.i].id))
        self.i += 1


    def is_decimal(self):
        v = self.tok[self.i].value[:]
        while len(v) > 0 and v[-1] in "ULul":
            v = v[:-1]
        for digit in v:
            if not digit.isdigit():
                return None

        self.i += 1
        return ("int", string.atoi(v))


    def is_hexadecimal(self):
        v = self.tok[self.i].value[:]
        while len(v) > 0 and v[-1] in "ULul":
            v = v[:-1]
        if len(v) > 2 and (v[0:2] == "0x" or v[0:2] == "0X"):
            for digit in v[2:]:
                if not digit in "0123456789abcdefABCDEF":
                    return None

            # for a hex expression tuple, the argument
            # is the value as an integer
            self.i += 1
            return ("hex", int(v[2:], 16))

        return None


    def is_integer(self):
        if self.tok[self.i].id != tokNUMBER:
            return None

        c = self.is_decimal()
        if c: return c

        c = self.is_hexadecimal()
        if c: return c

        return None


    def is_number(self):
        t = self.tok[self.i]
        if t.id == tokMINUS and self.i+1 < self.n:
            self.i += 1
            c = self.is_integer()
            if c:
                op, val  = c
                return (op, -val)
        if t.id == tokPLUS and self.i+1 < self.n:
            c = self.is_integer()
            if c: return c

        return self.is_integer()


    def is_defined(self):
        t = self.tok[self.i]
        if t.id != tokDEFINED:
            return None

        # we have the defined keyword, check the rest
        self.i += 1
        self.skip_spaces()
        used_parens = 0
        if self.i < self.n and self.tok[self.i].id == tokLPAREN:
            used_parens = 1
            self.i += 1
            self.skip_spaces()

        if self.i >= self.n:
            self.throw(CppConstantExpected,i,"### 'defined' must be followed  by macro name or left paren")

        t = self.tok[self.i]
        if t.id != tokIDENT:
            self.throw(CppConstantExpected,i,"### 'defined' must be followed by macro name")

        self.i += 1
        if used_parens:
            self.expectId(tokRPAREN)

        return ("defined", t.value)


    def is_call_or_ident(self):
        self.skip_spaces()
        if self.i >= self.n:
            return None

        t = self.tok[self.i]
        if t.id != tokIDENT:
            return None

        name = t.value

        self.i += 1
        self.skip_spaces()
        if self.i >= self.n or self.tok[self.i].id != tokLPAREN:
            return ("ident", name)

        params    = []
        depth     = 1
        self.i += 1
        j  = self.i
        while self.i < self.n:
            id = self.tok[self.i].id
            if id == tokLPAREN:
                depth += 1
            elif depth == 1 and (id == tokCOMMA or id == tokRPAREN):
                while j < self.i and self.tok[j].id == tokSPACE:
                    j += 1
                k = self.i
                while k > j and self.tok[k-1].id == tokSPACE:
                    k -= 1
                param = self.tok[j:k]
                params.append(param)
                if id == tokRPAREN:
                    break
                j = self.i+1
            elif id == tokRPAREN:
                depth -= 1
            self.i += 1

        if self.i >= self.n:
            return None

        self.i += 1
        return ("call", (name, params))


    # Implements the "precedence climbing" algorithm from http://www.engr.mun.ca/~theo/Misc/exp_parsing.htm.
    # The "classic" algorithm would be fine if we were using a tool to generate the parser, but we're not.
    # Dijkstra's "shunting yard" algorithm hasn't been necessary yet.
    def parseExpression(self, minPrecedence):
        self.skip_spaces()
        if self.i >= self.n:
            return None

        node = self.parsePrimary()
        while self.token() != None and self.isBinary(self.token()) and self.precedence(self.token()) >= minPrecedence:
            op = self.token()
            self.nextToken()
            rhs = self.parseExpression(self.precedence(op) + 1)
            node = (op.id, node, rhs)

        return node


    def parsePrimary(self):
        op = self.token()
        if self.isUnary(op):
            self.nextToken()
            return (op.id, self.parseExpression(self.precedence(op)))

        primary = None
        if op.id == tokLPAREN:
            self.nextToken()
            primary = self.parseExpression(0)
            self.expectId(tokRPAREN)
        elif op.id == "?":
            self.nextToken()
            primary = self.parseExpression(0)
            self.expectId(":")
        elif op.id == tokNUMBER:
            primary = self.is_number()
        elif op.id == tokIDENT:
            primary = self.is_call_or_ident()
        elif op.id == tokDEFINED:
            primary = self.is_defined()
        else:
            self.throw(BadExpectedToken, "didn't expect to see a %s in factor" % (self.tok[self.i].id))

        self.skip_spaces()

        return primary;


    def isBinary(self, token):
        return token.id in self.binaries


    def isUnary(self, token):
        return token.id in self.unaries


    def precedence(self, token):
        return self.precedences.get(token.id)


    def token(self):
        if self.i >= self.n:
            return None
        return self.tok[self.i]


    def nextToken(self):
        self.i += 1
        self.skip_spaces()
        if self.i >= self.n:
            return None
        return self.tok[self.i]


    def dump_node(self, e):
        op = e[0]
        line = "(" + op
        if op == "int":
            line += " %d)" % e[1]
        elif op == "hex":
            line += " 0x%x)" % e[1]
        elif op == "ident":
            line += " %s)" % e[1]
        elif op == "defined":
            line += " %s)" % e[1]
        elif op == "call":
            arg = e[1]
            line += " %s [" % arg[0]
            prefix = ""
            for param in arg[1]:
                par = ""
                for tok in param:
                    par += str(tok)
                line += "%s%s" % (prefix, par)
                prefix = ","
            line += "])"
        elif op in CppExpr.unaries:
            line += " %s)" % self.dump_node(e[1])
        elif op in CppExpr.binaries:
            line += " %s %s)" % (self.dump_node(e[1]), self.dump_node(e[2]))
        else:
            line += " ?%s)" % repr(e[1])

        return line

    def __repr__(self):
        return self.dump_node(self.expr)

    def source_node(self, e):
        op = e[0]
        if op == "int":
            return "%d" % e[1]
        if op == "hex":
            return "0x%x" % e[1]
        if op == "ident":
            # XXX: should try to expand
            return e[1]
        if op == "defined":
            return "defined(%s)" % e[1]

        prec = CppExpr.precedences.get(op,1000)
        arg  = e[1]
        if op in CppExpr.unaries:
            arg_src = self.source_node(arg)
            arg_op  = arg[0]
            arg_prec = CppExpr.precedences.get(arg[0],1000)
            if arg_prec < prec:
                return "!(" + arg_src + ")"
            else:
                return "!" + arg_src
        if op in CppExpr.binaries:
            arg2     = e[2]
            arg1_op  = arg[0]
            arg2_op  = arg2[0]
            arg1_src = self.source_node(arg)
            arg2_src = self.source_node(arg2)
            if CppExpr.precedences.get(arg1_op,1000) < prec:
                arg1_src = "(%s)" % arg1_src
            if CppExpr.precedences.get(arg2_op,1000) < prec:
                arg2_src = "(%s)" % arg2_src

            return "%s %s %s" % (arg1_src, op, arg2_src)
        return "???"

    def __str__(self):
        return self.source_node(self.expr)

    def int_node(self,e):
        if e[0] == "int":
            return e[1]
        elif e[1] == "hex":
            return int(e[1],16)
        else:
            return None

    def toInt(self):
        return self.int_node(self.expr)

    def optimize_node(self, e, macros={}):
        op = e[0]
        if op == "defined":
            op, name = e
            if macros.has_key(name):
                if macros[name] == kCppUndefinedMacro:
                    return ("int", 0)
                else:
                    try:
                        value = int(macros[name])
                        return ("int", value)
                    except:
                        return ("defined", macros[name])

            if kernel_remove_config_macros and name.startswith("CONFIG_"):
                return ("int", 0)

            return e

        elif op == "ident":
            op, name = e
            if macros.has_key(name):
                try:
                    value = int(macros[name])
                    expanded = ("int", value)
                except:
                    expanded = ("ident", macros[name])
                return self.optimize_node(expanded, macros)
            return e

        elif op == "!":
            op, v = e
            v = self.optimize_node(v, macros)
            if v[0] == "int":
                if v[1] == 0:
                    return ("int", 1)
                else:
                    return ("int", 0)
            return ('!', v)

        elif op == "&&":
            op, l, r = e
            l  = self.optimize_node(l, macros)
            r  = self.optimize_node(r, macros)
            li = self.int_node(l)
            ri = self.int_node(r)
            if li != None:
                if li == 0:
                    return ("int", 0)
                else:
                    return r
            elif ri != None:
                if ri == 0:
                    return ("int", 0)
                else:
                    return l
            return (op, l, r)

        elif op == "||":
            op, l, r = e
            l  = self.optimize_node(l, macros)
            r  = self.optimize_node(r, macros)
            li = self.int_node(l)
            ri = self.int_node(r)
            if li != None:
                if li == 0:
                    return r
                else:
                    return ("int", 1)
            elif ri != None:
                if ri == 0:
                    return l
                else:
                    return ("int", 1)
            return (op, l, r)

        else:
            return e

    def optimize(self,macros={}):
        self.expr = self.optimize_node(self.expr, macros)

    def is_equal_node(self,e1,e2):
        if e1[0] != e2[0] or len(e1) != len(e2):
            return False

        op = e1[0]
        if op == "int" or op == "hex" or op == "!" or op == "defined":
            return e1[0] == e2[0]

        return self.is_equal_node(e1[1],e2[1]) and self.is_equal_node(e1[2],e2[2])

    def is_equal(self,other):
        return self.is_equal_node(self.expr,other.expr)

def test_cpp_expr(expr, expected):
    e = CppExpr( CppLineTokenizer( expr ).toTokenList() )
    s1 = repr(e)
    if s1 != expected:
        print "[FAIL]: expression '%s' generates '%s', should be '%s'" % (expr, s1, expected)
        global failure_count
        failure_count += 1

def test_cpp_expr_optim(expr, expected, macros={}):
    e = CppExpr( CppLineTokenizer( expr ).toTokenList() )
    e.optimize(macros)
    s1 = repr(e)
    if s1 != expected:
        print "[FAIL]: optimized expression '%s' generates '%s' with macros %s, should be '%s'" % (expr, s1, macros, expected)
        global failure_count
        failure_count += 1

def test_cpp_expr_source(expr, expected):
    e = CppExpr( CppLineTokenizer( expr ).toTokenList() )
    s1 = str(e)
    if s1 != expected:
        print "[FAIL]: source expression '%s' generates '%s', should be '%s'" % (expr, s1, expected)
        global failure_count
        failure_count += 1

def test_CppExpr():
    test_cpp_expr("0", "(int 0)")
    test_cpp_expr("1", "(int 1)")
    test_cpp_expr("(0)", "(int 0)")
    test_cpp_expr("1 && 1", "(&& (int 1) (int 1))")
    test_cpp_expr("1 && 0", "(&& (int 1) (int 0))")
    test_cpp_expr("EXAMPLE", "(ident EXAMPLE)")
    test_cpp_expr("EXAMPLE - 3", "(- (ident EXAMPLE) (int 3))")
    test_cpp_expr("defined(EXAMPLE)", "(defined EXAMPLE)")
    test_cpp_expr("defined ( EXAMPLE ) ", "(defined EXAMPLE)")
    test_cpp_expr("!defined(EXAMPLE)", "(! (defined EXAMPLE))")
    test_cpp_expr("defined(ABC) || defined(BINGO)", "(|| (defined ABC) (defined BINGO))")
    test_cpp_expr("FOO(BAR)", "(call FOO [BAR])")
    test_cpp_expr("A == 1 || defined(B)", "(|| (== (ident A) (int 1)) (defined B))")

    test_cpp_expr_optim("0", "(int 0)")
    test_cpp_expr_optim("1", "(int 1)")
    test_cpp_expr_optim("1 && 1", "(int 1)")
    test_cpp_expr_optim("1 && 0", "(int 0)")
    test_cpp_expr_optim("0 && 1", "(int 0)")
    test_cpp_expr_optim("0 && 0", "(int 0)")
    test_cpp_expr_optim("1 || 1", "(int 1)")
    test_cpp_expr_optim("1 || 0", "(int 1)")
    test_cpp_expr_optim("0 || 1", "(int 1)")
    test_cpp_expr_optim("0 || 0", "(int 0)")
    test_cpp_expr_optim("A", "(ident A)")
    test_cpp_expr_optim("A", "(int 1)", { "A": 1 })
    test_cpp_expr_optim("A || B", "(int 1)", { "A": 1 })
    test_cpp_expr_optim("A || B", "(int 1)", { "B": 1 })
    test_cpp_expr_optim("A && B", "(ident B)", { "A": 1 })
    test_cpp_expr_optim("A && B", "(ident A)", { "B": 1 })
    test_cpp_expr_optim("A && B", "(&& (ident A) (ident B))")
    test_cpp_expr_optim("EXAMPLE", "(ident EXAMPLE)")
    test_cpp_expr_optim("EXAMPLE - 3", "(- (ident EXAMPLE) (int 3))")
    test_cpp_expr_optim("defined(EXAMPLE)", "(defined EXAMPLE)")
    test_cpp_expr_optim("defined(EXAMPLE)", "(defined XOWOE)", { "EXAMPLE": "XOWOE" })
    test_cpp_expr_optim("defined(EXAMPLE)", "(int 0)", { "EXAMPLE": kCppUndefinedMacro})
    test_cpp_expr_optim("!defined(EXAMPLE)", "(! (defined EXAMPLE))")
    test_cpp_expr_optim("!defined(EXAMPLE)", "(! (defined XOWOE))", { "EXAMPLE" : "XOWOE" })
    test_cpp_expr_optim("!defined(EXAMPLE)", "(int 1)", { "EXAMPLE" : kCppUndefinedMacro })
    test_cpp_expr_optim("defined(A) || defined(B)", "(|| (defined A) (defined B))")
    test_cpp_expr_optim("defined(A) || defined(B)", "(int 1)", { "A" : "1" })
    test_cpp_expr_optim("defined(A) || defined(B)", "(int 1)", { "B" : "1" })
    test_cpp_expr_optim("defined(A) || defined(B)", "(defined A)", { "B" : kCppUndefinedMacro })
    test_cpp_expr_optim("defined(A) || defined(B)", "(int 0)", { "A" : kCppUndefinedMacro, "B" : kCppUndefinedMacro })
    test_cpp_expr_optim("defined(A) && defined(B)", "(&& (defined A) (defined B))")
    test_cpp_expr_optim("defined(A) && defined(B)", "(defined B)", { "A" : "1" })
    test_cpp_expr_optim("defined(A) && defined(B)", "(defined A)", { "B" : "1" })
    test_cpp_expr_optim("defined(A) && defined(B)", "(int 0)", { "B" : kCppUndefinedMacro })
    test_cpp_expr_optim("defined(A) && defined(B)", "(int 0)", { "A" : kCppUndefinedMacro })
    test_cpp_expr_optim("A == 1 || defined(B)", "(|| (== (ident A) (int 1)) (defined B))" )
    test_cpp_expr_optim("defined(__KERNEL__) || !defined(__GLIBC__) || (__GLIBC__ < 2)", "(|| (! (defined __GLIBC__)) (< (ident __GLIBC__) (int 2)))", { "__KERNEL__": kCppUndefinedMacro })

    test_cpp_expr_source("0", "0")
    test_cpp_expr_source("1", "1")
    test_cpp_expr_source("1 && 1", "1 && 1")
    test_cpp_expr_source("1 && 0", "1 && 0")
    test_cpp_expr_source("0 && 1", "0 && 1")
    test_cpp_expr_source("0 && 0", "0 && 0")
    test_cpp_expr_source("1 || 1", "1 || 1")
    test_cpp_expr_source("1 || 0", "1 || 0")
    test_cpp_expr_source("0 || 1", "0 || 1")
    test_cpp_expr_source("0 || 0", "0 || 0")
    test_cpp_expr_source("EXAMPLE", "EXAMPLE")
    test_cpp_expr_source("EXAMPLE - 3", "EXAMPLE - 3")
    test_cpp_expr_source("defined(EXAMPLE)", "defined(EXAMPLE)")
    test_cpp_expr_source("defined EXAMPLE", "defined(EXAMPLE)")
    test_cpp_expr_source("A == 1 || defined(B)", "A == 1 || defined(B)")


#####################################################################################
#####################################################################################
#####                                                                           #####
#####          C P P   B L O C K                                                #####
#####                                                                           #####
#####################################################################################
#####################################################################################

class Block:
    """a class used to model a block of input source text. there are two block types:
        - directive blocks: contain the tokens of a single pre-processor directive (e.g. #if)
        - text blocks, contain the tokens of non-directive blocks

       the cpp parser class below will transform an input source file into a list of Block
       objects (grouped in a BlockList object for convenience)"""

    def __init__(self,tokens,directive=None,lineno=0):
        """initialize a new block, if 'directive' is None, this is a text block
           NOTE: this automatically converts '#ifdef MACRO' into '#if defined(MACRO)'
                 and '#ifndef MACRO' into '#if !defined(MACRO)'"""
        if directive == "ifdef":
            tok = Token()
            tok.set(tokDEFINED)
            tokens = [ tok ] + tokens
            directive = "if"

        elif directive == "ifndef":
            tok1 = Token()
            tok2 = Token()
            tok1.set(tokNOT)
            tok2.set(tokDEFINED)
            tokens = [ tok1, tok2 ] + tokens
            directive = "if"

        self.tokens    = tokens
        self.directive = directive
        if lineno > 0:
            self.lineno = lineno
        else:
            self.lineno = self.tokens[0].lineno

        if self.isIf():
            self.expr = CppExpr( self.tokens )

    def isDirective(self):
        """returns True iff this is a directive block"""
        return self.directive != None

    def isConditional(self):
        """returns True iff this is a conditional directive block"""
        return self.directive in ["if","ifdef","ifndef","else","elif","endif"]

    def isDefine(self):
        """returns the macro name in a #define directive, or None otherwise"""
        if self.directive != "define":
            return None

        return self.tokens[0].value

    def isIf(self):
        """returns True iff this is an #if-like directive block"""
        return self.directive in ["if","ifdef","ifndef","elif"]

    def isInclude(self):
        """checks whether this is a #include directive. if true, then returns the
           corresponding file name (with brackets or double-qoutes). None otherwise"""
        if self.directive != "include":
            return None

        if self.tokens[0].id == tokSTRING:
            # a double-quote include, that's easy
            return self.tokens[0].value

        # we only want the bracket part, not any comments or junk after it
        if self.tokens[0].id == "<":
            i   = 0
            tok = self.tokens
            n   = len(tok)
            while i < n and tok[i].id != ">":
                i += 1

            if i >= n:
                return None

            return string.join([ str(x) for x in tok[:i+1] ],"")

        else:
            return None

    def removeWhiteSpace(self):
        # Remove trailing whitespace and empty lines
        # All whitespace is also contracted to a single space
        if self.directive != None:
            return

        tokens = []
        line   = 0     # index of line start
        space  = -1    # index of first space, or -1
        ii = 0
        nn = len(self.tokens)
        while ii < nn:
            tok = self.tokens[ii]

            # If we find a space, record its position if this is the first
            # one the line start or the previous character. Don't append
            # anything to tokens array yet though.
            if tok.id == tokSPACE:
                if space < 0:
                    space = ii
                ii += 1
                continue

            # If this is a line space, ignore the spaces we found previously
            # on the line, and remove empty lines.
            if tok.id == tokLN:
                old_line  = line
                old_space = space
                ii   += 1
                line  = ii
                space = -1
                if old_space == old_line:  # line only contains spaces
                    continue
                if ii-1 == old_line:  # line is empty
                    continue
                tokens.append(tok)
                continue

            # Other token, append any space range if any, converting each
            # one to a single space character, then append the token.
            if space >= 0:
                jj = space
                space = -1
                while jj < ii:
                    tok2 = self.tokens[jj]
                    tok2.value = " "
                    tokens.append(tok2)
                    jj += 1

            tokens.append(tok)
            ii += 1

        self.tokens = tokens

    def writeWithWarning(self,out,warning,left_count,repeat_count):
        # removeWhiteSpace() will sometimes creates non-directive blocks
        # without any tokens. These come from blocks that only contained
        # empty lines and spaces. They should not be printed in the final
        # output, and then should not be counted for this operation.
        #
        if not self.directive and self.tokens == []:
            return left_count

        if self.directive:
            out.write(str(self).rstrip() + "\n")
            left_count -= 1
            if left_count == 0:
                out.write(warning)
                left_count = repeat_count

        else:
            for tok in self.tokens:
                out.write(str(tok))
                if tok.id == tokLN:
                    left_count -= 1
                    if left_count == 0:
                        out.write(warning)
                        left_count = repeat_count

        return left_count


    def __repr__(self):
        """generate the representation of a given block"""
        if self.directive:
            result = "#%s " % self.directive
            if self.isIf():
                result += repr(self.expr)
            else:
                for tok in self.tokens:
                    result += repr(tok)
        else:
            result = ""
            for tok in self.tokens:
                result += repr(tok)

        return result

    def __str__(self):
        """generate the string representation of a given block"""
        if self.directive:
            if self.directive == "if":
                # small optimization to re-generate #ifdef and #ifndef
                e = self.expr.expr
                op = e[0]
                if op == "defined":
                    result = "#ifdef %s" % e[1]
                elif op == "!" and e[1][0] == "defined":
                    result = "#ifndef %s" % e[1][1]
                else:
                    result = "#if " + str(self.expr)
            else:
                result = "#%s" % self.directive
                if len(self.tokens):
                    result += " "
                for tok in self.tokens:
                    result += str(tok)
        else:
            result = ""
            for tok in self.tokens:
                result += str(tok)

        return result

class BlockList:
    """a convenience class used to hold and process a list of blocks returned by
       the cpp parser"""
    def __init__(self,blocks):
        self.blocks = blocks

    def __len__(self):
        return len(self.blocks)

    def __getitem__(self,n):
        return self.blocks[n]

    def __repr__(self):
        return repr(self.blocks)

    def __str__(self):
        result = ""
        for b in self.blocks:
            result += str(b)
            if b.isDirective():
                result = result.rstrip() + '\n'
        return result

    def  optimizeIf01(self):
        """remove the code between #if 0 .. #endif in a BlockList"""
        self.blocks = optimize_if01(self.blocks)

    def optimizeMacros(self, macros):
        """remove known defined and undefined macros from a BlockList"""
        for b in self.blocks:
            if b.isIf():
                b.expr.optimize(macros)

    def removeMacroDefines(self,macros):
        """remove known macro definitions from a BlockList"""
        self.blocks = remove_macro_defines(self.blocks,macros)

    def removeWhiteSpace(self):
        for b in self.blocks:
            b.removeWhiteSpace()

    def optimizeAll(self,macros):
        self.optimizeMacros(macros)
        self.optimizeIf01()
        return

    def findIncludes(self):
        """return the list of included files in a BlockList"""
        result = []
        for b in self.blocks:
            i = b.isInclude()
            if i:
                result.append(i)

        return result


    def write(self,out):
        out.write(str(self))

    def writeWithWarning(self,out,warning,repeat_count):
        left_count = repeat_count
        for b in self.blocks:
            left_count = b.writeWithWarning(out,warning,left_count,repeat_count)

    def removeComments(self):
        for b in self.blocks:
            for tok in b.tokens:
                if tok.id == tokSPACE:
                    tok.value = " "

    def removeVarsAndFuncs(self,knownStatics=set()):
        """remove all extern and static declarations corresponding
           to variable and function declarations. we only accept typedefs
           and enum/structs/union declarations.

           however, we keep the definitions corresponding to the set
           of known static inline functions in the set 'knownStatics',
           which is useful for optimized byteorder swap functions and
           stuff like that.
           """
        # state = 0 => normal (i.e. LN + spaces)
        # state = 1 => typedef/struct encountered, ends with ";"
        # state = 2 => var declaration encountered, ends with ";"
        # state = 3 => func declaration encountered, ends with "}"
        state      = 0
        depth      = 0
        blocks2    = []
        skipTokens = False
        for b in self.blocks:
            if b.isDirective():
                blocks2.append(b)
            else:
                n     = len(b.tokens)
                i     = 0
                if skipTokens:
                    first = n
                else:
                    first = 0
                while i < n:
                    tok = b.tokens[i]
                    tokid = tok.id
                    # If we are not looking for the start of a new
                    # type/var/func, then skip over tokens until
                    # we find our terminator, managing the depth of
                    # accolades as we go.
                    if state > 0:
                        terminator = False
                        if tokid == '{':
                            depth += 1
                        elif tokid == '}':
                            if depth > 0:
                                depth -= 1
                            if (depth == 0) and (state == 3):
                                terminator = True
                        elif tokid == ';' and depth == 0:
                            terminator = True

                        if terminator:
                            # we found the terminator
                            state = 0
                            if skipTokens:
                                skipTokens = False
                                first = i+1

                        i = i+1
                        continue

                    # We are looking for the start of a new type/func/var
                    # ignore whitespace
                    if tokid in [tokLN, tokSPACE]:
                        i = i+1
                        continue

                    # Is it a new type definition, then start recording it
                    if tok.value in [ 'struct', 'typedef', 'enum', 'union', '__extension__' ]:
                        state = 1
                        i     = i+1
                        continue

                    # Is it a variable or function definition. If so, first
                    # try to determine which type it is, and also extract
                    # its name.
                    #
                    # We're going to parse the next tokens of the same block
                    # until we find a semi-column or a left parenthesis.
                    #
                    # The semi-column corresponds to a variable definition,
                    # the left-parenthesis to a function definition.
                    #
                    # We also assume that the var/func name is the last
                    # identifier before the terminator.
                    #
                    j = i+1
                    ident = ""
                    while j < n:
                        tokid = b.tokens[j].id
                        if tokid == '(':  # a function declaration
                            state = 3
                            break
                        elif tokid == ';': # a variable declaration
                            state = 2
                            break
                        if tokid == tokIDENT:
                            ident = b.tokens[j].value
                        j += 1

                    if j >= n:
                        # This can only happen when the declaration
                        # does not end on the current block (e.g. with
                        # a directive mixed inside it.
                        #
                        # We will treat it as malformed because
                        # it's very hard to recover from this case
                        # without making our parser much more
                        # complex.
                        #
                        #print "### skip unterminated static '%s'" % ident
                        break

                    if ident in knownStatics:
                        #print "### keep var/func '%s': %s" % (ident,repr(b.tokens[i:j]))
                        pass
                    else:
                        # We're going to skip the tokens for this declaration
                        #print "### skip variable /func'%s': %s" % (ident,repr(b.tokens[i:j]))
                        if i > first:
                            blocks2.append( Block(b.tokens[first:i]))
                        skipTokens = True
                        first      = n

                    i = i+1

                if i > first:
                    #print "### final '%s'" % repr(b.tokens[first:i])
                    blocks2.append( Block(b.tokens[first:i]) )

        self.blocks = blocks2

    def insertDisclaimer(self,disclaimer="/* auto-generated file, DO NOT EDIT */"):
        """insert your standard issue disclaimer that this is an
           auto-generated file, etc.."""
        tokens = CppLineTokenizer( disclaimer ).toTokenList()
        tokens = tokens[:-1]  # remove trailing tokLN
        self.blocks = [ Block(tokens) ] + self.blocks

    def replaceTokens(self,replacements):
        """replace tokens according to the given dict"""
        for b in self.blocks:
            made_change = False
            if b.isInclude() == None:
                for tok in b.tokens:
                    if tok.id == tokIDENT:
                        if tok.value in replacements:
                            tok.value = replacements[tok.value]
                            made_change = True

            if made_change and b.isIf():
                # Keep 'expr' in sync with 'tokens'.
                b.expr = CppExpr(b.tokens)

class BlockParser:
    """a class used to convert an input source file into a BlockList object"""

    def __init__(self,tokzer=None):
        """initialize a block parser. the input source is provided through a Tokenizer
           object"""
        self.reset(tokzer)

    def reset(self,tokzer):
        self.state  = 1
        self.tokzer = tokzer

    def getBlocks(self,tokzer=None):
        """tokenize and parse the input source, return a BlockList object
           NOTE: empty and line-numbering directives are ignored and removed
                 from the result. as a consequence, it is possible to have
                 two successive text blocks in the result"""
        # state 0 => in source code
        # state 1 => in source code, after a LN
        # state 2 => in source code, after LN then some space
        state   = 1
        lastLN  = 0
        current = []
        blocks  = []

        if tokzer == None:
            tokzer = self.tokzer

        while 1:
            tok = tokzer.getToken()
            if tok.id == tokEOF:
                break

            if tok.id == tokLN:
                state    = 1
                current.append(tok)
                lastLN   = len(current)

            elif tok.id == tokSPACE:
                if state == 1:
                    state = 2
                current.append(tok)

            elif tok.id == "#":
                if state > 0:
                    # this is the start of a directive

                    if lastLN > 0:
                        # record previous tokens as text block
                        block   = Block(current[:lastLN])
                        blocks.append(block)
                        lastLN  = 0

                    current = []

                    # skip spaces after the #
                    while 1:
                        tok = tokzer.getToken()
                        if tok.id != tokSPACE:
                            break

                    if tok.id != tokIDENT:
                        # empty or line-numbering, ignore it
                        if tok.id != tokLN and tok.id != tokEOF:
                            while 1:
                                tok = tokzer.getToken()
                                if tok.id == tokLN or tok.id == tokEOF:
                                    break
                        continue

                    directive = tok.value
                    lineno    = tok.lineno

                    # skip spaces
                    tok = tokzer.getToken()
                    while tok.id == tokSPACE:
                        tok = tokzer.getToken()

                    # then record tokens until LN
                    dirtokens = []
                    while tok.id != tokLN and tok.id != tokEOF:
                        dirtokens.append(tok)
                        tok = tokzer.getToken()

                    block = Block(dirtokens,directive,lineno)
                    blocks.append(block)
                    state   = 1

            else:
                state = 0
                current.append(tok)

        if len(current) > 0:
            block = Block(current)
            blocks.append(block)

        return BlockList(blocks)

    def parse(self,tokzer):
        return self.getBlocks( tokzer )

    def parseLines(self,lines):
        """parse a list of text lines into a BlockList object"""
        return self.getBlocks( CppLinesTokenizer(lines) )

    def parseFile(self,path):
        """parse a file into a BlockList object"""
        file = open(path, "rt")
        result = self.getBlocks( CppFileTokenizer(file) )
        file.close()
        return result


def test_block_parsing(lines,expected):
    blocks = BlockParser().parse( CppLinesTokenizer(lines) )
    if len(blocks) != len(expected):
        raise BadExpectedToken, "parser.buildBlocks returned '%s' expecting '%s'" \
              % (str(blocks), repr(expected))
    for n in range(len(blocks)):
        if str(blocks[n]) != expected[n]:
            raise BadExpectedToken, "parser.buildBlocks()[%d] is '%s', expecting '%s'" \
                  % (n, str(blocks[n]), expected[n])
    #for block in blocks:
    #    print block

def test_BlockParser():
    test_block_parsing(["#error hello"],["#error hello"])
    test_block_parsing([ "foo", "", "bar" ], [ "foo\n\nbar\n" ])
    test_block_parsing([ "foo", "  #  ", "bar" ], [ "foo\n","bar\n" ])
    test_block_parsing(\
        [ "foo", "   #  ", "  #  /* ahah */ if defined(__KERNEL__) ", "bar", "#endif" ],
        [ "foo\n", "#ifdef __KERNEL__", "bar\n", "#endif" ] )


#####################################################################################
#####################################################################################
#####                                                                           #####
#####        B L O C K   L I S T   O P T I M I Z A T I O N                      #####
#####                                                                           #####
#####################################################################################
#####################################################################################

def  remove_macro_defines( blocks, excludedMacros=set() ):
    """remove macro definitions like #define <macroName>  ...."""
    result = []
    for b in blocks:
        macroName = b.isDefine()
        if macroName == None or not macroName in excludedMacros:
            result.append(b)

    return result

def  find_matching_endif( blocks, i ):
    n     = len(blocks)
    depth = 1
    while i < n:
        if blocks[i].isDirective():
            dir = blocks[i].directive
            if dir in [ "if", "ifndef", "ifdef" ]:
                depth += 1
            elif depth == 1 and dir in [ "else", "elif" ]:
                return i
            elif dir == "endif":
                depth -= 1
                if depth == 0:
                    return i
        i += 1
    return i

def  optimize_if01( blocks ):
    """remove the code between #if 0 .. #endif in a list of CppBlocks"""
    i = 0
    n = len(blocks)
    result = []
    while i < n:
        j = i
        while j < n and not blocks[j].isIf():
            j += 1
        if j > i:
            D2("appending lines %d to %d" % (blocks[i].lineno, blocks[j-1].lineno))
            result += blocks[i:j]
        if j >= n:
            break
        expr = blocks[j].expr
        r    = expr.toInt()
        if r == None:
            result.append(blocks[j])
            i = j + 1
            continue

        if r == 0:
            # if 0 => skip everything until the corresponding #endif
            j = find_matching_endif( blocks, j+1 )
            if j >= n:
                # unterminated #if 0, finish here
                break
            dir = blocks[j].directive
            if dir == "endif":
                D2("remove 'if 0' .. 'endif' (lines %d to %d)" % (blocks[i].lineno, blocks[j].lineno))
                i = j + 1
            elif dir == "else":
                # convert 'else' into 'if 1'
                D2("convert 'if 0' .. 'else' into 'if 1' (lines %d to %d)" % (blocks[i].lineno, blocks[j-1].lineno))
                blocks[j].directive = "if"
                blocks[j].expr      = CppExpr( CppLineTokenizer("1").toTokenList() )
                i = j
            elif dir == "elif":
                # convert 'elif' into 'if'
                D2("convert 'if 0' .. 'elif' into 'if'")
                blocks[j].directive = "if"
                i = j
            continue

        # if 1 => find corresponding endif and remove/transform them
        k = find_matching_endif( blocks, j+1 )
        if k >= n:
            # unterminated #if 1, finish here
            D2("unterminated 'if 1'")
            result += blocks[j+1:k]
            break

        dir = blocks[k].directive
        if dir == "endif":
            D2("convert 'if 1' .. 'endif' (lines %d to %d)"  % (blocks[j].lineno, blocks[k].lineno))
            result += optimize_if01(blocks[j+1:k])
            i       = k+1
        elif dir == "else":
            # convert 'else' into 'if 0'
            D2("convert 'if 1' .. 'else' (lines %d to %d)"  % (blocks[j].lineno, blocks[k].lineno))
            result += optimize_if01(blocks[j+1:k])
            blocks[k].directive = "if"
            blocks[k].expr      = CppExpr( CppLineTokenizer("0").toTokenList() )
            i = k
        elif dir == "elif":
            # convert 'elif' into 'if 0'
            D2("convert 'if 1' .. 'elif' (lines %d to %d)" % (blocks[j].lineno, blocks[k].lineno))
            result += optimize_if01(blocks[j+1:k])
            blocks[k].expr      = CppExpr( CppLineTokenizer("0").toTokenList() )
            i = k
    return result

def  test_optimizeAll():
    text = """\
#if 1
#define  GOOD_1
#endif
#if 0
#define  BAD_2
#define  BAD_3
#endif

#if 1
#define  GOOD_2
#else
#define  BAD_4
#endif

#if 0
#define  BAD_5
#else
#define  GOOD_3
#endif

#if defined(__KERNEL__)
#define BAD_KERNEL
#endif

#if defined(__KERNEL__) || !defined(__GLIBC__) || (__GLIBC__ < 2)
#define X
#endif

#ifndef SIGRTMAX
#define SIGRTMAX 123
#endif /* SIGRTMAX */

#if 0
#if 1
#define  BAD_6
#endif
#endif\
"""

    expected = """\
#define GOOD_1

#define GOOD_2

#define GOOD_3


#if !defined(__GLIBC__) || __GLIBC__ < 2
#define X
#endif

#ifndef __SIGRTMAX
#define __SIGRTMAX 123
#endif

"""

    out = StringOutput()
    lines = string.split(text, '\n')
    list = BlockParser().parse( CppLinesTokenizer(lines) )
    #D_setlevel(2)
    list.replaceTokens( kernel_token_replacements )
    list.optimizeAll( {"__KERNEL__":kCppUndefinedMacro} )
    list.write(out)
    if out.get() != expected:
        print "[FAIL]: macro optimization failed\n"
        print "<<<< expecting '",
        print expected,
        print "'\n>>>> result '"
        print out.get(),
        print "'\n----"
        global failure_count
        failure_count += 1


# -- Always run the unit tests.

def runUnitTests():
    """run all unit tests for this program"""
    test_CppTokenizer()
    test_CppExpr()
    test_optimizeAll()
    test_BlockParser()

failure_count = 0
runUnitTests()
if failure_count != 0:
    sys.exit(1)
