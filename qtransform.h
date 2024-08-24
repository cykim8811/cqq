
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
        qgate::measure(this->bit);
        delete this->bit;
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
        qgate::measure(this->bit);
        delete this->bit;
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
        qgate::measure(this->bit);
        delete this->bit;
    };
public:
    TH(shared_ptr<QTransform> target) {
        this->children.push_back(target);
    };
};

class TAND : public QTransform {
private:
    void forward() {
        this->bit = new qbit(0);
        qgate::mcx({this->children[0]->bit, this->children[1]->bit}, this->bit);
    };
    void backward() {
        qgate::mcx({this->children[0]->bit, this->children[1]->bit}, this->bit);
        qgate::measure(this->bit);
        delete this->bit;
    };
public:
    TAND(shared_ptr<QTransform> target1, shared_ptr<QTransform> target2) {
        this->children.push_back(target1);
        this->children.push_back(target2);
    };
};

class TOR : public QTransform {
private:
    void forward() {
        this->bit = new qbit(0);
        qgate::x(this->children[0]->bit);
        qgate::x(this->children[1]->bit);
        qgate::mcx({this->children[0]->bit, this->children[1]->bit}, this->bit);
        qgate::x(this->children[0]->bit);
        qgate::x(this->children[1]->bit);

        qgate::x(this->bit);
    };
    void backward() {
        qgate::x(this->bit);

        qgate::x(this->children[0]->bit);
        qgate::x(this->children[1]->bit);
        qgate::mcx({this->children[0]->bit, this->children[1]->bit}, this->bit);
        qgate::x(this->children[0]->bit);
        qgate::x(this->children[1]->bit);

        qgate::measure(this->bit);
        delete this->bit;
    };
public:
    TOR(shared_ptr<QTransform> target1, shared_ptr<QTransform> target2) {
        this->children.push_back(target1);
        this->children.push_back(target2);
    };
};

class TXOR : public QTransform {
private:
    void forward() {
        this->bit = new qbit(0);
        qgate::cx(this->children[0]->bit, this->bit);
        qgate::cx(this->children[1]->bit, this->bit);
    }
    void backward() {
        qgate::cx(this->children[1]->bit, this->bit);
        qgate::cx(this->children[0]->bit, this->bit);
        qgate::measure(this->bit);
        delete this->bit;
    }
public:
    TXOR(shared_ptr<QTransform> target1, shared_ptr<QTransform> target2) {
        this->children.push_back(target1);
        this->children.push_back(target2);
    };
};


#endif
