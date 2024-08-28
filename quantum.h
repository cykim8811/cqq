
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

    void apply_forward() {
        if (applied) return;
        forward();
        applied = true;
    };

    void apply_backward() {
        if (!applied) return;
        backward();
        applied = false;
    };
    
    void temp_forward() {
        for (auto child: children) {
            child->temp_forward();
        }
        if (!alive) {
            apply_forward();
        }
    };
    void temp_backward() {
        if (!alive) {
            apply_backward();
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
        apply_forward();
    };
    void kill() {
        temp_forward();
        alive = false;
        temp_backward();
    };
    QTransform() : alive(true) {};
};


#endif
