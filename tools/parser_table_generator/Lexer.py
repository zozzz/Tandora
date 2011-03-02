# -*- coding: utf-8 -*-
'''
Created on 2011.03.01.

@author: Zozzz
'''

ASCII_MIN = 0
ASCII_MAX = 128

INCREMENT_LINE_NUMBER = 1 << 0
SKIP_TOKEN = 1 << 1

import re;

class Match:

    def __init__(self, startsWith, between=None, endsWith=None, needContent=False):
        self.start = _expParser(startsWith);
        if between:
            self.between = _expParser(between)

        if endsWith:
            self.end = _expParser(endsWith)

        self.needContent = needContent


class StringTableMatch:

    def __init__(self):
        pass

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
                    ret.append(ord(r[2]))
                else:
                    raise Exception("Ismeretlen hiba...")

            if makeUnique:
                ret = _unique(ret)

            if rangeRes.group(1) == "^":
                return map((lambda item: item | 128), ret)

            return ret

        else:
            chars = exp.split("|")
            ret = []
            for ch in chars:
                for chch in ch:
                    ret.append(ord(chch))

            if makeUnique:
                return _unique(ret)
            return ret;

def _unique(s):
    u = {}
    try:
        for x in s:
            u[x] = 1
    except TypeError:
        del u  # move on to the next method
    else:
        return u.keys()
