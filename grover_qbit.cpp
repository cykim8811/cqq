#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <functional>
#include <iomanip>
#include <map>
#include "qbyte.h"

using namespace std;

using OracleFunction = function<void(const vector<unique_ptr<qbit>>&, qbit*)>;

void oracle(const vector<unique_ptr<qbit>>& qubits, qbit* output, const vector<bool>& marked_state) {
    for (size_t i = 0; i < qubits.size(); ++i) {
        if (!marked_state[i]) {
            qgate::x(qubits[i].get());
        }
    }

    vector<qbit*> controls;
    for (const auto& qubit : qubits) {
        controls.push_back(qubit.get());
    }
    qgate::mcx(controls, output);

    for (size_t i = 0; i < qubits.size(); ++i) {
        if (!marked_state[i]) {
            qgate::x(qubits[i].get());
        }
    }
}

void diffusion(vector<unique_ptr<qbit>>& qubits) {
    for (auto& q : qubits) {
        qgate::h(q.get());
    }

    for (auto& q : qubits) {
        qgate::x(q.get());
    }

    vector<qbit*> controls;
    for (int i = 0; i < qubits.size() - 1; i++) {
        controls.push_back(qubits[i].get());
    }
    qgate::mcz(controls, qubits.back().get());

    for (auto& q : qubits) {
        qgate::x(q.get());
    }

    for (auto& q : qubits) {
        qgate::h(q.get());
    }
}

map<string, int> grover_algorithm(int num_qubits, const vector<bool>& marked_state, int iterations, int num_shots) {
    map<string, int> results;

    for (int shot = 0; shot < num_shots; ++shot) {
        vector<unique_ptr<qbit>> qubits;
        for (int i = 0; i < num_qubits; i++) {
            qubits.push_back(make_unique<qbit>(false));
        }
        
        unique_ptr<qbit> output = make_unique<qbit>(false);
        qgate::x(output.get());
        qgate::h(output.get());

        for (auto& q : qubits) {
            qgate::h(q.get());
        }

        OracleFunction oracle_func = [&marked_state](const vector<unique_ptr<qbit>>& qubits, qbit* output) {
            oracle(qubits, output, marked_state);
        };

        for (int i = 0; i < iterations; i++) {
            oracle_func(qubits, output.get());
            diffusion(qubits);
        }

        string measured_state;
        for (int i = 0; i < num_qubits; i++) {
            measured_state += to_string(qgate::measure(qubits[i].get()));
        }
        results[measured_state]++;
    }

    return results;
}

int main() {
    int num_qubits = 8;
    vector<bool> marked_state = {1, 1, 0, 1, 0, 0, 1, 0};
    int iterations = round(M_PI / 4.0 * sqrt(pow(2, num_qubits)));
    int num_shots = 1000;

    cout << "Grover's algorithm" << endl;

    cout << "Number of qubits: " << num_qubits << endl;
    cout << "Marked state: ";
    for (const auto& state : marked_state) {
        cout << state;
    }
    cout << endl;

    auto results = grover_algorithm(num_qubits, marked_state, iterations, num_shots);

    for (const auto& result : results) {
        cout << "|" << result.first << "> : " << setw(4) << result.second 
             << " (" << fixed << setprecision(1) << (100.0 * result.second / num_shots) << "%)" << endl;
    }

    return 0;
}