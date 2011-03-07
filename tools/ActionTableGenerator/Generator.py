# -*- coding: utf-8 -*-
'''
Created on 2011.03.01.

@author: Zozzz
'''

# tábla tömrítési tipp:
# az olyan token mint pl: "{" nem igényélnek saját action tablet (exact && chars.length == 1)

import Lexer
from Action import *
import pprint
from odict import odict


def dump(x):
    pprint.pprint(x)

class Generator:
    DEFAULT_NAME = "DEFAULT"


    def __init__(self, module, outputFilePath):
        self.matches = []
        self.tokens = {}
        self.actionTable = {}
        self.indexByStartChar = {}
        self.file = outputFilePath
        self.options = getattr(module, "OPTIONS")

        # Mindegyik match objektum legyen
        t = getattr(module, "TOKENS")
        t[self.DEFAULT_NAME] = Lexer.Match("")
        self._normalizeTokens(t)

        for match in self.matches:
            self._indexingByStartChar(match["name"], match["match"])

        self._createActionTables();

        #dump(self.actionTable)


    def _normalizeTokens(self, tokens, pk=None):
        for key, value in tokens.iteritems():

            if isinstance(value, str):
                self.tokens[key] = {"match":Lexer.Match(value), "group":pk, "name":key}
                self._insertMatch(self.tokens[key])
            elif isinstance(value, dict):
                self._normalizeTokens(value, key)
            elif isinstance(value, Lexer.Match):
                self.tokens[key] = {"match":value, "group":pk, "name":key}
                self._insertMatch(self.tokens[key])
                continue
            elif isinstance(value, Lexer.StringTableMatch):
                self._normalizeTokens(value.table, key)
            else:
                raise Exception("This type not allowed, key: " + key + ", instead: <Lexer.StringTableMatch>")
        return tokens

    def _insertMatch(self, m):
        self.actionTable[m["name"]] = []

        for x in range(Lexer.ASCII_MIN, Lexer.ASCII_MAX):
            self.actionTable[m["name"]].append(0)

        key = 0
        for item in self.matches:
            if len(item["match"].start) > len(m["match"].start):
                self.matches.insert(key, m)
                return
            key += 1

        self.matches.append(m)

    def _indexingByStartChar(self, k, m):
        # TODO: figyelembe venni ha negált karaterről van szó + a ... (1) -et
        pos = 0
        ordered = m.start.ordered
        for char in m.start.chars:
            if self.indexByStartChar.get(pos, None) == None:
                self.indexByStartChar[pos] = {}

            if not isinstance(char, list):
                char = [char]

            for ch in char:
                if self.indexByStartChar[pos].get(ch, None) == None:
                    self.indexByStartChar[pos][ch] = []

                if self.indexByStartChar[pos][ch].count(k) == 0:
                    self.indexByStartChar[pos][ch].append(k)

            if ordered:
                pos += 1

    '''
    def _findInStartIdx(self, pos, chrs, exclude):
        if not isinstance(chrs, list):
            chrs = [chrs]

        exclude.append(self.DEFAULT_NAME)

        matched = []
        idx = self.indexByStartChar
        for p in range(pos, -1, -1):
            #print "p:", p,
            for ch in chrs:
             #   print "ch:", ch, idx[p].has_key(ch),

                if idx[p].has_key(ch) and len(idx[p][ch]) > 0:
              #      print "EXTEND: ", idx[p][ch],
                    matched.extend(idx[p][ch])
            #print " "

        #matched = [item for item in matched if 0 and matched.count(item) == pos and exclude.count(item) == 0]
        ret = []
        for m in matched:
            if matched.count(m) == pos+1 and exclude.count(m) == 0 and ret.count(m) == 0:
                ret.append(m)
        if len(ret) > 0:
            return ret

        return None
    '''

    # ez nem kell :)
    '''
    def _findLookAhead(self, tok, pos):
        _idx = self.indexByStartChar
        _tok = self.tokens[tok]
        _match = _tok["match"]

        _testPos = 0
        _testedChars = []
        if _match.start.ordered:
            for chs in _match.start.chars:
                if not isinstance(chs, list):
                    chs = [chs]

                for ch in chs:
                    if len(_idx) > pos and _idx[pos].has_key(ch):
                        if len(_idx[pos][ch]) > 1 and _testPos == pos:
                            #print [item for item in _idx[pos][ch] if item != tok],
                            for tla in _idx[pos][ch]:
                                _tla = self.tokens[tla]
                                if _tla["match"].start.ordered:
                                    _tla_pos_ = 0
                                    for _tla_chs_ in _tla["match"].start.chars:
                                        if not isinstance(_tla_chs_, list):
                                            _tla_chs_ = [_tla_chs_]

                                        _tla_matched = True
                                        for _tla_ch_ in _tla_chs_:
                                            if len(_testedChars) <= _tla_pos_:
                                                break;
                                            if _tla_matched and _testedChars[_tla_pos_] != _tla_ch_:
                                                _tla_matched = False
                                                break;

                                        if not _tla_matched:
                                            break

                                        if _tla_pos_ == pos and tla != tok:
                                            return tla
                                        _tla_pos_ +=1

                                    if _tla_pos_ > pos:
                                        break

                    _testedChars.append(ch)
                _testPos += 1


            ##print tok, _avail, _deny

        return None
    '''

    def _createActionTables(self):

        for t in self.matches:
            if t["name"] == self.DEFAULT_NAME:
                self._fillDefaultActionTable();
            elif (t["match"].exact and len(t["match"].start)>1) or not t["match"].exact:
                self._fillActionTable(t["name"])


    # Megkeresi, hogy milyen token lesz a következő, ha a feltételek nem igazak
    # feltétel: (tok.chars[pos] != ch) ebben az esetben keres egy alternatívát
    # ha saját magával tér vissza, akkor ez a char ebben a pizícióban ok
    # ha nem saját magával akkor token típust kell váltani, ha (exact a match típúsa | középső feltétel nem igaz rá | befejező feltétel nem igaz rá | eatChars nem igaz rá)
    # ha None-al akkor hibát kell dobni ennél a karakternél
    def _findAlternativeToken(self, tok, pos, ch):
        match = self.tokens[tok]["match"]
        idx = self.indexByStartChar
        _availAlt = []

        print "\t_findAlternativeToken", pos, chr(ch)

        if match.start.ordered:

            if len(match.start) > pos and match.start.chars[pos] == ch:
                print "\t\tfull match"
                return tok

            _possible = []
            if len(idx) > pos and idx[pos].has_key(ch):
                _possible.extend(idx[pos][ch])

            for _key_, _tok_ in self.tokens.iteritems():
                if not _tok_["match"].start.ordered and ch in _tok_["match"].start.flatten() and _possible.count(_tok_["name"]) == 0:
                    _possible.append(_tok_["name"])

            print "\t\tpossible:", _possible
            for _alt in _possible:
                if _alt == tok:
                    continue

                _altMatch = self.tokens[_alt]["match"]
                if _altMatch.start.ordered:
                    if len(_altMatch.start) < pos:
                        continue

                cmp = self._cmpCharRange(match.start, _altMatch.start, pos-1)
                print "\t\tCMP: ", _alt, cmp
                if cmp < 0:
                    continue

                if not _alt in _availAlt:
                    _availAlt.append(_alt)

        else:
            pass

        if len(_availAlt) > 0:
            print "\t\tALTS:", _availAlt
            for _alt in _availAlt:
                alt = self.tokens[_alt]

                if alt["match"].start.ordered:
                    chs = self._normalizeChar(alt["match"].start.chars[pos])
                else:
                    chs = alt["match"].start.chars

                for _ch in chs:
                    if _ch == ch:
                        return _alt

        return None

    def _fillDefaultActionTable(self):
        pass

    def _fillActionTable(self, tokenName):

        # iterate 0...128
        #       _determineAction(...)
        # megfelelő viszgálatok...
        # mindenki boldog :)

        for actionChar in range(Lexer.ASCII_MIN, Lexer.ASCII_MAX):
            action = self._determineAction(tokenName, self.tokens[tokenName]["match"], actionChar)



    def _determineAction(self, token, match, actionChar):

        print token, '"'+chr(actionChar)+'"', actionChar

        if match.start.ordered:
            pos = 1
            for chs in match.start.chars[1:]:
                chs = self._normalizeChar(chs)

                for ch in chs:

                    altToken = self._findAlternativeToken(token, pos, actionChar)
                    if altToken:
                       print token, "->", altToken
                       pass

                pos += 1

        pass

    def _normalizeChar(self, chs):
        if not isinstance(chs, list):
            return [chs]
        return chs

    def _tokenID(self, name):
        i = 0
        for item in self.matches:
            if item["name"] == name:
                return i
            i+=1

    # return  0: ha egyenlő
    #         x: ha az első nagyobb ahol az x azt jelzi mennyivel nagobb
    #        -x: ha a második nagyobb ahol az ax azt jelzi mennyivel nagyobb
    def _cmpCharRange(self, range1, range2, maxLength=-1):

        # range1 ordered && range2 ordered -> hossz vizsgálat elsőnek maxLength-et figyelembe véve ha nem -1
            # ha egyenlőek végigmenni range1 karakterein és megvizsgálni, hogy egyezik-e rannge2-vel

        #print range1.ordered, range2.ordered, maxLength

        if range1.ordered and range2.ordered:

            pos = 0
            for chs in range1.chars:
                chs1 = self._normalizeChar(chs)

                if len(range2.chars) <= pos:
                    return pos - len(range1.chars)

                chs2 = self._normalizeChar(range2.chars[pos])

                _matched = False
                for ch1 in chs1:
                    for ch2 in chs2:
                        if ch1 == ch2:
                            _matched = True
                            break;
                    if _matched:
                        break

                if not _matched:
                    return pos - len(range2.chars)

                if maxLength != -1 and maxLength == pos:
                    return 0

                pos += 1

            return 0


        # range1 !ordered & range2 ordered -> felcserélés, vizsgálat, majd előjel váltás
        preSign = 1
        if not range1.ordered and range2.ordered:
            preSign = -1
            _tmp = range1
            range1 = range2
            range2 = _tmp

        # range1 ordered && range2 !ordered
        if range1.ordered and not range2.ordered:

            pos = 0
            for chs1 in range1.chars:
                chs1 = self._normalizeChar(chs1)
                matched = False
                for ch1 in chs1:

                    for chs2 in range2.chars:
                        chs2 = self._normalizeChar(chs2)
                        for ch2 in chs2:

                            if ch1 == ch2:
                                matched = True
                                break

                        if matched:
                            break


                if not matched:
                    return -(pos+1) * preSign

                if maxLength != -1 and maxLength == pos:
                    return pos * preSign

                pos += 1

            return 0

        # egyik se ordered -> range1-range2

        pass


    def create(self):
        print "creating: " + self.file + "...",
        print "[OK]"
