# -*- coding: utf-8 -*-
'''
@author zozzz
'''

__all__ = ["ActionTable"]

from Lexer import *
from Action import *
import os, sys
import os.path

class ActionTable:

    INDENT = "\t"
    EOL = "\n"


    def __init__(self, namespace, module, outFile, force=False):
        self.namespace = namespace
        self.lexer = Lexer(module)
        self.outFile = outFile
        self.moduleFile = module.__file__
        self._force = force
        self.actionTable = {}
        self._tokenChars = {}
        self._closedTokens = [self.lexer.default.name]

        #self.actionTable[self.lexer.default.name] = range(Lexer.ASCII_MIN, Lexer.ASCII_MAX)

    def create(self):
        moduleFInf = os.stat(self.moduleFile)

        try:
            outFInf = os.stat(self.outFile+".h")
        except:
            self._create()
            return

        if self._force is True or moduleFInf.st_mtime > outFInf.st_mtime:
            self._create()
        else:
            print "Module not changed: " + self.lexer._module.__name__

    def _create(self):
        print "Generate " + self.outFile + ".h ...",
        #try:
        f = open(self.outFile+".h", "w")
        f.write("// AUTO GENERATED CODE, DO NOT EDIT!!!\n\n");

        f.write("#ifndef _" + os.path.basename(self.outFile).upper() + "_" + self.EOL)
        f.write("#define _" + os.path.basename(self.outFile).upper() + "_" + self.EOL + self.EOL)

        ns = self.namespace.split(".")

        for x in ns:
            f.write("namespace " + x + " { ")
        f.write(self.EOL + self.EOL)

        f.write(self._createEnums(1) + self.EOL + self.EOL)
        f.write(self._createActionTable(1) + self.EOL)

        f.write(self.EOL + self.EOL)

        for x in ns:
            f.write("} /* "+ x +" */" + self.EOL)

        f.write(self.EOL)

        f.write("#endif")
        f.close()
        #except:
            #print "[ERROR]", sys.exc_info()[1]
            #return
        print "[OK]"


    # ==========================================================================
    # GENERATOR FUNCTIONS
    # ==========================================================================
    def _createEnums(self, indent):
        ret = [
            (indent * self.INDENT) + "enum TokenType",
            (indent * self.INDENT) + "{"
        ]

        _group = None
        _ml = len(self.lexer._tokensByGroup)
        i=0
        for (g, n) in self.lexer._tokensByGroup:
            if _group != g:
                _group = g
                (r_min, r_max) = self.lexer.tokenRange(g)
                _comment = ""
                if i != 0:
                    _comment = self.EOL
                _comment += ((indent+1) * self.INDENT) + "// " + g + " ("+str(r_min)+" - "+str(r_max)+")"
                ret.append(_comment)

            _append = ((indent+1) * self.INDENT) + n
            if i == 0:
                _append += " = 0"
            _append += ","
            ret.append(_append)
            i += 1

        ret.append( self.EOL + ((indent+1) * self.INDENT) + "TT_COUNT")

        ret.append((indent * self.INDENT) + "};")

        return self.EOL.join(ret)

    def _createActionTable(self, indent):
        self._fillActionTable()

        ret = [
            (indent * self.INDENT) + "static unsigned int actionTable["+str(len(self.lexer._tokensByGroup))+"]["+str(Lexer.ASCII_MAX)+"] = {"
        ]

        tokNameMaxWidth = 0
        for tn in self.lexer._tokens.keys():
            tokNameMaxWidth = max(tokNameMaxWidth, len(tn))

        length = len(self.lexer._tokensByGroup)

        pos = 0
        for (group, token) in self.lexer._tokensByGroup:
            row = ((indent+1) * self.INDENT) + "/* "+ token.ljust(tokNameMaxWidth, " ") +" */ {"

            for (ch, action) in enumerate(self.actionTable[token]):
                if action != 0:
                    row += action.asHex()
                else:
                    row += ActionError().asHex()

                if ch != Lexer.ASCII_MAX-1:
                    row += ","

            row += "}"

            if length-1 != pos:
                row += ","

            pos += 1

            ret.append(row)

        ret.append((indent * self.INDENT) + "};")

        return self.EOL.join(ret)


    def _fillActionTable(self):

        for token in self.lexer._ordered:
            self.actionTable[token.name] = []
            for _x in range(Lexer.ASCII_MIN, Lexer.ASCII_MAX):
                self.actionTable[token.name].append(0)


        for token in self.lexer._ordered[1:]:
            if token.infinity:

                pos = 0
                for pgroup in ["begin", "middle", "end"]:
                    if token[pgroup] is None:
                        continue

                    if pgroup == "middle":
                        _max = token[pgroup].minWidth()
                    else:
                        _max = token[pgroup].maxWidth()

                    if _max == 0:
                        for char in range(Lexer.ASCII_MIN, Lexer.ASCII_MAX):
                            (_tmatch, _mlength) = token.test([[char]], pgroup, Token.RESULT_EXTEND)
                            if _tmatch and _mlength == 1:
                                if self.actionTable[token.name][char] == 0:
                                    self.actionTable[token.name][char] = ActionContinue()
                                    self._addAvailChar(token, char, pos+1)
                    else:
                        for sub_pos in range(0, _max):
                            _addChars = []

                            for char in range(Lexer.ASCII_MIN, Lexer.ASCII_MAX):
                                if pos == 0:
                                    if token.test([[char]]) \
                                       and self.actionTable[self.lexer.default.name][char] == 0:
                                        self.actionTable[self.lexer.default.name][char] = ActionChangeTokenType(token)
                                        _addChars.append((token, pos, char))

                                else:
                                    avail = self._findAvailTokens(char, None, token, None, pos)
                                    if len(avail):
                                        print token.name, "["+chr(char)+":"+str(pos)+"]", "->", avail, self._tokenChars[token.name]
                                        if avail[0] == token.name:
                                            _addChars.append((token, pos, char))
                                        else:
                                            _alt = self.lexer._tokens[avail[0]]

                                            _addChars.append((_alt, pos, char))

                        for (_cha_token, _cha_pos, _cha_ch) in _addChars:
                            self._addAvailChar(_cha_token, _cha_ch, _cha_pos)

                        pos += 1


            else:
                _max = token.maxWidth()

                for pos in range(0, _max):

                    _addChars = []

                    for char in range(Lexer.ASCII_MIN, Lexer.ASCII_MAX):

                        if pos == 0:
                            if token.test([[char]]):
                                _alts = []
                                self._findAvailTokens(char, _alts)
                                if _max == 1 and token.exact and len(_alts) == 1:
                                    self.actionTable[self.lexer.default.name][char] = ActionClose(0, token)
                                    self._closedTokens.append(token.name)
                                    break
                                elif self.actionTable[self.lexer.default.name][char] == 0:
                                    self.actionTable[self.lexer.default.name][char] = ActionChangeTokenType(token)

                                for _alt in _alts:
                                    self._addAvailChar(self.lexer._tokens[_alt], char, pos)

                            continue

                        else:
                            avail = self._findAvailTokens(char, token=token, length=pos)

                            if len(avail):
                                #print token.name, "["+chr(char)+":"+str(pos)+"]", "->", avail, self._tokenChars[token.name]
                                if avail[0] == token.name:
                                    #print token.name, "["+chr(char)+":"+str(pos)+"]", "->", avail, self._tokenChars[token.name]

                                    if self.actionTable[token.name][char] == 0:
                                        self.actionTable[token.name][char] = ActionCharAt(token, pos)

                                    elif isinstance(self.actionTable[token.name][char], ActionCharAt)\
                                         and self.actionTable[token.name][char].tid == token.id():
                                        self.actionTable[token.name][char].addPosition(pos)

                                    else:
                                        self.actionTable[token.name][char] = ActionCharAt(token, pos)

                                    _addChars.append((token, pos, char))

                                else:
                                    _alt = self.lexer._tokens[avail[0]]
                                    _addChars.append((_alt, pos, char))

                                    if self.actionTable[token.name][char] == 0:

                                        if _alt.infinity:
                                            self.actionTable[token.name][char] = ActionChangeTokenType(_alt)
                                        else:
                                            self.actionTable[token.name][char] = ActionCharAt(_alt, pos)




                    #print "_postAddChars:", pos, _addChars
                    for (_cha_token, _cha_pos, _cha_ch) in _addChars:
                        self._addAvailChar(_cha_token, _cha_ch, _cha_pos)

                    if token.exact and pos == token.maxWidth():
                        self._closedTokens.append(token.name)

            """ EZ KELL CSAK RONTJA AZ ÁTLÁTHATÓSÁGOT
            for (char, action) in enumerate(self.actionTable[token.name]):
                if action != 0:
                    continue

                _alts = self._findAvailTokens(char, skipClosed=False)
                if len(_alts):
                    for _alt in _alts:
                        self.actionTable[token.name][char] = ActionClose(-1, self.lexer._tokens[_alt])
            """


        #return
        for (key, value) in self.actionTable.iteritems():
            print key,
            for action in value:
                print action,
            print ""


    def _insertTokenDefaultAction(self, token):
        pass


    """
    ch: 1 (chngType(int))
    |-> (default->int)
        |-> ch: 2 (OK)
        |   |-> int
        |-> ch: . (chngType(op))
            |-> (int->op)
                |-> ch: 2 (chngType(float))
                |   |-> (op->float)
                |-> ch: x
                |   |-> (op(close)->identifier)

    ch: c
    |-> (default->case)
        |-> ch: a (charAt(1))
        |   |-> case
        |->ch: l (chngType(identifier))
            |-> (case->identifier)
    """

    def _determineActionsFor(self, token):

        """
        infinity = token.maxWidth() >= 65535
        subGroup = None

        if infinity and token.begin.maxWidth() < 65535:
            subGroup = "begin"
            ml = token.begin.maxWidth()
        else:
            ml = token.maxWidth()


        variants = []
        for pos in range(0, ml+1):
            vSnapShot = variants[:]
            for char in range(Lexer.ASCII_MIN, Lexer.ASCII_MAX):
                #print vSnapShot
                alterTokens = []
                if pos > 0:
                    self._findAlternativeTokens(token, vSnapShot, char, alterTokens);

                if self._tokenIsMatch(token, vSnapShot, char, subGroup):
                    if len(variants) <= pos:
                        variants.append([])
                    variants[pos].append(char)


                if len(alterTokens):
                    print "alt["+token.name+":"+str(pos)+":"+chr(char)+"]: ", alterTokens

        #print token.name, variants
        """

        pos = 0
        while True:

            if pos == 0:
                for char in range(Lexer.ASCII_MIN, Lexer.ASCII_MAX):
                    avail = []
                    self._findAvailTokens(char, avail)

                    print "avail:", chr(char), avail

            break



    def _tokenIsMatch(self, tok, chars, ch, subGroup=None):
        chr = chars[:]
        if ch:
            chr.append([ch])

        (match, length) = tok.test(chr, subGroup, Token.RESULT_EXTEND)
        return len(chars) == length

    def _findAvailTokens(self, ch, result=None, token=None, groups=None, length=-1, skipClosed=True):
        def _tok(tn):
            return self.lexer._tokens[tn]

        if result is None:
            result = []

        if token is None or self._tokenChars.has_key(token.name) is False:
            chars = [[ch]]
        else:
            chars = self._tokenChars[token.name][:]

            if len(chars) == 0:
                chars = [[ch]]
            else:
                chars.append([ch])

        _chsLength = len(chars)

        if length != -1:
            if _chsLength-1 != length:
                return result

        selfMatched = False
        for altToken in self.lexer._ordered:

            if skipClosed and self._closedTokens.count(altToken.name) != 0:
                continue

            (_match, _length) = altToken.test(chars, groups, Token.RESULT_EXTEND)
            #if _match and token:
            #    print token.name, altToken.name, _chsLength, _length
            if _match and _length == _chsLength and result.count(altToken.name) == 0:
                if token is not None and altToken.name == token.name:
                    selfMatched = True
                    continue

                insertPos = 0

                for tn in result:
                    if _tok(tn).exact and altToken.exact:
                        if _tok(tn).minWidth() <= altToken.minWidth():
                            insertPos += 1
                        else:
                            break;
                    else:
                        if _tok(tn).exact or _tok(tn).minWidth() <= altToken.minWidth():
                            insertPos += 1
                        else:
                            break

                result.insert(insertPos, altToken.name)

        if selfMatched:
            result.insert(0, token.name);

        return result


    def _addAvailChar(self, tok, ch, pos):
        if self._tokenChars.has_key(tok.name) is False:
            self._tokenChars[tok.name] = []

        #print len(self._tokenChars[tok.name]), "<=", pos
        """while len(self._tokenChars[tok.name]) <= pos:
            print len(self._tokenChars[tok.name]), "<=", pos
            self._tokenChars[tok.name].append([])"""
        while True:
            if len(self._tokenChars[tok.name]) <= pos:
                self._tokenChars[tok.name].append([])
            else:
                break

        if self._tokenChars[tok.name][pos].count(ch) == 0:
            self._tokenChars[tok.name][pos].append(ch)
