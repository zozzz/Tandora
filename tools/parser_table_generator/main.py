'''
Created on 2011.03.01.

@author: Zozzz
'''

import Lexer
from Generator import *
import JSONLexer

TandoraSRCDir = "d:\\Works\\cpp\\Tandora\\src\\"

if __name__ == '__main__':

    #print Action(10233800).asHex()

    json = Generator(JSONLexer, TandoraSRCDir + "parser\\JSONActionTable.h")
    json.create()