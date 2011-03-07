# -*- coding: utf-8 -*-
'''
@author zozzz
'''

import sre_compile, sre_parse, sre_constants, re

class Token:

    IGNORE_CASE     = 1 << 0
    SKIP            = 1 << 1
    INC_LINE        = 1 << 2
    NEED_AS_STRING  = 1 << 3


    #pattern: regex kifejezés string formában lehetséges named groupok:
    #           - begin: ezt a részt vizsgálja, hogy elkezdje viszgálni a middle részt ha meg van adva
    #           - middle: OPCIONÁLIS, ez vizsgál mindent, ami nem a start részben van és a végén, ha meg van adva
    #           - end: OPCIONÁLIS, ez viszgálja a végét (middle megadása kötelező)
    #        lookupok, csak +/- 1 karakatert képesek
    def __init__(self, pattern, flag = 0):
        self.name = None
        self.group = None
        self.flag = flag
        self.lexer = None

        scf = 0
        if flag & self.IGNORE_CASE:
            scf |= sre_compile.SRE_FLAG_IGNORECASE

        cpattern = re.compile(pattern, scf)
        groupidx = cpattern.groupindex

        self.parsed = sre_parse.parse(pattern, scf)

        self.begin = None
        self.middle = None
        self.end = None
        self.exact = False

        for (op, (gidx, val)) in self.parsed:

            for (gname, idx) in groupidx.iteritems():
                if op == sre_constants.SUBPATTERN and gidx == idx:
                    if gname == "begin":
                        self.begin = MatchPattern(self, val, (flag & self.IGNORE_CASE) == self.IGNORE_CASE)
                    elif gname == "middle":
                        self.middle = MatchPattern(self, val, (flag & self.IGNORE_CASE) == self.IGNORE_CASE)
                    elif gname == "end":
                        self.end = MatchPattern(self, val, (flag & self.IGNORE_CASE) == self.IGNORE_CASE)

        if self.begin is None:
            raise Exception("Need to define 'begin' group!")

        if self.end is not None and self.middle is None:
            raise Exception("Need to define 'middle' group when 'end' defined!")

        if self.middle is None and self.end is None:
            _ex = True
            for (_op, _val) in self.begin._pattern:
                if _op != sre_constants.LITERAL:
                    _ex = False
                    break
            self.exact = _ex


    def maxWidth(self):
        (min, max) = self.parsed.getwidth()
        return max

    def minWidth(self):
        (min, max) = self.parsed.getwidth()
        return min

    def id(self):
        return self.lexer.tokenID(self.name)

    def group(self, idx):
        pass

    # átmenetileg felfüggesztve
    # @param Token pattern
    # @return 0: ha eygeznek
    #         1: ha ez a pattern nagyobb
    #        -1: ha a kapott a nagyobb
    def compare(self, token, maxWidth=-1):

        if maxWidth == -1:
            maxWidth = min(self.maxWidth(), token.maxWidth())

        pos = 0

        cache = []
        self._cmpTwoPattern(self.parsed, token.parsed, cache, [0, maxWidth])

    def _cmpTwoPattern(self, p1, p2, cache, range):
        print "\n_cmpTwoPattern"

        def _cacheLast(_cache_):
            if len(_cache_) > 0:
                return _cache_[len(_cache_)-1]
            return ("", 0)

        def _cachePop(_cache_, _name_):
            (_o_, _p_) = _cacheLast(_cache_)
            if _o_ == _name_:
                _cache_.pop()
                return (_o_, _p_)
            return ("", 0)



        def _addToCache(_cache_, op, val):
            _cache_.append((op, val))

        print "P1:", p1
        print "P2:", p2

        pos_in_p1 = 0
        pos_in_p2 = 0
        pos_in_str = 0
        while pos_in_p1 != len(p1):
            (op1, val1) = p1[pos_in_p1]

            while pos_in_p2 != len(p2):
                (op1, val1) = p1[pos_in_p1]

                if op1 == sre_constants.LITERAL:
                    chr_cache = []
                    res_chset = self._chrset(val1, p2, 0, chr_cache)
                    (cop, cval) = _cachePop(chr_cache, "fail")

                    print "res_chset", res_chset, cval

                    if cval == 1:
                        sub_cache = []
                        self._cmpTwoPattern(p2, (op1, val1), sub_cache, [pos_in_str, range[1]])

                elif op1 == sre_constants.SUBPATTERN:
                    self._cmpTwoPattern(val1[1], p2, cache, [pos_in_str, range[1]])

                elif op1 == sre_constants.BRANCH:

                    for branch_item in val1[1]:
                        cmp_res = self._cmpTwoPattern(branch_item, p2, cache, [pos_in_str, range[1]])
                        if not cmp_res:
                            return False
                    return True


                (cop, cval) = _cachePop(cache, "back")
                pos_in_str -= cval

                pos_in_p2 += 1

            pos_in_p1 +=1


    def _chrTest(self, ch, p):
        (op, val) = p
        pflag = self.flag

        if op == sre_constants.LITERAL:
            if pflag & self.IGNORE_CASE:
                if chr(ch).lower() == chr(val).lower():
                    return True
                else:
                    return False
            return ch == val

        elif op == sre_constants.IN:
            res = False
            negate = False

            for (sop, sval) in val:
                if sop == sre_constants.NEGATE:
                    negate = True
                    continue

                res = self._chrTest(ch, (sop, sval))
                if res == True:
                    break

            if negate:
                return not res
            else:
                return res

        elif op == sre_constants.RANGE:
            if pflag & self.IGNORE_CASE:
                if chr(ch).lower() >= chr(val[0]).lower() and chr(ch).lower() <= chr(val[1]).lower():
                    return True
                else:
                    return False
            return ch >= val[0] and ch <= val[1]

        elif op == sre_constants.CATEGORY:
            cat = None
            if val == sre_constants.CATEGORY_DIGIT:
                (_tmp, cat) = re.compile("[0-9]")
            elif val == sre_constants.CATEGORY_NOT_DIGIT:
                (_tmp, cat) = re.compile("[^0-9]")
            elif val == sre_constants.CATEGORY_WORD:
                (_tmp, cat) = re.compile("[a-zA-Z_]")
            elif val == sre_constants.CATEGORY_NOT_WORD:
                (_tmp, cat) = re.compile("[^a-zA-Z_]")
            elif val == sre_constants.CATEGORY_SPACE:
                (_tmp, cat) = re.compile("[\r\n\t ]")
            else:
                raise Exception("Unsupported category: " + val)

            return self._chrTest(ch, cat)

        else:
            return -1

        return False

    def _getPatternForCharPosition(self, pos):
        _pos = 0
        for (op, (gidx, val)) in self.parsed:
            (minw, maxw) = val.getwidth()
            _pos += maxw
            if _pos > pos:
                return val

        return None

    # Testel egy adott karakter sort, hogy illeszkedik-e
    # chars = [10, 20, 30, 40, ...]
    def test(self, chars, group=None):
        p = self.parsed

        if group is not None:
            if group == "begin":
                p = self.begin._pattern
            elif group == "middle":
                p = self.middle._pattern
            elif group == "end":
                p = self.end._pattern

        return self._test(p, chars, 0, 0)[0]

    def _test(self, pattern, chars, pos, level):

        def _print(*x):
            return
            print (level * "\t"),
            for _x in x:
                print _x,
            print ""

        chars = chars[:]

        maxPos = len(chars)-1
        res = False
        p_pos = 0
        add_pos = 1
        asserts = []
        appendAssert = None
        for (op, val) in pattern:

            for ch in chars[pos]:
                add_pos = 1
                ch_res = self._chrTest(ch, (op, val))
                _print("ch_res", pos, ch, ch_res)

                #print self.name, maxPos, "/", pos, ch, "=", ch_test_res
                if ch_res == -1:
                    if op == sre_constants.SUBPATTERN:
                        (res, pos) = self._test(val[1], chars, pos, level+1)
                        _print("sub_res", res, add_pos)

                    elif op == sre_constants.BRANCH:

                        for branch_item in val[1]:
                            (res, pos) = self._test(branch_item, chars, pos, level+1)
                            if res is True:
                                _print("branch_res", res, add_pos)
                                break

                    elif op == sre_constants.ASSERT or op == sre_constants.ASSERT_NOT:
                        asserts.append((op == sre_constants.ASSERT_NOT, val))

                        """
                        _print("assert", pos)

                        assert_res = self._test(val[1], chars, pos, level+1)
                        _print(assert_res)
                        if op == sre_constants.ASSERT_NOT:
                            res = not assert_res[0]
                        else:
                            res = assert_res[0]

                        return (res, assert_res[1])
                        """

                        _print("assertAdded", pos, maxPos)

                        if len(chars) <= pos + (val[0] * -1):
                            return (False, pos)
                        else:
                            pos += (val[0] * -1)

                        #res = True

                        if val[0] < 0:
                            continue

                        """if pos == maxPos:
                            print "assertAdded and overflow"
                            return (False, pos)"""
                else:
                    res = ch_res


                if res == True and len(asserts) > 0:
                    (negate, _assert) = asserts.pop()
                    assert_res = self._test(_assert[1], chars, pos + _assert[0], level+1)

                    _print("assert", _assert[1], pos, maxPos, assert_res)

                    if negate:
                        res = not assert_res[0]
                    else:
                        res = assert_res[0]


                _print("before_ret", pos, res, maxPos)
                if res == False or pos >= maxPos:
                    _print("return:", (res, pos))
                    return (res, pos)

                pos += add_pos

            p_pos += 1

        return (res, pos)


    def __str__(self):
        return "<Lexer.Token "+str(self.name)+" begin="+str(bool(self.begin))+" middle="+str(bool(self.middle))+" end="+str(bool(self.end))+">"

    def __getattr__(self, name):
        if name == "begin":
            return self.begin
        elif name == "middle":
            return self.middle
        elif name == "end":
            return self.end
        else:
            raise AttributeError()


