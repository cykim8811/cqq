
#include "quantum.h"
#include "qubit.h"
#include "qubyte.h"
#include "qtransform.h"

#include <iostream>

using namespace std;


int main() {
    QByte<4> qbyte1(7);
    QByte<4> qbyte2(3);

    QByte<4> qbyte = qbyte1 + qbyte2;

    cout << (int) qbyte << endl;
    
    return 0;
}

