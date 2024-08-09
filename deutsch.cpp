
#include <iostream>
#include "quantum.h"

using namespace std;

void oracle1(Qubit &q1, Qubit &q2) {    // constant
    return;
}

void oracle2(Qubit &q1, Qubit &q2) {    // balanced
    q2 ^= q1;
    return;
}

void oracle3(Qubit &q1, Qubit &q2) {    // constant
    !q2;
    return;
}

void oracle4(Qubit &q1, Qubit &q2) {    // balanced
    !q1;
    q2 ^= q1;
    !q1;
    return;
}


int main() {

    Qubit q1(0);
    Qubit q2(1);

    q1.h();
    q2.h();
    oracle1(q1, q2);
    q1.h();
    if (q1.measure()) cout << "oracle1 is balanced" << endl;
    else cout << "oracle1 is constant" << endl;

    q1 = 0;
    q2 = 1;
    q1.h();
    q2.h();
    oracle2(q1, q2);
    q1.h();
    if (q1.measure()) cout << "oracle2 is balanced" << endl;
    else cout << "oracle2 is constant" << endl;

    q1 = 0;
    q2 = 1;
    q1.h();
    q2.h();
    oracle3(q1, q2);
    q1.h();
    if (q1.measure()) cout << "oracle3 is balanced" << endl;
    else cout << "oracle3 is constant" << endl;

    q1 = 0;
    q2 = 1;
    q1.h();
    q2.h();
    oracle4(q1, q2);
    q1.h();
    if (q1.measure()) cout << "oracle4 is balanced" << endl;
    else cout << "oracle4 is constant" << endl;

    

    return 0;
}
