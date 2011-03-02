'''
Created on 2011.03.01.

@author: Zozzz
'''

from Lexer import *;

KEYWORDS = [
]

LITERALS = {

}

TOKENS = {
    "IDENTIFIER"    : Match("[a-z$_]", "[0-9a-z$_]"),
    "KEYWORD"       : StringTableMatch(KEYWORDS),
    "LITERAL"       : LITERALS,
    "WHITESPACE"    : {
        "WS_LF" : "\n",
        "WS_CR" : "\r",
        "WS_CRLF" : "\r\n",
        "WS_SPACE" : " ",
        "WS_TAB" : "\t"
    },
    "PREPROCESSOR"  : Match("#", "...", "\r|\n"),
    "COMMENT"       : {
        "CT_SINGLE" : Match("//", "...", "\r|\n"),
        "CT_BLOCK" : Match("/*", "...", "*/")
    }
}

OPTIONS = {
    "WS_LF" : INCREMENT_LINE_NUMBER | SKIP_TOKEN,
    "WS_CR" : INCREMENT_LINE_NUMBER | SKIP_TOKEN,
    "WS_CRLF" : INCREMENT_LINE_NUMBER | SKIP_TOKEN,
    "WS_SPACE": SKIP_TOKEN,
    "WS_TAB" : SKIP_TOKEN,
    "CT_SINGLE": SKIP_TOKEN
}