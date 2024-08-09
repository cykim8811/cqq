
#include <algorithm>
#include <random>
#include <vector>
#include <iostream>

#include "quantum.h"

#define DEBUG

using namespace std;


Qubit::Qubit() {
    state = new QubitState(this);
}

Qubit::Qubit(bool value) {
    state = new QubitState(this);
    if (value) {
        state->x(this);
    }
}

Qubit::~Qubit() {
    state->remove(this);
}


bool Qubit::measure() {
    return state->measure(this);
}

void Qubit::h() {
    state->h(this);
}

void Qubit::x() {
    state->x(this);
}

void Qubit::cx(Qubit &control) {
    state->cx(this, &control);
}

void Qubit::mcx(vector<Qubit*> controls) {
    state->mcx(controls, this);
}

void Qubit::mcz(vector<Qubit*> controls) {
    state->mcz(controls, this);
}

void Qubit::operator^=(Qubit &q) {
    state->cx(&q, this);
}

void Qubit::_display() {
    state->_display();
}

Qubit& Qubit::operator!() {
    this->x();
    return *this;
}

Qubit& Qubit::operator=(Qubit &q) {
    bool res = this->measure();
    this->state->amplitudes = {1, 0};
    *this ^= q;
    return *this;
}

Qubit& Qubit::operator=(bool value) {
    bool res = this->measure();
    this->state->amplitudes = {1, 0};
    if (value) {
        this->x();
    }
    return *this;
}

void Qubit::phase(float angle) {
    state->phase(this, angle);
}





QubitState::QubitState(Qubit* qubit): qubits({qubit}), amplitudes({1, 0}) {}

QubitState::~QubitState() {
    #ifdef DEBUG
    for (Qubit* qubit : qubits) {
        if (qubit->state != this) {
            cerr << "QubitState destructor error" << endl;
            // We will fix this error later, by measuring the qubit
            exit(1);
        }
    }
    #endif
}

bool QubitState::measure(Qubit* target) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dis(0, 1);

    if (qubits.size() == 1) {
        bool result = dis(gen) < norm(amplitudes[1]);
        amplitudes = {(complex<float>)!result, (complex<float>)result};
        return result;
    }

    const int tIndex = find(qubits.begin(), qubits.end(), target) - qubits.begin();
    float probability = 0;

    for (int i = 0; i < amplitudes.size(); i++) {
        if ((i >> tIndex) & 1) {
            probability += norm(amplitudes[i]);
        }
    }

    bool result = dis(gen) < probability;

    vector<complex<float>> newAmplitudes(amplitudes.size() / 2);
    
    float norm = 0;
    if (result) {
        norm = sqrt(probability);
    } else {
        norm = sqrt(1 - probability);
    }

    for (int i = 0; i < amplitudes.size() / 2; i++) {
        const int prevIndex = ((i >> tIndex) << (tIndex + 1)) | (i & ((1 << tIndex) - 1)) | (result << tIndex);
        newAmplitudes[i] = amplitudes[prevIndex] / norm;
    }

    amplitudes = newAmplitudes;

    this->qubits.erase(this->qubits.begin() + tIndex);

    target->state = new QubitState(target);
    target->state->amplitudes = {(complex<float>)!result, (complex<float>)result};

    return result;
}

void QubitState::remove(Qubit* qubit) {
    this->measure(qubit);
    delete qubit->state;
}

void QubitState::h(Qubit* target) {
    const int tIndex = find(qubits.begin(), qubits.end(), target) - qubits.begin();
    vector<complex<float>> newAmplitudes(amplitudes.size());
    
    for (int i = 0; i < amplitudes.size(); i++) {
        const int zeroIndex = i & ~(1 << tIndex);
        const int oneIndex = i | (1 << tIndex);
        if ((i >> tIndex) & 1) {
            newAmplitudes[zeroIndex] += amplitudes[oneIndex] / (float)sqrt(2);
            newAmplitudes[oneIndex] -= amplitudes[oneIndex] / (float)sqrt(2);
        } else {
            newAmplitudes[zeroIndex] += amplitudes[zeroIndex] / (float)sqrt(2);
            newAmplitudes[oneIndex] += amplitudes[zeroIndex] / (float)sqrt(2);
        }
    }

    amplitudes = newAmplitudes;

}

