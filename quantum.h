
#ifndef QUANTUM_H
#define QUANTUM_H

#include <vector>
#include <complex>
#include <memory>

using namespace std;

class QState;

class QTransform {
public:
    bool applied = false;
    virtual void forward(QState* target) = 0;
    virtual void backward(QState* target) = 0;
};

class QState {
public:
    vector<unique_ptr<QTransform>> transforms;
    int forward(unique_ptr<QTransform> transform) {
        transforms.push_back(transform);
        transform->forward(this);
        transform->applied = true;
        return transforms.size() - 1;
    }
    void backward(int index) {
        // transforms[index]->backward(this);
        for (int i = 0; i <= index-1; i++) {
            if (!transforms[i]->applied) {
                transforms[i]->forward(this);
            }
        }
        transforms[index]->backward(this);
        transforms[index]->applied = false;
        for (int i = index-1; i >= 0; i--) {
            if (!transforms[i]->applied) {
                transforms[i]->backward(this);
            }
        }
    }
};


#endif // QUANTUM_H

