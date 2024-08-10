
#include <iostream>
#include <chrono>
#include "qbyte.h"

using namespace std;
using namespace qgate;


int main() {
    auto start = chrono::high_resolution_clock::now();

    qbyte_t q1(0);
    qbyte_t q2(10);

    q1.h();

    q2 += q1;

    cout << (int)q1 << endl;
    cout << (int)q2 << endl;

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Time taken: " << duration.count() / 1000.0 << " ms" << endl;

    return 0;
}

