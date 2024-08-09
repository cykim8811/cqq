
#include <iostream>
#include "qbyte.h"

using namespace std;

int main() {
    qbit q1(false);
    qbit q2(false);
    qbit q3(true);

    qgate::h(&q1);
    qgate::h(&q2);
    qgate::h(&q3);

    qgate::x(&q1);
    qgate::mcx({&q1, &q2}, &q3);
    qgate::x(&q1);

    qgate::h(&q1);
    qgate::h(&q2);
    qgate::h(&q3);

    qgate::x(&q1);
    qgate::x(&q2);

    qgate::mcz({&q1, &q2}, &q3);

    qgate::x(&q1);
    qgate::x(&q2);

    qgate::h(&q1);
    qgate::h(&q2);

    cout << "q1: " << qgate::measure(&q1) << endl;
    cout << "q2: " << qgate::measure(&q2) << endl;

    return 0;
}
