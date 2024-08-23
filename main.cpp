
#include "quantum.h"
#include <iostream>

int main() {
    Qubit q0;
    Qubit q1;

    QState::merge({q0.state, q1.state});

    q0._print();
}

