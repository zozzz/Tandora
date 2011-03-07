class Action:
    PAD_LENGTH = 8

    def __init__(self, number):
        self.number = number

    def asHex(self):
        r = "0x%0." + str(Action.PAD_LENGTH) + "X"
        return r % (self.number)


class ActionMatchPrevToken(Action):
    def __init__(self):
        pass

class ActionNotMatchPrevToken(ActionMatchPrevToken):
    def __init__(self):
        pass

class ActionCharAt(Action):
    def __init__(self, chngType=None):
        pass

    def addTest(self, pos, ch):
        pass

class ActionNotCharAt(ActionCharAt):
    def __init__(self):
        pass

class ActionClose(Action):
    def __init__(self, current=False, endOfsset=0, chngType=None):
        self.id = 0

class ActionChangeTokenType(Action):
    def __init__(self, chngType):
        pass

class ActionIncrementLN(Action):
    def __init__(self):
        pass

class ActionContinue(Action):
    def __init__(self):
        pass

class ActionError(Action):
    def __init__(self):
        pass