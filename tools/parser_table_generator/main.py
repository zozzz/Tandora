'''
Created on 2011.03.01.

@author: Zozzz
'''

import Lexer

if __name__ == '__main__':
    print Lexer._expParser("[^0-1]")
    print Lexer._expParser("[0-1]")
    print Lexer._expParser("[a-z]")
    print Lexer._expParser("catch")