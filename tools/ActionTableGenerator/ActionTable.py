# -*- coding: utf-8 -*-
'''
@author zozzz
'''

__all__ = ["ActionTable"]

from Lexer import *
import os, sys
import os.path

class ActionTable:

    INDENT = "\t"
    EOL = "\n"


    def __init__(self, namespace, module, outFile, force=False):
        self.namespace = namespace
        self.lexer = Lexer(module)
        self.outFile = outFile
        self.moduleFile = module.__file__
        self._force = force
        self.actionTable = {}

        self.actionTable[self.lexer.default.name] = range(Lexer.ASCII_MIN, Lexer.ASCII_MAX)

    def create(self):
        moduleFInf = os.stat(self.moduleFile)

        try:
            outFInf = os.stat(self.outFile+".h")
        except:
            self._create()
            return

        if self._force is True or moduleFInf.st_mtime > outFInf.st_mtime:
            self._create()
        else:
            print "Module not changed: " + self.lexer._module.__name__

    def _create(self):
        print "Generate " + self.outFile + ".h ...",
        #try:
        f = open(self.outFile+".h", "w")
        f.write("// AUTO GENERATED CODE DO NOT EDIT!!!\n\n");

        f.write("#ifndef _" + os.path.basename(self.outFile).upper() + "_" + self.EOL)
        f.write("#define _" + os.path.basename(self.outFile).upper() + "_" + self.EOL + self.EOL)

        ns = self.namespace.split(".")

        for x in ns:
            f.write("namespace " + x + " { ")
        f.write(self.EOL + self.EOL)

        f.write(self._createEnums(1) + self.EOL + self.EOL)
        f.write(self._createActionTable(1) + self.EOL)

        f.write(self.EOL + self.EOL)

        for x in ns:
            f.write("} /* "+ x +" */" + self.EOL)

        f.write(self.EOL)

        f.write("#endif")
        f.close()
        #except:
            #print "[ERROR]", sys.exc_info()[1]
            #return
        print "[OK]"


    # ==========================================================================
    # GENERATOR FUNCTIONS
    # ==========================================================================
    def _createEnums(self, indent):
        ret = [
            (indent * self.INDENT) + "enum TokenType",
            (indent * self.INDENT) + "{"
        ]

        _group = None
        for (g, n) in self.lexer._tokensByGroup:
            if _group != g:
                _group = g
                (r_min, r_max) = self.lexer.tokenRange(g)
                ret.append( self.EOL + ((indent+1) * self.INDENT) + "# " + g + " ("+str(r_min)+" - "+str(r_max)+")")

            ret.append(((indent+1) * self.INDENT) + n)

        ret.append((indent * self.INDENT) + "};")

        return self.EOL.join(ret)

    def _createActionTable(self, indent):
        self._fillActionTable()

        ret = [
            (indent * self.INDENT) + "static const int actionTable["+str(len(self.lexer._tokensByGroup))+"]["+str(Lexer.ASCII_MAX)+"] = {"
        ]

        ret.append((indent * self.INDENT) + "};")

        return self.EOL.join(ret)

    def _fillActionTable(self):
        for token in self.lexer._ordered[1:]:
            self.actionTable[token.name] = []
            for _x in range(Lexer.ASCII_MIN, Lexer.ASCII_MAX):
                self.actionTable[token.name].append(0)
            self._determineActionsFor(token)


    """
    ch: 1 (chngType(int))
    |-> (default->int)
        |-> ch: 2 (OK)
        |   |-> int
        |-> ch: . (chngType(op))
            |-> (int->op)
                |-> ch: 2 (chngType(float))
                |   |-> (op->float)
                |-> ch: x
                |   |-> (op(close)->identifier)

    ch: c
    |-> (default->case)
        |-> ch: a (charAt(1))
        |   |-> case
        |->ch: l (chngType(identifier))
            |-> (case->identifier)
    """

    def _determineActionsFor(self, token):

        infinity = token.maxWidth() >= 65535
        subGroup = None

        if infinity and token.begin.maxWidth() < 65535:
            subGroup = "begin"
            ml = token.begin.maxWidth()
        else:
            ml = token.maxWidth()

        variants = []
        for pos in range(0, ml+1):
            vSnapShot = variants[:]
            for char in range(Lexer.ASCII_MIN, Lexer.ASCII_MAX):
                #print vSnapShot
                alterTokens = []
                if pos > 0:
                    self._findAlternativeTokens(token, vSnapShot, char, alterTokens);

                if self._tokenIsMatch(token, vSnapShot, char, subGroup):
                    if len(variants) <= pos:
                        variants.append([])
                    variants[pos].append(char)


                if len(alterTokens):
                    print "alt["+token.name+":"+str(pos)+":"+chr(char)+"]: ", alterTokens

        #print token.name, variants

    def _tokenIsMatch(self, tok, chars, ch, subGroup=None):
        chr = chars[:]
        chr.append([ch])
        return tok.test(chr, subGroup)

    def _findAlternativeTokens(self, token, chars, ch, result):
        def _tok(tn):
            return self.lexer._tokens[tn]

        for altToken in self.lexer._ordered:
            if altToken.name != token.name and self._tokenIsMatch(altToken, chars, ch) and result.count(altToken.name) == 0:
                insertPos = 0

                for tn in result:
                    if _tok(tn).exact and altToken.exact:
                        if _tok(tn).maxWidth() <= altToken.maxWidth():
                            insertPos += 1
                        else:
                            break;
                    elif _tok(tn).exact and altToken.exact is False:
                        if _tok(tn).exact:
                            insertPos += 1
                        else:
                            break

                result.insert(insertPos, altToken.name)
