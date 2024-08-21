
#pragma once

#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <random>

using namespace std;

class QMemory {
public:
    int size;
    vector<complex<float>> amp;
    QMemory(int size) {
        this->size = size;
        this->amp = vector<complex<float>>(pow(2, size));
        this->amp[0] = 1;
    }

    void x(int index) {
        vector<complex<float>> newAmp = vector<complex<float>>(pow(2, size));
        for (int i=0; i<pow(2, size); i++) {
            int new_index = i ^ (1 << index);
            newAmp[new_index] = amp[i];
        }
        amp = newAmp;
    }

    void h(int index) {
        vector<complex<float>> newAmp = vector<complex<float>>(pow(2, size));
        for (int i=0; i<pow(2, size); i++) {
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

    void cx(int control, int target) {
        vector<complex<float>> newAmp = vector<complex<float>>(pow(2, size));
        for (int i=0; i<pow(2, size); i++) {
            if ((i & (1 << control)) == 0) {
                newAmp[i] = amp[i];
            } else {
                int new_index = i ^ (1 << target);
                newAmp[new_index] = amp[i];
            }
        }
        amp = newAmp;
    }

    void mcx(vector<int> controls, int target) {
        vector<complex<float>> newAmp = vector<complex<float>>(pow(2, size));
        for (int i=0; i<pow(2, size); i++) {
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

    void mcz(vector<int> controls) {
        vector<complex<float>> newAmp = vector<complex<float>>(pow(2, size));
        for (int i=0; i<pow(2, size); i++) {
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
};

