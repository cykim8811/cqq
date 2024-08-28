
#include "quantum.h"
#include "qubyte.h"

#include <memory>
#include <iostream>

using namespace std;

qbool_t oracle(const qbool_t &x) {
    return ~x;
}

int main() {
    // Grover's algorithm

    cout << "Grover's algorithm" << endl;

    qbool_t a = 0;
    h(a);
    z(oracle(a));
    h(a);

    a.transform->data->_display();
    
    return 0;
}

