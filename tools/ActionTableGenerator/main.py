'''
Created on 2011.03.01.

@author: Zozzz
'''

import Lexer
from Token import Token
from ActionTable import *

#Lexer.Lexer.DEBUG = True

import JSONLexer

import pprint

def dump(x):
    pprint.pprint(x)

#TandoraSRCDir = "c:\\Users\\Zozzz\\Documents\\Tandora\\src\\"
TandoraSRCDir = "d:\\Works\\cpp\\Tandora\\src\\"

def _out(x):
    return x, 1

if __name__ == '__main__':

    json = ActionTable("parser.json", JSONLexer, TandoraSRCDir + "parser\\JSONActionTable", True)
    json.create()

    #print Action(10233800).asHex()

    #json = Generator(JSONLexer, TandoraSRCDir + "parser\\JSONActionTable.h")
    #json.create()

    #rex = re.compile("[0.]|\.|[1-9]", re.DEBUG)
    #print rex

    #x = "(?P<begin>[0.]|\.|[1-9])(?P<middle>[0-9]*)(?P<end>[^0-9])"

    '''
    comp = sre_compile.compile(x, re.DEBUG)
    print comp.groupindex


    t = sre_parse.parse(x)
    for op, value in t:
        print op, value

    '''

    _patterns = [
        "1*",
        "1?",
        "1+",
        "1+?",
        "1{1,}",
        "1{1,2}",
        "1*?"
    ]


    import re

    #for p in _patterns:
    #    print "\n" + p + ":"
    #    re.compile(p, re.DEBUG)



    CMP1 = json.lexer._tokens["LI_FLOAT"]
    CMP2 = json.lexer._tokens["LI_HEX"]
    LI_FALSE = json.lexer._tokens["LI_FALSE"]


    #print CMP1.test();

    #CMP1.parsed.dump()

    # (?P<begin>((?<=0)x)|((?<=[0-9])\.)|\.)(?P<middle>([0-9.]+(e(\+|-)?[0-9]+)?))
    #print [[ord('0')], [ord("x")]]
    #print CMP1.test([[ord('0')]])
    #print CMP1.test([[ord('z'), ord('0'), ord('1')], [ord('.')]])

    test = "alma"

    """chrs = [
        [ord("h")],
        [ord("e")],
        [ord("l")],
        [ord("l")],
        [ord("o")]
    ]"""

    chrs = []
    for ch in test:
        chrs.append([ord(ch)])

    chrs.append([0])

    #print len(chrs)
    #print json.lexer._tokens["IDENTIFIER"].test(chrs, resultType=Token.RESULT_EXTEND)

    #print json.lexer._tokens["LI_FLOAT"].minWidth()
    #print json.lexer._tokens["LI_INT"].minWidth()

    #json.lexer._tokens["LI_FLOAT"].parsed.dump()
    #print json.lexer._tokens["LI_FLOAT"].test(chrs, resultType=Token.RESULT_EXTEND)


    #print json.lexer._tokens["IDENTIFIER"].test(chrs, resultType=Token.RESULT_EXTEND)
    #print Token("(?P<begin>\<.*?\>[a-z\s]+)", Token.IGNORE_CASE).test(chrs, resultType=Token.RESULT_EXTEND)

    #print CMP1.compare(CMP2)