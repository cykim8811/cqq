
#pragma once

#include "qbit.h"
#include <vector>
#include <iostream>
#include <memory>

using namespace std;

class QGate {
public:
    QGate() {}
    virtual void forward(vector<qbit*> qbits) = 0;
    virtual void backward(vector<qbit*> qbits) = 0;
};

class QXGate: public QGate {
public:
    vector<int> qbit_index;

    QXGate(vector<int> qbit_index) {
        this->qbit_index = qbit_index;
    }

    QXGate(int qbit_index) {
        this->qbit_index.push_back(qbit_index);
    }

    void forward(vector<qbit*> qbits) {
        vector<qbit*> qbits_ptr;
        for (int i : qbit_index) {
            qbits_ptr.push_back(qbits[i]);
        }
        qgate::x(qbits_ptr);
    }

    void backward(vector<qbit*> qbits) {
        vector<qbit*> qbits_ptr;
        for (int i : qbit_index) {
            qbits_ptr.push_back(qbits[i]);
        }
        qgate::x(qbits_ptr);
    }
};

class QZGate: public QGate {
public:
    int qbit_index;
    
    QZGate(int qbit_index) {
        this->qbit_index = qbit_index;
    }

    void forward(vector<qbit*> qbits) {
        qgate::z(qbits[qbit_index]);
    }

    void backward(vector<qbit*> qbits) {
        qgate::z(qbits[qbit_index]);
    }
};

class QHGate: public QGate {
public:
    int qbit_index;
    
    QHGate(int qbit_index) {
        this->qbit_index = qbit_index;
    }

    void forward(vector<qbit*> qbits) {
        qgate::h(qbits[qbit_index]);
    }

    void backward(vector<qbit*> qbits) {
        qgate::h(qbits[qbit_index]);
    }
};

class QCXGate: public QGate {
public:
    int control_index;
    int target_index;
    
    QCXGate(int control_index, int target_index) {
        this->control_index = control_index;
        this->target_index = target_index;
    }

    void forward(vector<qbit*> qbits) {
        qgate::cx(qbits[control_index], qbits[target_index]);
    }

    void backward(vector<qbit*> qbits) {
        qgate::cx(qbits[control_index], qbits[target_index]);
    }
};

class QMCXGate: public QGate {
public:
    vector<int> control_indices;
    int target_index;
    
    QMCXGate(vector<int> control_indices, int target_index) {
        this->control_indices = control_indices;
        this->target_index = target_index;
    }

    void forward(vector<qbit*> qbits) {
        vector<qbit*> controls;
        for (int i : control_indices) {
            controls.push_back(qbits[i]);
        }
        qgate::mcx(controls, qbits[target_index]);
    }

    void backward(vector<qbit*> qbits) {
        vector<qbit*> controls;
        for (int i : control_indices) {
            controls.push_back(qbits[i]);
        }
        qgate::mcx(controls, qbits[target_index]);
    }
};

class QMCZGate: public QGate {
public:
    vector<int> control_indices;
    int target_index;
    
    QMCZGate(vector<int> control_indices, int target_index) {
        this->control_indices = control_indices;
        this->target_index = target_index;
    }

    void forward(vector<qbit*> qbits) {
        vector<qbit*> controls;
        for (int i : control_indices) {
            controls.push_back(qbits[i]);
        }
        qgate::mcz(controls, qbits[target_index]);
    }

    void backward(vector<qbit*> qbits) {
        vector<qbit*> controls;
        for (int i : control_indices) {
            controls.push_back(qbits[i]);
        }
        qgate::mcz(controls, qbits[target_index]);
    }
};


class QCircuit: public QGate {
public:
    vector<unique_ptr<QGate>> gates;

    QCircuit() {}

    void add_gate(unique_ptr<QGate> gate) {
        gates.push_back(move(gate));
    }

    void x(int qbit_index) {
        gates.push_back(make_unique<QXGate>(qbit_index));
    }

    void x(vector<int> qbit_index) {
        gates.push_back(make_unique<QXGate>(qbit_index));
    }

    void z(int qbit_index) {
        gates.push_back(make_unique<QZGate>(qbit_index));
    }

    void h(int qbit_index) {
        gates.push_back(make_unique<QHGate>(qbit_index));
    }

    void cx(int control_index, int target_index) {
        gates.push_back(make_unique<QCXGate>(control_index, target_index));
    }

    void mcx(vector<int> control_indices, int target_index) {
        gates.push_back(make_unique<QMCXGate>(control_indices, target_index));
    }

    void mcz(vector<int> control_indices, int target_index) {
        gates.push_back(make_unique<QMCZGate>(control_indices, target_index));
    }

    void forward(vector<qbit*> qbits) {
        for (auto& gate : gates) {
            gate->forward(qbits);
        }
    }
    
    void backward(vector<qbit*> qbits) {
        for (auto it=gates.rbegin(); it!=gates.rend(); ++it) {
            (*it)->backward(qbits);
        }
    }
};

