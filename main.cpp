
#include <iostream>
#include "qbyte.h"

using namespace std;

void test(qint4_t &a) { // Segmentation fault
    if (true) {
        qint4_t b = a + 2;
        qint4_t c = b + 3;
        cout << 1 << endl;
        c -= a + 5;
        cout << 2 << endl;
    }
}

int main() {
    qint4_t a = 0;
    a.h();

    qint4_t b = a;

    b ^= a;

    a.h();

    cout << "a: " << (int)a << endl;


    return 0;
}

