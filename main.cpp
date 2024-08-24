
#include "quantum.h"
#include "qubit.h"
#include "qubyte.h"
#include "qtransform.h"

#include <iostream>

using namespace std;

qint4_t add(qint4_t &a, qint4_t &b) {
    return a + b;
}

int main() {
    qint4_t qbyte1(0);
    qbyte1 = h(qbyte1);
    qint4_t qbyte2(3);

    qint4_t qbyte = add(qbyte1, qbyte2);

    cout << (int) qbyte1 << " " << (int) qbyte2 << " " << (int) qbyte << endl;
    
    return 0;
}

