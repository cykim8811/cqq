
#ifndef QUANTUM_H
#define QUANTUM_H

#include <iostream>
#include <vector>
#include <complex>
#include <memory>

class Qubit;

class QTransform {
public:
    virtual void apply(std::vector<std::complex<float>>& amp) = 0;
};

class QState {
private:
    std::vector<std::complex<float>> amp;
    std::vector<Qubit*> qubit;
public:
    QState(Qubit* qubit): amp({1, 0}), qubit({qubit}) {}
    void apply(QTransform *gate) {
        gate->apply(amp);
    }
    void _print() {
        std::cout << "QState: ";
        for (auto a : amp) {
            std::cout << a << " ";
        }
        std::cout << std::endl;
    }
};

class Qubit {
private:
    std::shared_ptr<QState> state;
    std::shared_ptr<QTransform> transform;
public:
    Qubit() {
        state = std::make_shared<QState>(this);
        transform = nullptr;
    }
    void _print() {
        state->_print();
    }
};


#endif // QUANTUM_H
