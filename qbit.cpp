
#include "qbit.h"

#include <iostream>
#include <algorithm>
using namespace std;


qbit_state::qbit_state(qbit* qbit, bool init_value): qbits({qbit}) {
    if (init_value) {
        amplitudes = {0, 1};
    } else {
        amplitudes = {1, 0};
    }
}


qbit::qbit(): state(make_shared<qbit_state>(this)), state_index(0) {}
qbit::qbit(bool value): state(make_shared<qbit_state>(this, value)), state_index(0) {}
qbit::~qbit() {}

void qbit::_display() {
    for (int i = 0; i < this->state->amplitudes.size(); i++) {
        cout << this->state->amplitudes[i].real();
        if (this->state->amplitudes[i].imag() > 0) {
            cout << " + " << this->state->amplitudes[i].imag() << "i";
        } else if (this->state->amplitudes[i].imag() < 0) {
            cout << " - " << -this->state->amplitudes[i].imag() << "i";
        }
        cout << " ";
    }
    cout << endl;
}



void qgate::x(qbit* qbit) {
    vector<complex<float>> new_amplitudes(qbit->state->amplitudes.size());
    for (int i = 0; i < qbit->state->amplitudes.size(); i++) {
        const int target = i ^ (1 << qbit->state_index);
        new_amplitudes[target] = qbit->state->amplitudes[i];
    }
    qbit->state->amplitudes = new_amplitudes;
}

void qgate::z(qbit* qbit) {
    vector<complex<float>> new_amplitudes(qbit->state->amplitudes.size());
    for (int i = 0; i < qbit->state->amplitudes.size(); i++) {
        const int target = i;
        if ((i >> qbit->state_index) & 1) {
            new_amplitudes[target] = -qbit->state->amplitudes[i];
        } else {
            new_amplitudes[target] = qbit->state->amplitudes[i];
        }
    }
    qbit->state->amplitudes = new_amplitudes;
}

void qgate::h(qbit* qbit) {
    vector<complex<float>> new_amplitudes(qbit->state->amplitudes.size(), 0);
    for (int i = 0; i < qbit->state->amplitudes.size(); i++) {
        const int zero = i & ~(1 << qbit->state_index);
        const int one = i | (1 << qbit->state_index);
        if ((i >> qbit->state_index) & 1) {
            new_amplitudes[zero] += qbit->state->amplitudes[i] / (float)sqrt(2);
            new_amplitudes[one] -= qbit->state->amplitudes[i] / (float)sqrt(2);
        } else {
            new_amplitudes[zero] += qbit->state->amplitudes[i] / (float)sqrt(2);
            new_amplitudes[one] += qbit->state->amplitudes[i] / (float)sqrt(2);
        }
    }
    qbit->state->amplitudes = new_amplitudes;
}

void qgate::entangle(qbit* qbit1, qbit* qbit2) {
    if (qbit1->state.get() == qbit2->state.get()) {
        return;
    }
    vector<complex<float>> new_amplitudes(qbit1->state->amplitudes.size() * qbit2->state->amplitudes.size());
    for (int i = 0; i < qbit2->state->amplitudes.size(); i++) {
        for (int j = 0; j < qbit1->state->amplitudes.size(); j++) {
            new_amplitudes[i * qbit1->state->amplitudes.size() + j] = qbit1->state->amplitudes[j] * qbit2->state->amplitudes[i];
        }
    }

    
    qbit1->state->amplitudes = new_amplitudes;
    for (qbit* qbit : qbit2->state->qbits) {
        qbit->state_index += qbit1->state->qbits.size();
        qbit->state = qbit1->state;
        qbit1->state->qbits.push_back(qbit);
    }
}

void qgate::entangle(vector<qbit*> qbits) {
    for (int i = 1; i < qbits.size(); i++) {
        entangle(qbits[0], qbits[i]);
    }
}

void qgate::cx(qbit* control, qbit* target) {
    qgate::entangle(control, target);
    vector<complex<float>> new_amplitudes(target->state->amplitudes.size());
    for (int i = 0; i < target->state->amplitudes.size(); i++) {
        const int tInd = i ^ (1 << (target->state_index));
        if ((i >> control->state_index) & 1) {
            new_amplitudes[i] = target->state->amplitudes[tInd];
        } else {
            new_amplitudes[i] = target->state->amplitudes[i];
        }
    }
    target->state->amplitudes = new_amplitudes;
}

void qgate::mcx(vector<qbit*> controls, qbit* target) {
    qgate::entangle(controls);
    qgate::entangle(controls[0], target);
    vector<complex<float>> new_amplitudes(target->state->amplitudes.size());
    for (int i = 0; i < target->state->amplitudes.size(); i++) {
        const int tInd = i ^ (1 << (target->state_index));
        bool controlFlag = true;
        for (qbit* control : controls) {
            if (!((i >> control->state_index) & 1)) {
                controlFlag = false;
                break;
            }
        }
        if (controlFlag) {
            new_amplitudes[i] = target->state->amplitudes[tInd];
        } else {
            new_amplitudes[i] = target->state->amplitudes[i];
        }
    }
    target->state->amplitudes = new_amplitudes;
}

void qgate::mcz(vector<qbit*> controls, qbit* target) {
    qgate::entangle(controls);
    qgate::entangle(controls[0], target);
    vector<complex<float>> new_amplitudes(target->state->amplitudes.size());
    for (int i = 0; i < target->state->amplitudes.size(); i++) {
        const int tInd = i;
        bool controlFlag = true;
        for (qbit* control : controls) {
            if (!((i >> control->state_index) & 1)) {
                controlFlag = false;
                break;
            }
        }
        if (controlFlag && ((i >> target->state_index) & 1)) {
            new_amplitudes[i] = -target->state->amplitudes[i];
        } else {
            new_amplitudes[i] = target->state->amplitudes[i];
        }
    }
    target->state->amplitudes = new_amplitudes;
}

bool qgate::measure(qbit* qbit) {
    float prob = 0;
    for (int i = 0; i < qbit->state->amplitudes.size(); i++) {
        if ((i >> qbit->state_index) & 1) {
            prob += norm(qbit->state->amplitudes[i]);
        }
    }
    bool result = ((float)rand() / RAND_MAX) < prob;
    
    vector<complex<float>> new_amplitudes(qbit->state->amplitudes.size()/2);
    int newInd = 0;
    for (int i = 0; i < qbit->state->amplitudes.size(); i++) {
        if (((i >> qbit->state_index) & 1) == result) {
            new_amplitudes[newInd] = qbit->state->amplitudes[i];
            newInd++;
        }
    }
    qbit->state->amplitudes = new_amplitudes;

    for (int i = qbit->state_index + 1; i < qbit->state->qbits.size(); i++) {
        qbit->state->qbits[i]->state_index--;
    }
    qbit->state->qbits.erase(qbit->state->qbits.begin() + qbit->state_index);

    qbit->state_index = 0;
    qbit->state = make_shared<qbit_state>(qbit, result);

    return result;
}