void QubitState::x(Qubit* target) {
    const int tIndex = find(qubits.begin(), qubits.end(), target) - qubits.begin();
    vector<complex<float>> newAmplitudes(amplitudes.size());
    
    for (int i = 0; i < amplitudes.size(); i++) {
        const int resultIndex = i ^ (1 << tIndex);
        newAmplitudes[resultIndex] = amplitudes[i];
    }

    amplitudes = newAmplitudes;
}

void QubitState::entangle(Qubit* qubit) {
    const int qIndex = find(qubits.begin(), qubits.end(), qubit) - qubits.begin();
    if (qIndex == qubits.size()) {
        this->product(qubit->state);
    }
}

void QubitState::entangle(vector<Qubit*> qubits) {
    for (Qubit* qubit : qubits) {
        this->entangle(qubit);
    }
}

void QubitState::cx(Qubit* control, Qubit* target) {
    const int cIndex = find(qubits.begin(), qubits.end(), control) - qubits.begin();
    const int tIndex = find(qubits.begin(), qubits.end(), target) - qubits.begin();
    
    if (cIndex == qubits.size()) {
        this->product(control->state);
    }

    vector<complex<float>> newAmplitudes(amplitudes.size());
    
    for (int i = 0; i < amplitudes.size(); i++) {
        if ((i >> cIndex) & 1) {
            const int resultIndex = i ^ (1 << tIndex);
            newAmplitudes[resultIndex] = amplitudes[i];
        } else {
            newAmplitudes[i] = amplitudes[i];
        }
    }

    amplitudes = newAmplitudes;
}

void QubitState::mcx(vector<Qubit*> controls, Qubit* target) {
    this->entangle(controls);
    vector<int> cIndices(controls.size(), -1);
    for (int i = 0; i < controls.size(); i++) {
        cIndices[i] = find(qubits.begin(), qubits.end(), controls[i]) - qubits.begin();
    }
    const int tIndex = find(qubits.begin(), qubits.end(), target) - qubits.begin();

    vector<complex<float>> newAmplitudes(amplitudes.size());

    for (int i = 0; i < amplitudes.size(); i++) {
        bool flag = true;
        for (int cIndex : cIndices) {
            if (!((i >> cIndex) & 1)) {
                flag = false;
                break;
            }
        }
        if (flag) {
            const int resultIndex = i ^ (1 << tIndex);
            newAmplitudes[resultIndex] = amplitudes[i];
        } else {
            newAmplitudes[i] = amplitudes[i];
        }
    }

    amplitudes = newAmplitudes;
}

void QubitState::mcz(vector<Qubit*> controls, Qubit* target) {
    this->entangle(controls);
    vector<int> cIndices(controls.size(), -1);
    for (int i = 0; i < controls.size(); i++) {
        cIndices[i] = find(qubits.begin(), qubits.end(), controls[i]) - qubits.begin();
    }
    const int tIndex = find(qubits.begin(), qubits.end(), target) - qubits.begin();

    vector<complex<float>> newAmplitudes(amplitudes.size());

    for (int i = 0; i < amplitudes.size(); i++) {
        bool flag = true;
        for (int cIndex : cIndices) {
            if (!((i >> cIndex) & 1)) {
                flag = false;
                break;
            }
        }
        if (flag && (i >> tIndex) & 1) {
            newAmplitudes[i] = -amplitudes[i];
        } else {
            newAmplitudes[i] = amplitudes[i];
        }
    }

    amplitudes = newAmplitudes;
}

void QubitState::phase(Qubit* target, float angle) {
    const int tIndex = find(qubits.begin(), qubits.end(), target) - qubits.begin();
    
    for (int i = 0; i < amplitudes.size(); i++) {
        if ((i >> tIndex) & 1) {
            amplitudes[i] *= exp(complex<float>(0, angle));
        }
    }
}

void QubitState::product(QubitState* other) {
    vector<complex<float>> newAmplitudes(amplitudes.size() * other->amplitudes.size());
    
    for (int j = 0; j < other->amplitudes.size(); j++) {
        for (int i = 0; i < amplitudes.size(); i++) {
            newAmplitudes[j * amplitudes.size() + i] = amplitudes[i] * other->amplitudes[j];
        }
    }

    amplitudes = newAmplitudes;
    for (Qubit* qubit : other->qubits) {
        qubits.push_back(qubit);
        qubit->state = this;
    }
    #ifdef DEBUG
    other->qubits.clear();
    #endif
    delete other;
}

void QubitState::_display() {
    for (int i = 0; i < amplitudes.size(); i++) {
        cout << amplitudes[i] << " ";
    }
    cout << endl;
}
