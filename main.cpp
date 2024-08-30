
#include "quantum.h"
#include "qubyte.h"

#include <vector>
#include <functional>
#include <memory>
#include <iostream>

using namespace std;

qbool_t oracle1(const qbool_t &x) {
    return x;
}

qbool_t oracle2(const qbool_t &x) {
    return ~x;
}

qbool_t oracle3(const qbool_t &x) {
    return x & ~x;
}

qbool_t oracle4(const qbool_t &x) {
    return x | ~x;
}


qbool_t oracle_grover(const qbool_t &a, const qbool_t &b) {
    return ~a & b;
}

int main() {
    cout << "Testing Deutsch's algorithm" << endl;

    vector<function<qbool_t(const qbool_t&)>> oracles = {oracle1, oracle2, oracle3, oracle4};

    for (auto oracle: oracles) {
        qbool_t a = 0;
        z(oracle(h(a)));
        bool result = (int) a;
        if (result) {
            cout << "Oracle is balanced" << endl;
        } else {
            cout << "Oracle is constant" << endl;
        }
    }

    cout << endl;

    cout << "Testing Grover's algorithm" << endl;

    qbool_t a = h<qbool_t>(0);
    qbool_t b = h<qbool_t>(0);
    z(oracle_grover(a, b));
    z(~h(a) & ~h(b));
    cout << (int) a << (int) b << endl;
    
    return 0;
}

