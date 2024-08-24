
#include "quantum.h"
#include "qubit.h"
#include "qtransform.h"

#include <iostream>

using namespace std;


int main() {
    int n = 4;

    vector<Qubit*> qubits;
    for (int i = 0; i < n; i++) {
        qubits.push_back(new Qubit());
    }

    Qubit carry;
    carry = ~carry;

    Qubit new_carry;

    for (int i = 0; i < n; i++) {
        new_carry = carry & *qubits[i];
        *qubits[i] = *qubits[i] ^ carry;
        carry = new_carry;
    }

    for (int i = 0; i < n; i++) {
        cout << (int)*qubits[n - i - 1];
    }

    cout << endl;

    return 0;
}

