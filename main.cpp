
#include <iostream>
#include <memory>
#include "qbyte.h"
#include "qcircuit.h"

#include <time.h>

using namespace std;

QCircuit fibonacci(int a, int b, int n, int count, int anc1_idx, int anc2_idx, int cond_idx) {
    QCircuit circuit;
    for (int i=0; i<n; i++) {
        circuit.add_gate(make_unique<QCircuit>(add(a, a+count, b, b+count, anc1_idx, anc2_idx, cond_idx)));
        circuit.add_gate(make_unique<QCircuit>(swap(a, b, count, cond_idx)));
    }
    return circuit;
}


int main() {
    cout << "Running main.cpp" << endl;

    clock_t start, end;
    double cpu_time_used;


    qbyte_t a(0);
    qbyte_t b(1);
    int n = 10;

    vector<qbit*> qbits;

    qbit anc1(false);
    qbit anc2(false);
    qbit cond(true);
    
    qbits.insert(qbits.end(), a.qbits.begin(), a.qbits.end());
    qbits.insert(qbits.end(), b.qbits.begin(), b.qbits.end());
    qbits.push_back(&anc1);
    qbits.push_back(&anc2);
    qbits.push_back(&cond);

    QCircuit circuit = fibonacci(0, 8, n, 8, 16, 17, 18);

    start = clock();
    circuit.forward(qbits);
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    cout << "Result: " << a << endl;
    cout << "Time taken: " << cpu_time_used << "s" << endl;
    
    return 0;
}

