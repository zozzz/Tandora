'''
Created on 2011.03.01.

@author: Zozzz
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
    "LI_FLOAT": Match("[0-9.]", "[0-9.eE+-]", needContent=True, charMaxCount=[
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