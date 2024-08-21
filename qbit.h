
#pragma once

#include <vector>
#include <memory>
#include "qgate.h"
#include "qmemory.h"


class Qubit {
public:
    QMemory* qm;
    int index;
    Qubit(QMemory *qm, int index): qm(qm), index(index) {}
    Qubit(const Qubit& q): qm(q.qm), index(q.index) {}

    ~Qubit() {}

};



