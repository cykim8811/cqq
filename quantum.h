
#ifndef QUANTUM_H
#define QUANTUM_H

#include <iostream>
#include <vector>
#include <memory>

using namespace std;

class Transform {
private:
    bool alive;
    string name;
    void forward() {
        cout << "Forward: " << name << endl;
    };
    void backward() {
        cout << "Backward: " << name << endl;
    };
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
    vector<shared_ptr<Transform>> children;
public:
    void apply() {
        forward();
    };
    void kill() {
        temp_forward();
        alive = false;
        temp_backward();
    };
    Transform(string name): name(name), alive(true) {};
};


class Qubit {
public:
    shared_ptr<Transform> transform;
    Qubit(shared_ptr<Transform> _transform): transform(_transform) {
        transform->apply();
    }
    ~Qubit() {
        transform->kill();
    };
};


class UnaryTransform: public Transform {
public:
    UnaryTransform(string name, Qubit* p0): Transform(name) {
        children.push_back(p0->transform);
    };
};

class BinaryTransform: public Transform {
public:
    BinaryTransform(string name, Qubit* p0, Qubit* p1): Transform(name) {
        children.push_back(p0->transform);
        children.push_back(p1->transform);
    };
};

#endif
