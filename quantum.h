
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
    
    // void temp_forward() {
    //     for (auto child: children) {
    //         child->temp_forward();
    //     }
    //     if (!alive) {
    //         apply_forward();
    //     }
    // };

    void temp_forward() {
        vector<QTransform<T>*> queue;
        vector<QTransform<T>*> order;
        queue.push_back(this);
        while (!queue.empty()) {
            QTransform<T>* current = queue.back();
            queue.pop_back();
            order.push_back(current);
            for (auto child: current->children) {
                queue.push_back(child.get());
            }
        }
        reverse(order.begin(), order.end());
        for (auto current: order) {
            if (!current->alive) {
                current->apply_forward();
            }
        }
    };
        
    
    // void temp_backward() {
    //     if (!alive) {
    //         apply_backward();
    //     }
    //     for (auto child: children) {
    //         child->temp_backward();
    //     }
    // };

    void temp_backward() {
        vector<QTransform<T>*> queue;
        queue.push_back(this);
        while (!queue.empty()) {
            QTransform<T>* current = queue.back();
            queue.pop_back();
            if (!current->alive) {
                current->apply_backward();
            }
            for (auto child: current->children) {
                queue.push_back(child.get());
            }
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
