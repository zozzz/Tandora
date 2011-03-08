# -*- coding: utf-8 -*-
'''
@author zozzz
'''

from Token import Token
import re

__all__ = ["Lexer", "Token"]

class Lexer:

    DEBUG = False
    DEFAULT_TOKEN_NAME = "DEFAULT"

    ASCII_MIN = 0
    ASCII_MAX = 128

    def __init__(self, module):
        self.default = None
        self._module = module
        self._tokens = {}
        self._tokensByGroup = [] # touple list (group, token)

        # ordered list from tokens
        # lowest width first
        self._ordered = []

        self._makeTokenStructs(getattr(module, "TOKENS"))

        if self.default is None:
            deft = Token("(?P<begin>)")
            deft.name = self.DEFAULT_TOKEN_NAME
            self.default = deft
            self._tokens[deft.name] = deft
            self._addToIndex(deft)

        #for t in self._ordered:
        #    print t

    def tokenID(self, name):
        idx = 0
        for (g, n) in self._tokensByGroup:
            if n == name:
                return idx
            idx += 1

        raise Exception("Undefined token name: '"+str(name)+"'")

    def tokenRange(self, group):
        _min = -1
        _max = -1

        idx = 0
        findLast = False
        for (g, n) in self._tokensByGroup:
            if findLast is False and g == group:
                findLast = True
                _min = _max = idx
            elif findLast and g == group:
                _max += 1
            idx += 1

        if _min == -1:
            raise Exception("Undefined token group: '"+str(group)+"'")

        if _max == -1:
            if findLast:
                _max = len(self._tokensByGroup)-1
            else:
                _max = _min

        return (_min, _max)

    def token(self, name):
        return self.token[name]

    def __getattribute__(self, name):
        return self.token[name]

    def _makeTokenStructs(self, tokens, parent=None):

        for name, value in tokens.iteritems():

            if isinstance(value, Token):
                value.name = name
                value.group = parent
                value.lexer = self
                self._tokens[name] = value
                self._addToIndex(value)

            elif isinstance(value, str):
                tok = Token("(?P<begin>" + re.escape(value) + ")")
                tok.name = name
                tok.group = parent
                tok.lexer = self
                self._tokens[name] = value
                self._addToIndex(value)

            elif isinstance(value, TokenTable):
                self._makeTokenStructs(value.table, name)

            elif isinstance(value, dict):
                self._makeTokenStructs(value, name)

    def _addToIndex(self, token):
        if token.maxWidth() == 0:
            self.default = token

        self._addToTokensByGroup(token)

        inserted = False
        for key, val in enumerate(self._ordered):
            if val.maxWidth() > token.maxWidth():
                inserted = True
                self._ordered.insert(key, token)
                break

        if not inserted:
            self._ordered.append(token)

    def _addToTokensByGroup(self, token):
        pos = 0

        if token.group is None:
            tgroup = token.name
        else:
            tgroup = token.group

        if token.maxWidth() != 0:
            findLast = False
            i = 0
            for (group_name, token_name) in self._tokensByGroup:
                if findLast is False and group_name == tgroup:
                    findLast = True
                    pos = i
                elif findLast and group_name == tgroup:
                    pos += 1
                    break
                i += 1

            if pos == 0:
                pos = len(self._tokensByGroup)

        self._tokensByGroup.insert(pos, (tgroup, token.name))

    # @param List chars
    # @param List groups [begin, middle, end]
    # amelyik exact az prioritást élvez
    def find(self, chars, groups=None):
        pass

    # @param Token token
    # @param Int ch Karakterkód
    # @param Int pos Aktuális pozíció, ami előtt egyeznie kell a token.begin résznek
    #                és az alterToken.begin résznek + az alterToken.begin-nek illeszkednie
    #                kell ebben a pozícióban lévő ch-ra
    def findAlternative(self, token, ch, pos):
        pass


class TokenTable:

    CASE_UPPER = "upper"
    CASE_LOWER = "lower"
    CASE_NO_CHANGE = "no_change"

    def __init__(self, table, prefix=None, flag=0, chngCase="upper"):
        self.table = {}

        prep = {}
        if isinstance(table, list):
            for val in table:
                prep[val] = Token("(?P<begin>" + re.escape(val) + ")", flag)
        else:
            prep = table

        for key, value in prep.iteritems():
            if prefix is not None:
                nKey = prefix
            else:
                nKey = None

            nValue = None
            if chngCase == self.CASE_LOWER:
                nKey += key.lower()
            elif chngCase == self.CASE_UPPER:
                nKey += key.upper()
            else:
                nKey += key

            if isinstance(value, Token):
                nValue = value
                value.flag |= flag
            elif isinstance(value, str):
                nValue = Token("(?P<begin>" + re.escape(value) + ")", flag)
            else:
                raise Exception("Unsupported type: " + value)

            self.table[nKey] = nValue