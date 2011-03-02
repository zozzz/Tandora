# -*- coding: utf-8 -*-
'''
Created on 2011.03.01.

@author: Zozzz
'''

ASCII_MIN = 0
ASCII_MAX = 128

INCREMENT_LINE_NUMBER = 1 << 0
SKIP_TOKEN = 1 << 1
CLOSE_PREV_TOKEN = 1 << 2

import re;

class Match:

    def __init__(self,
                 startsWith,
                 between=None,
                 endsWith=None,
                 needContent=False,
                 skipClose=None,
                 eatChars=None,
                 charMaxCount=None):

        self.start = _expParser(startsWith);
        self.exact = not between and \
                     not endsWith and \
                     not eatChars and \
                     not charMaxCount

        if between:
            self.between = _expParser(between)
        else:
            self.between = None

        if endsWith:
            self.end = _expParser(endsWith)
        else:
            self.end = None

        if skipClose:
            self.skipClose = _expParser(skipClose)
        else:
            self.skipClose = None

        if charMaxCount:
            self.charMaxCount = {}
            for item in charMaxCount:
                self.charMaxCount[ord(item["ch"])] = item["count"]
        else:
            self.charMaxCount = None

        self.needContent = needContent


class StringTableMatch:

    def __init__(self, table, prefix=None):

        if isinstance(table, list):
            tdict = {}
            for item in table:
                key = ""
                if prefix:
                    key += prefix
                key += item.upper()
                tdict[key] = item
            table = tdict
        elif isinstance(table, dict):
            if prefix:
                tdict = {}
                for key, value in table.iteritems():
                    tdict[prefix+key] = value
                table = tdict

        self.table = table
        #self.prefix = prefix

class CharRange:

    def __init__(self, chars, ordered = True):
        self.chars = chars
        self.ordered = ordered

    def __len__(self):
        return len(self.chars)

    def __str__(self):
        return "CharRange[ordered:"+str(self.ordered)+"]: " + str(self.chars)

    def flatten(self):
        if self.ordered:
            return self.chars
        else:
            return _flattenHelper(self.chars)

    def _flattenHelper(self, x):
        ret = []
        for v in x:
            if isinstance(v, list):
                ret.extend(self._flattenHelper(v))
            else:
                ret.append(v)
        return ret

#===============================================================================
# Visszaad egy tömböt, ami azoknak a karaktereknek a kódját tartalmazza,
# amit a kifejezés leírt 0-127 (1: speciális karakter, ami illeszkedik mindenre)
# a 8-ik bit ha be van állítva 1-re akkor az a karakter nem szerepelhet benne
#===============================================================================
def _expParser(exp, makeUnique=True):

    if exp == "...":
        return [1]
    else:
        # rx_range = compile("\[(\^)?(.*?)\]")
        rangeRes = re.match(r"\[(\^)?(.*?)\]", exp)
        if rangeRes:
            if not rangeRes.group(2):
                raise Exception("Undefined range: " + exp)

            ret = []
            res = re.findall(r"(.-.)|(.)", rangeRes.group(2))

            for r in res:

                if r[0]:
                    r_parts = r[0].split("-")
                    ret.extend(range(ord(r_parts[0]), ord(r_parts[1]) + 1))
                elif r[1]:
                    ret.append(ord(r[1]))
                else:
                    raise Exception("Ismeretlen hiba...")

            if makeUnique:
                ret = _unique(ret)

            if rangeRes.group(1) == "^":
                ret = map((lambda item: item | 128), ret)

            return CharRange(ret, False)

        else:

            chars = exp.split("|")
            grouping = len(re.findall(r"([^\\|]{2,})", exp)) > 0
            ordered = len(chars) == 1 or grouping

            ret = []
            prevLength = 0
            prevIndex = 0
            c = 0
            for ch in chars:
                if grouping and prevLength > 1 and len(ch) == 1:
                    ret[prevIndex] = [ret[prevIndex], ord(ch)]
                elif grouping and len(ch) > 1 and c == len(chars)-1 and prevLength == 1:
                    first = ch[0:1]
                    last = ch[1:]

                    ret[prevIndex] = [ret[prevIndex], ord(first)]

                    for chch in last:
                        prevIndex = len(ret)
                        ret.append(ord(chch))
                else:
                    for chch in ch:
                        prevIndex = len(ret)
                        ret.append(ord(chch))
                prevLength = len(ch)
                c += 1

            return CharRange(ret, ordered);


    '''
    if exp == "...":
        return CharRange(1, False)
    else:
        ret = []
        ranges = re.findall(r"\[(\^)?(.*?)\]", exp)

        if ranges:
            for _range in ranges:
                parsedRange = []
                charRange = re.findall(r"(.-.)|(.)", _range[1])

                for chr in charRange:
                    if chr[0]:
                        parts = chr[0].split("-")

                        parsedRange.extend(range(ord(parts[0]), ord(parts[1]) + 1))
                    elif chr[1]:
                        parsedRange.append(ord(chr[1]))

                parsedRange = _unique(parsedRange)

                if _range[0] == "^":
                    parsedRange = map((lambda item: item | 128), parsedRange)

                _final = []
                _or = _final


                i = True
                c = 0
                for ch in parsedRange:
                    if c == 0 or c == len(parsedRange)-1:
                        _or.append(ch)
                    else:
                        _nor = [ch]
                        _or.append(_nor)
                        _or = _nor
                    i = not i
                    c+=1
                print _final

    '''





def _unique(s):
    u = {}
    try:
        for x in s:
            u[x] = 1
    except TypeError:
        del u
    else:
        return u.keys()
