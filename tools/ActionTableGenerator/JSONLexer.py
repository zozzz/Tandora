# -*- coding: utf-8 -*-
'''
@author zozzz
'''

'''
from Lexer import *;

KEYWORDS = [
    "case","catch","continue"
]

LITERALS = {
    "LI_TRUE": Match("true"),
    "LI_FALSE": Match("false"),
    "LI_NULL": Match("null"),
    "LI_STRING": Match("\"|\'", "...", "\"|\'", True, "\\"),
    "LI_INT": Match("[1-9]", "[0-9]", needContent=True),
    "LI_FLOAT": Match("[0.]|\.|[1-9]", "[0-9.eE+-]", needContent=True, charMaxCount=[
        {"ch":".", "count":1},
        {"ch":"e", "count":1},
        {"ch":"E", "count":1},
        {"ch":"+", "count":1},
        {"ch":"-", "count":1}
    ]),
    "LI_HEX": Match("0x|X", "[0-9ABCDEFabcdef]", needContent=True)
}

OPERATORS = {
    "LSQRBRACER"    : "[",
    "RSQRBRACER"    : "]",
    "LCURBRACER"    : "{",
    "RCURBRACER"    : "}",
    "COLON"         : ":",
    "PLUS"          : "+",
    "MINUS"         : "-",
    "COMMA"         : ","
}

WHITESPACES = {
    "WS_LF"     : "\n",
    "WS_CR"     : "\r",
    "WS_CRLF"   : "\r\n",
    "WS_SPACE"  : " ",
    "WS_TAB"    : "\t"
}

#WHITESPACES = {
#    "WS_LF"     : Match("\n", "[\r\n\t ]", "[^\r\n\t ]"),
#    "WS_CR"     : Match("\r", "[\r\n\t ]", "[^\r\n\t ]"),
#    "WS_CRLF"   : Match("\r\n", "[\r\n\t ]", "[^\r\n\t ]"),
#    "WS_SPACE"  : Match(" ", "[\r\n\t ]", "[^\r\n\t ]"),
#    "WS_TAB"    : Match("\t", "[\r\n\t ]", "[^\r\n\t ]")
#}

# TOKENS, OPTIONS always needed
TOKENS = {
    "IDENTIFIER"    : Match("[a-zA-Z$_]", "[0-9a-zA-Z$_]"),
    "KEYWORD"       : StringTableMatch(KEYWORDS, "KW_"),
    "OPERATOR"      : StringTableMatch(OPERATORS, "OP_"),
    "LITERAL"       : LITERALS,
    "WHITESPACE"    : WHITESPACES,
    "PREPROCESSOR"  : Match("#", "...", "\r|\n", True, "/", " |\t"),
    "COMMENT"       : {
        "CT_SINGLE" : Match("//", "...", "\r|\n"),
        "CT_BLOCK" : Match("/*", "...", "*/", True)
    }
}

OPTIONS = {
    "WS_LF" : INCREMENT_LINE_NUMBER | SKIP_TOKEN | CLOSE_PREV_TOKEN,
    "WS_CR" : INCREMENT_LINE_NUMBER | SKIP_TOKEN | CLOSE_PREV_TOKEN,
    "WS_CRLF" : INCREMENT_LINE_NUMBER | SKIP_TOKEN | CLOSE_PREV_TOKEN,
    "WS_SPACE": SKIP_TOKEN | CLOSE_PREV_TOKEN,
    "WS_TAB" : SKIP_TOKEN | CLOSE_PREV_TOKEN,
    "CT_SINGLE": SKIP_TOKEN
}

'''

from Lexer import Token, TokenTable

OPERATORS = {
    "LSQRB"    : "[",
    "RSQRB"    : "]",
    "LCURB"    : "{",
    "RCURB"    : "}",
    "COLON"    : ":",
    "PLUS"     : "+",
    "MINUS"    : "-",
    "COMMA"    : ","
}

LITERALS = {
    "TRUE"   : "true",
    "FALSE"  : "false",
    "NULL"   : "null",
    "STRING" : Token("(?P<begin>'|\")(?P<middle>.*?)(?P<end>(?P=begin))", Token.NEED_AS_STRING),
    "INT"    : Token("(?P<begin>[1-9])(?P<middle>[0-9]*)", Token.NEED_AS_STRING),
    "FLOAT"  : Token("(?P<begin>[0-9]|\.)(?P<middle>([0-9.]+(e(\+|\-)?[0-9]+)?))", Token.IGNORE_CASE | Token.NEED_AS_STRING),
    "HEX"    : Token("(?P<begin>0x)(?P<middle>[0-9abcdef]*)", Token.IGNORE_CASE | Token.NEED_AS_STRING)
}

WHITESPACES = {
    "LF"    : Token("(?P<begin>\n)", Token.INC_LINE),
    "CR"    : Token("(?P<begin>\r)", Token.INC_LINE),
    "CRLF"  : Token("(?P<begin>\r\n)", Token.INC_LINE),
    "SPACE" : " ",
    "TAB"   : "\t"
}

COMMENTS = {
    "SINGLE"    : Token("(?P<begin>//)(?P<middle>.*?)(?P<end>\r|\n)"),
    "BLOCK"     : Token("(?P<begin>/\\*)(?P<middle>.*?)(?P<end>\\*/)")
}

TOKENS = {
    "UNDEFINED"     : Token("(?P<begin>)"),
    "IDENTIFIER"    : Token("(?P<begin>[a-z$_])(?P<middle>[0-9a-z$_]*)", Token.IGNORE_CASE),
    "OPERATOR"      : TokenTable(OPERATORS, "OP_"),
    "LITERAL"       : TokenTable(LITERALS, "LI_"),
    "WHITESPACE"    : TokenTable(WHITESPACES, "WS_", Token.SKIP),
    "COMMENT"       : TokenTable(COMMENTS, "CT_", Token.SKIP),

    "KEYWORDS"      : TokenTable(["case", "catch", "continue", "this", "is", "assert"], "KW_")
}