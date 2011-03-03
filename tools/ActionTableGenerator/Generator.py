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

        #dump(self.indexByStartChar)

        self._insertStartActions()

        dump(self.actionTable["DEFAULT"])


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

    def _getTokenID(self, name):
        i = 0
        for item in self.matches:
            if item["name"] == name:
                return i
            i+=1

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

    def _insertStartActions(self):
        _idx = self.indexByStartChar

        for m in self.matches:
            currTok = m["name"]

            if currTok == self.DEFAULT_NAME:
                continue

            match = m["match"]
            chr = match.start

            #if currTok == "IDENTIFIER":
            #    print chr

            pos = 0
            for chs in chr.chars:
                if not isinstance(chs, list):
                    chs = [chs]

                #print chr.chars

                for ch in chs:
                    tokLookAhead = None
                    if len(_idx) > pos and _idx[pos].has_key(ch) and len(_idx[pos][ch]) > 1:
                        tokLookAhead = [item for item in _idx[pos][ch] if item != currTok]

                    #if tokLookAhead:

                    # ha van look ahead akkor, ha pos+1 char vizsgálat nem teljesül átváltunk arra egyébként hiba

                    if pos == 0:
                        if self.actionTable[self.DEFAULT_NAME][ch] == 0:
                            if not tokLookAhead and match.exact and len(chr) == 1:
                                if (self.options.get(currTok, 0) & Lexer.SKIP_TOKEN) == 0:
                                    self.actionTable[self.DEFAULT_NAME][ch] = ActionClose(True, 0, self._getTokenID(currTok))
                                else:
                                    self.actionTable[self.DEFAULT_NAME][ch] = ActionContinue()
                            else:
                                self.actionTable[self.DEFAULT_NAME][ch] = ActionChangeTokenType(self._getTokenID(currTok))


                #    print currTok, pos, chr.chars[0:pos+1]


                    #print currTok, pos, "->", tokLookAhead
                if chr.ordered:
                    pos += 1

        return
        for pos, chrs in self.indexByStartChar.iteritems():

            for ch, tokens in chrs.iteritems():
                currTok = tokens[0]
                actionToken = currTok
                match = self.tokens[currTok]["match"]
                chr = match.start

                #print currTok, "find: ", pos-1, chr.chars[0:pos]

                tokLookAhead = self._findInStartIdx(pos, chr.chars[0:pos], [currTok])

                if not tokLookAhead and len(tokens) > 1:
                    tokLookAhead = tokens[1]

                #print pos, currTok, "->", tokLookAhead

                # spec eset mindig a default indítja el itt a dolgokat
                if pos == 0:
                    actionToken = self.DEFAULT_NAME

                # \r (\n ? chng:CRLF : )
                # \r\n

                #print currTok, len(chr.chars), pos+1, tokLookAhead

                #if chr.ordered and tokLookAhead

                '''
                if chr.ordered and len(chr.chars) == pos+1 and not tokLookAhead:
                    # todo vizsgálni, hogy elég-e ennyi ahoz hogy lezárja
                    if self.options.get(currTok, 0) & Lexer.CLOSE_PREV_TOKEN:
                        print actionToken, "close", currTok
                        self.actionTable[actionToken] = ActionClose(False, -1, self._getTokenID(currTok))

                #print currTok, "->", tokLookAhead
                '''


    def create(self):
        print "creating: " + self.file + "...",
        print "[OK]"


