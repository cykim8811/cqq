
#include "immut.h"
#include <iostream>


class Op {
public:
    var * value;
    virtual ~Op() {}
};

class addiOp: public Op {
public:
    Op *a;
    int b;
    addiOp(Op *a, int bval) {
        value = new var();

        this->a = a;
        b = bval;

        value->value ^= a->value->value;
        value->value += b;
    }

    ~addiOp() {
        value->value -= b;
        value->value ^= a->value->value;
    }
};

class zeroOp: public Op {
public:
    zeroOp() {
        value = new var();
    }
    ~zeroOp() {
        delete value;
    }
};

class addOp: public Op {
public:
    Op *a;
    Op *b;
    addOp(Op *a, Op *b) {
        value = new var();
        this->a = a;
        this->b = b;
        value->value ^= a->value->value;
        value->value += b->value->value;
    }
    ~addOp() {
        value->value -= b->value->value;
        value->value ^= a->value->value;
    }
};

addOp test() {
    zeroOp o0 = zeroOp();
    addiOp o1 = addiOp(&o0, 3);
    zeroOp o2 = zeroOp();
    addiOp o3 = addiOp(&o2, 5);
    addOp o4 = addOp(&o1, &o3);
    return o4;
}

int main() {
    addOp o5 = test();
    addiOp o6 = addiOp(&o5, 7);
    cout << o6.value->value << endl;
    return 0;
}