class MatchPattern:

    @classmethod
    def convertInOpToChars(cls, op):
        (_in, _list) = op

        ret = []
        negate = False
        for (_op, _val) in _list:
            if _op == sre_constants.RANGE:
                (_min, _max) = _val
                ret.extend(range(_min, _max))
            elif _op == sre_constants.LITERAL:
                ret.append(_val)
            elif _op == sre_constants.NEGATE:
                negate = True

        return (negate, ret)

    def __init__(self, token, parsed, ignoreCase=False):
        self._pattern = parsed
        self._ignoreCase = ignoreCase
        self._minLength = -1
        self._token = token
        self._chars = self._convertToCharList()

    def minWidth(self):
        (min, max) = self._pattern.getwidth()
        return min

    def maxWidth(self):
        (min, max) = self._pattern.getwidth()
        return max

    '''
    # @param MatchPattern pattern
    # @return 0: ha eygeznek
    #         1: ha ez a pattern nagyobb
    #        -1: ha a kapott a nagyobb
    def compare(self, pattern, maxWidth=-1):
        print self._pattern
        print pattern._pattern

    # Testel egy adott karakter sort, hogy illeszkedik-e
    # chars = [10, 20, 30, 40, ...]
    def test(self, chars):
        pass

    # @return [ (1, 2, [(1, [])]) ]
    # két szintű touple, az első szinten
    def possibleCharsAt(self, chars, idx):
        pass
    '''

    '''
    lehet ez nem fog kelleni nem elég beszédes
    @return [
        (1, 2, [10, 13]), // min length 1, max 2, karakter lista
        (1, 2, [
            (2, 2, [10, 13]),
            (1, 1, [13])
        ])  // min length 1, max 2, karakter lsita, ami ugyan ezt a struktúrát alkalmazza
    ]
    '''
    def _convertToCharList(self):
        pass

    def __str__(self):
        return "<Lexer.MatchPattern>"