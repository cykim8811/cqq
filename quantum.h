
#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <complex>
#include <cmath>
#include <random>

using namespace std;

class QGate {
public:
    virtual void forward(vector<complex<float>> &amp) = 0;
    virtual void backward(vector<complex<float>> &amp) = 0;
};

class QMemory {
private:
    vector<complex<float>> amp;
public:
    int size;
    QMemory(int size) {
        this->size = size;
        this->amp = vector<complex<float>>(pow(2, size));
        this->amp[0] = 1;
    }

    bool measure(int index) {
        float prob_zero = 0;
        for (int i=0; i<pow(2, size); i++) {
            if ((i & (1 << index)) == 0) {
                prob_zero += norm(amp[i]);
            }
        }
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<float> dis(0, 1);
        float r = dis(gen);
        bool result = r > prob_zero;
        vector<complex<float>> newAmp = vector<complex<float>>(pow(2, size));
        for (int i=0; i<pow(2, size); i++) {
            if ((i & (1 << index)) == result) {
                newAmp[i] = amp[i] / sqrt(prob_zero);
            }
        }
        amp = newAmp;
        return result;
    }

    void _display() {
        for (int i=0; i<pow(2, size); i++) {
            cout << amp[i] << endl;
        }
    }

    void forward(shared_ptr<QGate> gate) {
        gate->forward(amp);
    }

    void backward(shared_ptr<QGate> gate) {
        gate->backward(amp);
    }
};

class XGate : public QGate {
public:
    int index;
    XGate(int index) {
        this->index = index;
    }

    void forward(vector<complex<float>> &amp) {
        vector<complex<float>> newAmp = vector<complex<float>>(amp.size());
        for (int i=0; i<amp.size(); i++) {
            int new_index = i ^ (1 << index);
            newAmp[new_index] = amp[i];
        }
        amp = newAmp;
    }

    void backward(vector<complex<float>> &amp) {
        this->forward(amp);
    }
};

class HGate : public QGate {
public:
    int index;
    HGate(int index) {
        this->index = index;
    }

    void forward(vector<complex<float>> &amp) {
        vector<complex<float>> newAmp = vector<complex<float>>(amp.size());
        for (int i=0; i<amp.size(); i++) {
            int zero_index = i & ~(1 << index);
            int one_index = i | (1 << index);
            if ((i & (1 << index)) == 0) {
                newAmp[zero_index] += amp[zero_index] / (float)sqrt(2);
                newAmp[one_index] += amp[zero_index] / (float)sqrt(2);
            } else {
                newAmp[zero_index] += amp[one_index] / (float)sqrt(2);
                newAmp[one_index] -= amp[one_index] / (float)sqrt(2);
            }
        }
        amp = newAmp;
    }

    void backward(vector<complex<float>> &amp) {
        this->forward(amp);
    }
};

class CXGate : public QGate {
public:
    int control;
    int target;
    CXGate(int control, int target) {
        this->control = control;
        this->target = target;
    }

    void forward(vector<complex<float>> &amp) {
        vector<complex<float>> newAmp = vector<complex<float>>(amp.size());
        for (int i=0; i<amp.size(); i++) {
            if ((i & (1 << control)) == 0) {
                newAmp[i] = amp[i];
            } else {
                int new_index = i ^ (1 << target);
                newAmp[new_index] = amp[i];
            }
        }
        amp = newAmp;
    }

    void backward(vector<complex<float>> &amp) {
        this->forward(amp);
    }
};

class MCXGate : public QGate {
public:
    vector<int> controls;
    int target;
    MCXGate(vector<int> controls, int target) {
        this->controls = controls;
        this->target = target;
    }

    void forward(vector<complex<float>> &amp) {
        vector<complex<float>> newAmp = vector<complex<float>>(amp.size());
        for (int i=0; i<amp.size(); i++) {
            bool flag = true;
            for (int j=0; j<controls.size(); j++) {
                if ((i & (1 << controls[j])) == 0) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                int new_index = i ^ (1 << target);
                newAmp[new_index] = amp[i];
            } else {
                newAmp[i] = amp[i];
            }
        }
        amp = newAmp;
    }

    void backward(vector<complex<float>> &amp) {
        this->forward(amp);
    }
};

class MCZGate : public QGate {
public:
    vector<int> controls;
    MCZGate(vector<int> controls) {
        this->controls = controls;
    }

    void forward(vector<complex<float>> &amp) {
        vector<complex<float>> newAmp = vector<complex<float>>(amp.size());
        for (int i=0; i<amp.size(); i++) {
            bool flag = true;
            for (int j=0; j<controls.size(); j++) {
                if ((i & (1 << controls[j])) == 0) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                newAmp[i] = -amp[i];
            } else {
                newAmp[i] = amp[i];
            }
        }
        amp = newAmp;
    }

    void backward(vector<complex<float>> &amp) {
        this->forward(amp);
    }
};

