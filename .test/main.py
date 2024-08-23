class var:
    def __init__(self):
        self.val = 0

    def __del__(self):
        assert self.val == 0


class Op: pass

class ZeroOp(Op):
    def __init__(self):
        self.value = var()

class AddiOp(Op):
    def __init__(self, target: Op, delta: int):
        self.value = var()
        self.target = target
        self.delta = delta

        self.value.val 
