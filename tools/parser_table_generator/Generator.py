# -*- coding: utf-8 -*-
'''
Created on 2011.03.01.

@author: Zozzz
'''

import Lexer
import pprint
from odict import odict


def dump(x):
    pprint.pprint(x)

class Generator:


    def __init__(self, module, outputFilePath):
        self.matches = []
        self.tokens = {}
        self.indexByStartChar = {}
        self.file = outputFilePath

        options = getattr(module, "OPTIONS")

        # Mindegyik match objektum legyen
        self._normalizeTokens(getattr(module, "TOKENS"))
        #dump(self.tokens)
        dump(self.indexByStartChar)
        ##dump(self.matches)


    def _normalizeTokens(self, tokens, pk=None):
        for key, value in tokens.iteritems():
            if isinstance(value, str):
                self.tokens[key] = {"match":Lexer.Match(value), "group":pk, "name":key}
                self._insertMatch(self.tokens[key])
                self._indexByChar(key, self.tokens[key]["match"])
            elif isinstance(value, dict):
                self._normalizeTokens(value, key)
            elif isinstance(value, Lexer.Match):
                self.tokens[key] = {"match":value, "group":pk, "name":key}
                self._insertMatch(self.tokens[key])
                self._indexByChar(key, self.tokens[key]["match"])
                continue
            elif isinstance(value, Lexer.StringTableMatch):
                self._normalizeTokens(value.table, key)
            else:
                raise Exception("This type not allowed, key: " + key + ", instead: <Lexer.StringTableMatch>")
        return tokens

    def _insertMatch(self, m):
        key = 0
        for item in self.matches:
            if len(item["match"].start) > len(m["match"].start):
                self.matches.insert(key, m)
                return
            key += 1

        self.matches.append(m)

    def _indexByChar(self, k, m):
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


    def create(self):
        print "creating: " + self.file + "...",
        print "[OK]"


class Action:
    PAD_LENGTH = 8

    def __init__(self, number):
        self.number = number

    def asHex(self):
        r = "0x%0." + str(Action.PAD_LENGTH) + "X"
        return r % (self.number)


class ActionMatchPrevToken(Action):
    def __init__(self):
        pass

class ActionNotMatchPrevToken(ActionMatchPrevToken):
    def __init__(self):
        pass

class ActionCharAt(Action):
    def __init__(self):
        pass

class ActionNotCharAt(ActionCharAt):
    def __init__(self):
        pass

class ActionClose(Action):
    def __init__(self):
        pass

class ActionChangeTokenType(Action):
    def __init__(self):
        pass

class ActionIncrementLN(Action):
    def __init__(self):
        pass

class ActionContinue(Action):
    def __init__(self):
        pass

class ActionError(Action):
    def __init__(self):
        pass