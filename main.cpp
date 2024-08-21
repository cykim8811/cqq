
#include "qbit.h"
#include "qgate.h"
#include <iostream>
#include <memory>

using namespace std;

unique_ptr<Qgate> x(QMemory* qm, Qubit& q) {
    unique_ptr<Qgate> gate = make_unique<XGate>(qm, q.index);
    return gate;
}

unique_ptr<Qgate> cx(QMemory* qm, Qubit& control, Qubit& target) {
    unique_ptr<Qgate> gate = make_unique<CXGate>(qm, control.index, target.index);
    return gate;
}


void test(QMemory *qm, Qubit a, Qubit b) {
    x(qm, a);
    cx(qm, a, b);
    x(qm, a);
    qm->_display();
}

int main() {
    QMemory *qm = new QMemory(2);

    Qubit a(qm, 0);
    Qubit b(qm, 1);

    
    test(qm, a, b);


    qm->_display();

    delete qm;
    return 0;
}

