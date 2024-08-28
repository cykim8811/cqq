
#include "quantum.h"
#include "qubyte.h"

#include <memory>
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

    qbool_t c = z(oracle(a, b));

    cout << "b.size(): " << b.transform->data->qbits.size() << endl;

    
    return 0;
}

