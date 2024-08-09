#ifndef QUANTUM_H
#define QUANTUM_H

#include <vector>
#include <complex>

using namespace std;

class QubitState;

class Qubit {
    private:
        QubitState* state;

    public:
        Qubit();
        Qubit(bool value);
        ~Qubit();
        bool measure();
        void h();
        void x();
        void cx(Qubit& control);
        void mcx(vector<Qubit*> controls);
        void mcz(vector<Qubit*> controls);
        void phase(float angle);
        void operator^=(Qubit &q);
        Qubit& operator!();
        Qubit& operator=(Qubit &q);
        Qubit& operator=(bool value);
        void _display();
    friend class QubitState;
};

class QubitState {
    private:
    
        vector<Qubit*> qubits;
        vector<complex<float>> amplitudes;
        void entangle(Qubit* qubit);
        void entangle(vector<Qubit*> qubits);
    public:
        QubitState(Qubit* qubit);
        ~QubitState();
        bool measure(Qubit* target);
        void h(Qubit* target);
        void x(Qubit* target);
        void cx(Qubit* control, Qubit* target);
        void mcx(vector<Qubit*> controls, Qubit* target);
        void mcz(vector<Qubit*> controls, Qubit* target);
        void phase(Qubit* target, float angle);
        void remove(Qubit* qubit);
        void product(QubitState* other);
        void _display();
    friend class Qubit;
};

#endif // QUANTUM_H