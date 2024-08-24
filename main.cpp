
#include "quantum.h"
#include "qubit.h"
#include "qtransform.h"

#include <iostream>

using namespace std;

int main() {
    Qubit q0 = Qubit::h(Qubit());

    Qubit q1 = !q0;

    cout << (int)q1 << endl;
    cout << (int)q0 << endl;
}

