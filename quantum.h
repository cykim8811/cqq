
#ifndef QUANTUM_H
#define QUANTUM_H

#include <iostream>
#include <vector>
#include <complex>
#include <memory>
#include <algorithm>

class Qubit;

class QTransform {
public:
    virtual void apply(std::vector<std::complex<float>>& amp) = 0;
};

class QState {
public:
    std::vector<std::complex<float>> amp;
    std::vector<Qubit*> qubit;
    QState() {}
    QState(Qubit* qubit): amp({1, 0}), qubit({qubit}) {}
    void apply(QTransform *gate) {
        gate->apply(amp);
    }
    static void merge(std::vector<std::shared_ptr<QState>> states);
    void _print() {
        std::cout << "QState: ";
        for (auto a : amp) {
            std::cout << a << " ";
        }
        std::cout << std::endl;
    }
};

class Qubit {
public:
    std::shared_ptr<QTransform> transform;
    std::shared_ptr<QState> state;
    Qubit() {
        state = std::make_shared<QState>(this);
        transform = nullptr;
    }
    void _print() {
        state->_print();
    }
    friend class QState;
};

void QState::merge(std::vector<std::shared_ptr<QState>> states) {
    states.erase(std::unique(states.begin(), states.end()), states.end());
    std::shared_ptr<QState> result = std::make_shared<QState>();
    result->amp = {1};
    for (auto s : states) {
        std::vector<std::complex<float>> new_amp;
        for (auto a : s->amp) {
            for (auto r : result->amp) {
                new_amp.push_back(a * r);
            }
        }
        result->amp = new_amp;
        result->qubit.insert(result->qubit.end(), s->qubit.begin(), s->qubit.end());
    }
    for (auto q : result->qubit) {
        q->state = result;
    }
}


#endif // QUANTUM_H
