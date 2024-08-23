
#pragma once

#include <vector>
#include <memory>
#include "qgate.h"
#include "qmemory.h"


class Context {
public:
    vector<unique_ptr<Qgate>> gates;
    ~Context() {
        for (int i=gates.size()-1; i>=0; i--) {
            gates[i].reset();
        }
        gates.clear();
    };
};

class Qubit {
public:
    QMemory* qm;
    int index;
    shared_ptr<Context> context;
    Qubit(QMemory *qm, int index): qm(qm), index(index), context(make_shared<Context>()) {
    }
    Qubit(const Qubit& q): qm(q.qm), index(q.index), context(make_shared<Context>()) {
    }

    ~Qubit() {}

};

