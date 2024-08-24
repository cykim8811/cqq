
#ifndef QTRANSFORM_H
#define QTRANSFORM_H

#include "qbit.h"
#include "quantum.h"
#include <iostream>

using namespace std;

class TCreate : public QTransform {
private:
    void forward() {
        this->bit = new qbit(0);
    };
    void backward() {
        bool res = qgate::measure(this->bit);
        if (res == 1) {
            cerr << "Measurement failed" << endl;
        }
        delete this->bit;
    };
};

class TCopy : public QTransform {
private:
    void forward() {
        this->bit = new qbit(0);
        qgate::cx(this->children[0]->bit, this->bit);
    };
    void backward() {
        qgate::cx(this->children[0]->bit, this->bit);
    };
public:
    TCopy(shared_ptr<QTransform> target) {
        this->children.push_back(target);
    };
};

class TX : public QTransform {
private:
    void forward() {
        this->bit = new qbit(0);
        qgate::cx(this->children[0]->bit, this->bit);
        qgate::x(this->bit);
    };
    void backward() {
        qgate::x(this->bit);
        qgate::cx(this->children[0]->bit, this->bit);
    };
public:
    TX(shared_ptr<QTransform> target) {
        this->children.push_back(target);
    };
};

class TH : public QTransform {
private:
    void forward() {
        this->bit = new qbit(0);
        qgate::cx(this->children[0]->bit, this->bit);
        qgate::h(this->bit);
    };
    void backward() {
        qgate::h(this->bit);
        qgate::cx(this->children[0]->bit, this->bit);
    };
public:
    TH(shared_ptr<QTransform> target) {
        this->children.push_back(target);
    };
};


#endif
