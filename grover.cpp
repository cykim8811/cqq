#include <iostream>
#include <vector>
#include <cmath>
#include "quantum/quantum.h"

using namespace std;

void oracle(vector<Qubit*>& qubits) {
    vector<Qubit*> controls = {qubits[0], qubits[1], qubits[2]};
    qubits[2]->x();
    qubits[3]->mcx(controls);
    qubits[2]->x();
}

void diffusion(vector<Qubit*>& qubits) {
    vector<Qubit*> qubits_view(qubits.begin(), qubits.begin() + 3);
    for (auto& qubit : qubits_view) {
        qubit->h();
    }
    for (auto& qubit : qubits_view) {
        qubit->x();
    }
    qubits_view[2]->mcz({qubits_view[0], qubits_view[1]});
    for (auto& qubit : qubits_view) {
        qubit->x();
    }
    for (auto& qubit : qubits_view) {
        qubit->h();
    }
}

int main() {
    const int n_qubits = 3;
    const int n_iterations = floor(M_PI/4 * sqrt(pow(2, n_qubits)));
    
    vector<Qubit*> qubits;
    for (int i = 0; i < n_qubits; i++) {
        qubits.push_back(new Qubit(false));
    }
    
    qubits.push_back(new Qubit(true));
    
    for (auto& qubit : qubits) {
        qubit->h();
    }
    
    for (int i = 0; i < n_iterations; i++) {
        oracle(qubits);
        diffusion(qubits);
    }
    
    cout << "Measured state: ";
    for (int i = 0; i < n_qubits; i++) {
        cout << qubits[i]->measure();
    }
    cout << endl;
    
    for (auto& qubit : qubits) {
        delete qubit;
    }
    
    return 0;
}
