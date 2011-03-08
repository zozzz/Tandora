class Action:
    PAD_LENGTH = 8

    def __init__(self, number):
        self.number = number

    def asHex(self):
        r = "0x%0." + str(Action.PAD_LENGTH) + "X"
        return r % (self._getNumber())

    def _getNumber(self):
        return self.number

    def __str__(self):
        return "<Action>"


class ActionMatchPrevToken(Action):
    def __init__(self):
        Action.__init__(self, 0)


    def __str__(self):
        return "<ActionMatchPrevToken>"

class ActionNotMatchPrevToken(ActionMatchPrevToken):
    def __init__(self):
        ActionMatchPrevToken.__init__(self, 0)


    def __str__(self):
        return "<ActionNotMatchPrevToken>"

class ActionCharAt(Action):
    def __init__(self, chngType, pos=None):
        Action.__init__(self, 0)

    def addPosition(self, pos):
        pass

    def __str__(self):
        return "<ActionCharAt>"

class ActionNotCharAt(ActionCharAt):
    def __init__(self):
        ActionCharAt.__init__(self, 0)

    def __str__(self):
        return "<ActionNotCharAt>"

class ActionClose(Action):
    def __init__(self, current=False, endOfsset=0, chngType=None):
        Action.__init__(self, 0)

    def __str__(self):
        return "<ActionClose>"

class ActionChangeTokenType(Action):
    def __init__(self, chngType):
        Action.__init__(self, 0)

    def __str__(self):
        return "<ActionChangeTokenType>"

class ActionIncrementLN(Action):
    def __init__(self):
        Action.__init__(self, 0)

    def __str__(self):
        return "<ActionIncrementLN>"

class ActionContinue(Action):
    def __init__(self):
        Action.__init__(self, 0)

    def __str__(self):
        return "<ActionContinue>"

class ActionError(Action):
    def __init__(self):
        Action.__init__(self, 0)

    def __str__(self):
        return "<ActionError>"