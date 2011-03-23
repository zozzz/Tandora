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
        try:
            print "Generate " + self.outFile + ".h ...",

            f = open(self.outFile+".h", "w")
            f.write("// AUTO GENERATED CODE, DO NOT EDIT!!!" + self.EOL * 2);

            f.write("#ifndef _" + os.path.basename(self.outFile).upper() + "_" + self.EOL)
            f.write("#define _" + os.path.basename(self.outFile).upper() + "_" + self.EOL * 2)

            ns = self.namespace.split(".")

            for x in ns:
                f.write("namespace " + x + " { ")
            f.write(self.EOL + self.EOL)

            f.write(self._createEnums(1) + self.EOL * 2)
            f.write(self.INDENT + "extern unsigned int actionTable["+str(len(self.lexer._tokensByGroup))+"]["+str(Lexer.ASCII_MAX)+"];");

            f.write(self.EOL * 2)

            for x in ns:
                f.write("} /* "+ x +" */" + self.EOL)

            f.write(self.EOL)

            f.write("#endif")
            f.close()

            print "[OK]"
        except:
            print "[ERROR]", sys.exc_info()[1]
            return


        #try:
        print "Generate " + self.outFile + ".cpp ...",

        f = open(self.outFile+".cpp", "w")
        f.write("// AUTO GENERATED CODE, DO NOT EDIT!!!" + self.EOL * 2);

        f.write('#include "'+os.path.basename(self.outFile)+'.h"' + self.EOL * 2);

        ns = self.namespace.split(".")

        for x in ns:
            f.write("namespace " + x + " { ")
        f.write(self.EOL + self.EOL)

        f.write(self._createActionTable(1) + self.EOL * 2)

        for x in ns:
            f.write("} /* "+ x +" */" + self.EOL)

        f.close()

        print "[OK]"
        #except:
        #    print "[ERROR]", sys.exc_info()[1]
        #    return



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
            (indent * self.INDENT) + "unsigned int actionTable["+str(len(self.lexer._tokensByGroup))+"]["+str(Lexer.ASCII_MAX)+"] = {"
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

    def _initActionTable(self):
        for token in self.lexer._ordered:
            self.actionTable[token.name] = []
            for _x in range(Lexer.ASCII_MIN, Lexer.ASCII_MAX):
                self.actionTable[token.name].append(0)

    # TODO: remake
    # minden tokent egységesen kell kezelni:
    # a pos == 0 mindenhol ugyan az ellenkező esetben pedig 3 részre kell osztani az ellenőrzést

    def _fillActionTable(self):

        self._initActionTable();
        tokens = self.lexer._ordered[1:];

        for token in tokens:
            # (token:Token, pos:int, ch:int)
            _addChars = []

            if self._insertTokenActions(token):
                self._closedTokens.append(token.name);

            if token.escape:
                for esc_ch in token.escape:
                    self.actionTable[token.name][ord(esc_ch)] = ActionSeekForward(2)

            for (_ach_token_, _ach_pos_, _ach_ch_) in _addChars:
                self._addAvailChar(_ach_token_, _ach_pos_, _ach_ch_)

            if token.end is None:
                for (char, action) in enumerate(self.actionTable[token.name]):
                    if action != 0:
                        continue

                    _alts = self._findAvailTokens(char, skipClosed=False)
                    if len(_alts):
                        self.actionTable[token.name][char] = ActionClose(1, token)

            # inc line token elhelyezés a következő képpen:
            # if AT[x][x] == 0 || AT[x][x] is not ActionChangeType
            # ActionChangeType(inclineToken, True, False)

        for (key, value) in self.actionTable.iteritems():
            print key,
            for action in value:
                print action,
            print ""

        """
        each token:

            for pgroup in ["begin", "middle"]:

                if token[pgrpoup] is None:
                    continue

                if token[pgrpoup].infinity:
                    plen = token[pgrpoup].minWidth()

                plen = (token.infinity ? token.minWidth() : token.maxWidth())

                for pos in plen:

                    for char in 0...128:

                        if pos == 0:
                            megállapítani a begin alapján, hogy mivel kezdődhet és azt betolni  default-ba

                        else:

                            if token[pgroup].infinity:
                                pass
                            else:
                                ebben az esetben az adott group karakter egyezéseit vizsgálja és a sorrend fontos
                                azaz valószínűleg CharAt action lesz mind

            # (?P<begin>/\\*)(?P<middle>.*?)(?P<end>\\*/)
            # (?P<begin>//)(?P<middle>.*?)(?P<end>\r|\n)
            if token.end is not None:
                endlen = token.end.maxWidth()

                for epos in endlen:
                    for char in 0...128:

            // insertIncLineAction(token)

        """

        pass

    # @param Token
    # @param (token:Token, pos:int, ch:int)
    # @return True/False ha True akkor véglegesen lezárja a tokent
    def _insertTokenActions(self, token):
        groups = []
        if token.begin:
            groups.append("begin")
        if token.middle:
            groups.append("middle")
        if token.end:
            groups.append("end")

        defToken = self.lexer.default
        _ascii_chars_ = range(self.lexer.ASCII_MIN, self.lexer.ASCII_MAX)
        AT = self.actionTable;
        _endChars = []

        pos = 0

        for groupName in groups:
            group = token[groupName]
            glen = 0

            if group.infinity:
                glen = max(1, group.minWidth())
            else:
                glen = group.maxWidth()

            for gpos in range(0, glen):

                availChars = []
                endAvailChars = []

                for char in _ascii_chars_:

                    # ==========================================================================
                    # POSITION == 0
                    # ==========================================================================
                    if pos == 0:
                        altTokens = self._findAvailTokens(char)

                        if len(altTokens) > 0 and altTokens[0] == token.name:
                            if token.exact and token.maxWidth() == 1 and len(altTokens) == 1:
                                AT[defToken.name][char] = ActionClose(0, token)
                            else:
                                if AT[defToken.name][char] == 0:
                                    AT[defToken.name][char] = ActionChangeType(token)

                                for _alt in altTokens[1:]:
                                    AT[token.name][char] = ActionChangeType(self.lexer._tokens[_alt])

                            for _alt in altTokens:
                                availChars.append((self.lexer._tokens[_alt], pos, char))

                    # ==========================================================================
                    # END GROUP
                    # ==========================================================================
                    elif groupName == "end":
                        _chs = _endChars[:]
                        while len(_chs) <= gpos:
                            _chs.append([])

                        _chs[gpos].append(char)

                        (tr_match, tr_length) = token.test(_chs, "end", Token.RESULT_EXTEND)
                        if tr_match and tr_length-1 == gpos:
                            altTokens = self._findAvailTokens(_chs, length=gpos)
                            if len(altTokens) > 0 and altTokens[0] != token.name:
                                AT[token.name][char] = ActionChangeType(self.lexer._tokens[altTokens[0]], True, True)
                            else:
                                AT[token.name][char] = ActionSubCharAt(token, gpos, token.end.maxWidth())

                            endAvailChars.append((token, gpos, char))

                    # ==========================================================================
                    # ANY OTHER...
                    # ==========================================================================
                    else:
                        altTokens = self._findAvailTokens(char, token=token, length=pos)
                        #print token.name, "["+chr(char)+":"+str(pos)+"]", "->", altTokens, self._tokenChars[token.name]

                        if len(altTokens) > 0:
                            #if token.infinity:
                            #    print token.name, "["+chr(char)+":"+str(pos)+"]", "->", altTokens, self._tokenChars[token.name]

                            if altTokens[0] == token.name:
                                if group.infinity is False:
                                    if AT[token.name][char] == 0:
                                        AT[token.name][char] = ActionCharAt(token, pos)
                                    elif isinstance(AT[token.name][char], ActionCharAt) \
                                        and AT[token.name][char].tid == token.id():
                                        AT[token.name][char].addPosition(pos)
                                else:
                                    if AT[token.name][char] == 0:
                                        AT[token.name][char] = ActionContinue()
                            else:
                                for _alt in altTokens:
                                    if AT[token.name][char] == 0:
                                        AT[token.name][char] = ActionChangeType(self.lexer._tokens[_alt])

                            for _alt in altTokens:
                                availChars.append((self.lexer._tokens[_alt], pos, char))


                for (_ach_token_, _ach_pos_, _ach_ch_) in availChars:
                    self._addAvailChar(_ach_token_, _ach_ch_, _ach_pos_)

                for (_ach_token_, _ach_pos_, _ach_ch_) in endAvailChars:
                    while len(_endChars) <= _ach_pos_:
                        _endChars.append([])

                    if _endChars[_ach_pos_].count(_ach_ch_) == 0:
                        _endChars[_ach_pos_].append(_ach_ch_)

                gpos += 1
                pos += 1

    def _insertIncLineAction(self):

        """
        increment lineos token vizsgálata, ha azon a helyen szerepl action
        akkor ennek megfelelően kel módosítani:
        CONTINUE -> INC_LINE
        CLOSE -> CLOSE(incLine)
        """

        pass

    '''
    def _fillActionTable_old(self):




        for token in self.lexer._ordered[1:]:
            if token.infinity:

                pos = 0
                for pgroup in ["begin", "middle"]:
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
                                elif self.actionTable[self.lexer.default.name][char] == 0:
                                    self.actionTable[self.lexer.default.name][char] = ActionChangeTokenType(token)

                                for _alt in _alts:
                                    self._addAvailChar(self.lexer._tokens[_alt], char, pos)

                                #if len(_alts) > 1:
                                #    for _alt in _alts[1:]:
                                #        _atoken = self.lexer._tokens[_alt]
                                #        for achar in range(Lexer.ASCII_MIN, Lexer.ASCII_MAX):
                                #            if self.actionTable[token.name][achar] == 0:
                                #                self.actionTable[token.name][char] = ActionChangeTokenType(self.lexer._tokens[_alt])


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

                                    if pos == 1:
                                        for _achar in self._tokenChars[token.name][0:1]:
                                            for achar in _achar:
                                                if self.actionTable[self.lexer.default.name][achar] \
                                                    and self.actionTable[self.lexer.default.name][achar].token \
                                                    and self.actionTable[self.actionTable[self.lexer.default.name][achar].token.name][char] == 0:
                                                    self.actionTable[self.actionTable[self.lexer.default.name][achar].token.name][char] = ActionChangeTokenType(token)

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

            # ha van end pattern...
            #if token.end is not None:


            """ EZ KELL CSAK RONTJA AZ ÁTLÁTHATÓSÁGOT """
            for (char, action) in enumerate(self.actionTable[token.name]):
                if action != 0:
                    continue

                _alts = self._findAvailTokens(char, skipClosed=False)
                if len(_alts):
                    self.actionTable[token.name][char] = ActionClose(1, token)
                    # jelenleg nem kell, de lehet, hogy a skipp-el kombinálva majd jó lesz csak az offset lesz 0 olyankor
                    #for _alt in _alts:
                    #    if isinstance(self.actionTable[self.lexer.default.name][char], ActionClose) \
                    #        and self.actionTable[self.lexer.default.name][char].offset == 0:
                    #        self.actionTable[token.name][char] = ActionClose(1, token, self.lexer._tokens[_alt])
                    #    else:




        return
        for (key, value) in self.actionTable.iteritems():
            print key,
            for action in value:
                print action,
            print ""
    '''

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
            if isinstance(ch, list):
                chars = ch[:]
            else:
                chars = [[ch]]
        else:
            if length != -1:
                chars = self._tokenChars[token.name][0:length]
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

        while len(self._tokenChars[tok.name]) <= pos:
            self._tokenChars[tok.name].append([])

        if self._tokenChars[tok.name][pos].count(ch) == 0:
            self._tokenChars[tok.name][pos].append(ch)
