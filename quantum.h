
#ifndef QUANTUM_H
#define QUANTUM_H

#include "qbit.h"
#include <iostream>
#include <vector>
#include <memory>
#include <complex>

using namespace std;

template<typename T>
class QTransform {
private:
    bool alive;
    virtual void forward() = 0;
    virtual void backward() = 0;
    void temp_forward() {
        for (auto child: children) {
            child->temp_forward();
        }
        if (!alive) {
            forward();
        }
    };
    void temp_backward() {
        if (!alive) {
            backward();
        }
        for (auto child: children) {
            child->temp_backward();
        }
    };
protected:
    vector<shared_ptr<QTransform>> children;
public:
    T *data;
    bool applied = false;
    void apply() {
        forward();
    };
    void kill() {
        temp_forward();
        alive = false;
        temp_backward();
    };
    QTransform() : alive(true) {};
};


#endif
