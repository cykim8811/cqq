
#ifndef QUTRANSFORM_H
#define QUTRANSFORM_H

#include "qbyte.h"
#include "quantum.h"
#include <iostream>

using namespace std;

template<int N>
class TBCreate: public QTransform<QuantumRegister<N>> {
private:
    int initial_value;
    void forward() {
        this->data = new QuantumRegister<N>(initial_value);
    };
    void backward() {
        (int) (*this->data);   // Measure
        delete this->data;
    };
public:
    TBCreate(): initial_value(0) {};
    TBCreate(int value): initial_value(value) {};
};

template<int N>
class TBCopy: public QTransform<QuantumRegister<N>> {
private:
    void forward() {
        this->data = new QuantumRegister<N>();
        (*this->data) ^= (*this->children[0]->data);
    };
    void backward() {
        (*this->data) ^= (*this->children[0]->data);
        (int) (*this->data);   // Measure
        delete this->data;
    };
public:
    TBCopy(shared_ptr<QTransform<QuantumRegister<N>>> target) {
        this->children.push_back(target);
    };
};

template<int N>
class TBAddi: public QTransform<QuantumRegister<N>> {
private:
    int value;
    void forward() {
        this->data = new QuantumRegister<N>();
        (*this->data) += (*this->children[0]->data);
        (*this->data) += value;
    };
    void backward() {
        (*this->data) -= value;
        (*this->data) -= (*this->children[0]->data);
        (int) (*this->data);   // Measure
        delete this->data;
    };
public:
    TBAddi(shared_ptr<QTransform<QuantumRegister<N>>> target, int value): value(value) {
        this->children.push_back(target);
    };
};

template<int N>
class TBAdd: public QTransform<QuantumRegister<N>> {
private:
    void forward() {
        this->data = new QuantumRegister<N>();
        (*this->data) += (*this->children[0]->data);
        (*this->data) += (*this->children[1]->data);
    };
    void backward() {
        (*this->data) -= (*this->children[1]->data);
        (*this->data) -= (*this->children[0]->data);
        (int) (*this->data);   // Measure
        delete this->data;
    };
public:
    TBAdd(shared_ptr<QTransform<QuantumRegister<N>>>& target1, shared_ptr<QTransform<QuantumRegister<N>>>& target2) {
        this->children.push_back(target1);
        this->children.push_back(target2);
    };
};

template<int N>
class TBSubi: public QTransform<QuantumRegister<N>> {
private:
    int value;
    void forward() {
        this->data = new QuantumRegister<N>();
        (*this->data) += (*this->children[0]->data);
        (*this->data) -= value;
    };
    void backward() {
        (*this->data) += value;
        (*this->data) -= (*this->children[0]->data);
        (int) (*this->data);   // Measure
        delete this->data;
    };
public:
    TBSubi(shared_ptr<QTransform<QuantumRegister<N>>>& target, int value): value(value) {
        this->children.push_back(target);
    };
};

template<int N>
class TBSub: public QTransform<QuantumRegister<N>> {
private:
    void forward() {
        this->data = new QuantumRegister<N>();
        (*this->data) += (*this->children[0]->data);
        (*this->data) -= (*this->children[1]->data);
    };
    void backward() {
        (*this->data) += (*this->children[1]->data);
        (*this->data) -= (*this->children[0]->data);
        (int) (*this->data);   // Measure
        delete this->data;
    };
public:
    TBSub(shared_ptr<QTransform<QuantumRegister<N>>>& target1, shared_ptr<QTransform<QuantumRegister<N>>>& target2) {
        this->children.push_back(target1);
        this->children.push_back(target2);
    };
};

template<int N>
class TBNeg: public QTransform<QuantumRegister<N>> {
private:
    void forward() {
        this->data = new QuantumRegister<N>();
        (*this->data) -= (*this->children[0]->data);
    };
    void backward() {
        (*this->data) += (*this->children[0]->data);
        (int) (*this->data);   // Measure
        delete this->data;
    };
public:
    TBNeg(shared_ptr<QTransform<QuantumRegister<N>>>& target) {
        this->children.push_back(target);
    };
};


#endif // QUTRANSFORM_H

