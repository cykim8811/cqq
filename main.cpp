
#include "quantum.h"
#include "qubyte.h"

#include <memory>
#include <iostream>

using namespace std;

qbool_t oracle(qbool_t &x) {
    return x & (~x);
}

int main() {
    // Grover's algorithm

    cout << "Grover's algorithm" << endl;

    qbool_t a = h<qbool_t>(0);

    qbool_t b = oracle(a);

    b.transform->data->_display();
    
    return 0;
}

