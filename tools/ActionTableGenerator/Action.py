# ==========================================================================
# Action
# ==========================================================================
class Action:
    PAD_LENGTH = 8
    ACTIONS = [
        "ERROR",
        "CONTINUE",
        "CLOSE",
        "CHAR_AT",
        "CHNG_TYPE"
    ]

    @classmethod
    def id(cls, name):
        for (i, a) in enumerate(cls.ACTIONS):
            if a == name:
                return i
        raise Exception("Undefined action name: " + name)

    def __init__(self, id, token=None):
        self.id = id
        self.token = token

        if token is not None:
            self.tid = token.id()
        else:
            self.tid = 0

    def asHex(self):
        r = "0x%0." + str(Action.PAD_LENGTH) + "X"
        return r % (self._getNumber())

    def _getNumber(self):
        return self.id

    def __str__(self):
        return "<action>"



# ==========================================================================
# Match prev token
# ==========================================================================
class ActionMatchPrevToken(Action):
    def __init__(self):
        Action.__init__(self, 0)

    def __str__(self):
        return "<matchPrevToken>"



# ==========================================================================
#
# ==========================================================================
class ActionNotMatchPrevToken(ActionMatchPrevToken):
    def __init__(self):
        ActionMatchPrevToken.__init__(self, 0)

    def __str__(self):
        return "<notMatchPrevToken>"



# ==========================================================================
# CHAR_AT
# ==========================================================================
class ActionCharAt(Action):
    MAX_POS = 15
    MAX_POS_NUMBER = 5

    def __init__(self, token, pos=None):
        self.posList = []

        if pos is not None:
            self.addPosition(pos)

        Action.__init__(self, Action.id("CHAR_AT"), token)

    def addPosition(self, pos):
        if len(self.posList) == self.MAX_POS_NUMBER:
            raise Exception("Too many positions!")

        if pos > self.MAX_POS:
            raise Exception("Position is too large "+str(pos)+"!")

        self.posList.append(pos)

    def _getNumber(self):
        ret = self.id
        lsh = 1
        for pos in self.posList:
            ret |= pos << lsh
            lsh += 1

        ret |= self.tid << 24
        return ret

    def __str__(self):
        return "<charAt "+str(self.tid)+", "+("|".join([str(p) for p in self.posList]))+">"



# ==========================================================================
#
# ==========================================================================
class ActionNotCharAt(ActionCharAt):
    def __init__(self):
        ActionCharAt.__init__(self, 0)

    def __str__(self):
        return "<notCharAt>"



# ==========================================================================
#
# ==========================================================================
class ActionClose(Action):
    def __init__(self, endOfsset=0, token=None):
        Action.__init__(self, Action.id("CLOSE"), token)

        self.endOfsset = endOfsset
        if abs(endOfsset) > 15:
            raise Exception("Max allowed offset size is 15!")

    def _getNumber(self):
        ret = self.id
        ret |= self.endOfsset << 4
        ret |= self.tid <<8
        return ret

    def __str__(self):
        return "<close "+str(self.endOfsset)+", "+str(self.tid)+">"



# ==========================================================================
#
# ==========================================================================
class ActionChangeTokenType(Action):
    def __init__(self, token):
        Action.__init__(self, Action.id("CHNG_TYPE"), token)

    def _getNumber(self):
        ret = self.id
        ret |= self.tid << 4
        return ret

    def __str__(self):
        return "<chngType "+str(self.tid)+">"



# ==========================================================================
#
# ==========================================================================
class ActionIncrementLN(Action):
    def __init__(self):
        Action.__init__(self, 0)

    def __str__(self):
        return "<incLN>"



# ==========================================================================
#
# ==========================================================================
class ActionContinue(Action):
    def __init__(self):
        Action.__init__(self, Action.id("CONTINUE"))

    def __str__(self):
        return "<continue>"



# ==========================================================================
#
# ==========================================================================
class ActionError(Action):
    def __init__(self):
        Action.__init__(self, Action.id("ERROR"))

    def __str__(self):
        return "<error>"