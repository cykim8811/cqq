
#include "qbit.h"
#include "qgate.h"
#include <iostream>
#include <memory>

using namespace std;

void x(QMemory* qm, Qubit& q) {
    q.context->gates.push_back(make_unique<XGate>(qm, q.index));
}

void cx(QMemory* qm, Qubit& control, Qubit& target) {
    if (control.context == target.context) {
        control.context->gates.push_back(make_unique<CXGate>(qm, control.index, target.index));
        return;
    }
    shared_ptr<Context> new_context = make_shared<Context>();
    for (auto& gate : control.context->gates) {
        new_context->gates.push_back(move(gate));
    }
    control.context->gates.clear();
    for (auto& gate : target.context->gates) {
        new_context->gates.push_back(move(gate));
    }
    target.context->gates.clear();
    new_context->gates.push_back(make_unique<CXGate>(qm, control.index, target.index));
    control.context = new_context;
    target.context = new_context;
}


int main() {
    QMemory qm(3);

    Qubit a(&qm, 0);
    Qubit b(&qm, 1);
    Qubit c(&qm, 2);

    

    qm._display();

    return 0;
}

