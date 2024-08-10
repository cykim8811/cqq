
#ifndef QBIT_H
#define QBIT_H

#include <vector>
#include <complex>
#include <memory>

using namespace std;

class qbit;

namespace qgate {
    void x(qbit* qbit);
    void x(vector<qbit*> qbits);
    void z(qbit* qbit);
    void h(qbit* qbit);
    void cx(qbit* control, qbit* target);
    void mcx(vector<qbit*> controls, qbit* target);
    void mcz(vector<qbit*> controls, qbit* target);
    void entangle(qbit* qbit1, qbit* qbit2);
    void entangle(vector<qbit*> qbits);
    bool measure(qbit* qbit);
}

class qbit_state {
    public:
        qbit_state(qbit* qbit, bool init_value=false);
        vector<qbit*> qbits;
        vector<complex<float>> amplitudes;
};

class qbit {
    public:
        shared_ptr<qbit_state> state;
        int state_index;
        qbit();
        qbit(bool value);
        qbit(const qbit& q);            // disallow copy constructor
        qbit& operator=(const qbit& q); // disallow copy assignment
        ~qbit();

        void _display();
};


#endif // QBIT_H

