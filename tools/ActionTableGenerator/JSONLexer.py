# -*- coding: utf-8 -*-
'''
@author zozzz
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
    "STRING" : Token("(?P<begin>'|\")(?P<middle>.*?)(?P<end>(?P=begin))", Token.NEED_CONTENT, ["\\"]),
    "INT"    : Token("(?P<begin>(\-|\+)?[1-9])(?P<middle>[0-9]*)", Token.NEED_CONTENT),
    "FLOAT"  : Token("(?P<begin>[0-9]|\.)(?P<middle>([0-9.]+(e(\+|\-)?[0-9]+)?))", Token.IGNORE_CASE | Token.NEED_CONTENT),
    "HEX"    : Token("(?P<begin>0x)(?P<middle>[0-9abcdef]*)", Token.IGNORE_CASE | Token.NEED_CONTENT)
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