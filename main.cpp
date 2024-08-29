
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

int main() {
    // cout << "Testing Deutsch's algorithm" << endl;

    // vector<function<qbool_t(const qbool_t&)>> oracles = {oracle1, oracle2, oracle3, oracle4};

    // for (auto oracle: oracles) {
    //     qbool_t a = 0;
    //     z(oracle(h(a)));
    //     bool result = (int) a;
    //     if (result) {
    //         cout << "Oracle is balanced" << endl;
    //     } else {
    //         cout << "Oracle is constant" << endl;
    //     }
    // }

    qbool_t a = 0;
    cout << 1 << endl;
    qbool_t r = a & a;
    cout << 2 << endl;
    a = 0;
    cout << 2 << endl;
    qbool_t s = ~r;
    cout << 3 << endl;
    s = 0;
    cout << 4 << endl;

    
    return 0;
}

