
#include <iostream>
#include <memory>
#include "qbyte.h"
#include "qcircuit.h"

using namespace std;

int main() {

    qbyte_t a(0);
    qbyte_t b(0);


    qgate::x(a.qbits[0]);
    qgate::x(a.qbits[1]);

    qgate::x(a.qbits[4]);

    QCircuit circuit = increment(0, 3, 3, 4);
    circuit.forward(a.qbits);


    qgate::x(a.qbits[4]);

    cout << a << endl;
    
    return 0;
}

