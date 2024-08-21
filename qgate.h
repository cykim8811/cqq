
#pragma once

#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <random>
#include "qmemory.h"

using namespace std;

class Qgate {
public:
    virtual ~Qgate() {}
};

class XGate : public Qgate {
public:
    int target;
    QMemory* qm;
    XGate(QMemory* qm, int target) {
        this->qm = qm;
        this->target = target;
        apply_x();
    }

    void apply_x(){
        vector<complex<float>> newAmp = vector<complex<float>>(pow(2, qm->size));
        for (int i=0; i<pow(2, qm->size); i++) {
            int new_index = i ^ (1 << target);
            newAmp[new_index] = qm->amp[i];
        }
        qm->amp = newAmp;
    }

    ~XGate() {
        apply_x();
    }
};

class CXGate : public Qgate {
public:
    int control;
    int target;
    QMemory* qm;
    CXGate(QMemory* qm, int control, int target) {
        this->qm = qm;
        this->control = control;
        this->target = target;

        apply_cx();
    }

    void apply_cx() {
        vector<complex<float>> newAmp = vector<complex<float>>(pow(2, qm->size));
        for (int i=0; i<pow(2, qm->size); i++) {
            if ((i & (1 << control)) == 0) {
                newAmp[i] = qm->amp[i];
            } else {
                int new_index = i ^ (1 << target);
                newAmp[new_index] = qm->amp[i];
            }
        }
        qm->amp = newAmp;
    }

    virtual ~CXGate() {
        apply_cx();
    }
};