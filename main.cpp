
#include "quantum.h"
#include "qubit.h"
#include "qubyte.h"
#include "qtransform.h"

#include <iostream>

using namespace std;

qbool_t oracle(qbool_t &x, qbool_t &y) {
    return x & y;
}

int main() {
    // Grover's algorithm

    cout << "Grover's algorithm" << endl;

    qbool_t a = h<qbool_t>(0);
    qbool_t b = h<qbool_t>(0);

    qbool_t res2 = z(oracle(a, b));

    a.transform->data->_display();

    res2.transform->data->_display();

    
    return 0;
}

