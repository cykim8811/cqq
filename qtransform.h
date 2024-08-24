
#ifndef QTRANSFORM_H
#define QTRANSFORM_H

#include "qbit.h"
#include "quantum.h"
#include <iostream>

using namespace std;

class TCreate : public QTransform<qbit> {
private:
    void forward() {
        this->data = new qbit(0);
    };
    void backward() {
        bool res = qgate::measure(this->data);
        if (res == 1) {
            cerr << "Measurement failed" << endl;
        }
        delete this->data;
    };
};

class TCopy : public QTransform<qbit> {
private:
    void forward() {
        this->data = new qbit(0);
        qgate::cx(this->children[0]->data, this->data);
    };
    void backward() {
        qgate::cx(this->children[0]->data, this->data);
        qgate::measure(this->data);
        delete this->data;
    };
public:
    TCopy(shared_ptr<QTransform<qbit>> target) {
        this->children.push_back(target);
    };
};

class TX : public QTransform<qbit> {
private:
    void forward() {
        this->data = new qbit(0);
        qgate::cx(this->children[0]->data, this->data);
        qgate::x(this->data);
    };
    void backward() {
        qgate::x(this->data);
        qgate::cx(this->children[0]->data, this->data);
        qgate::measure(this->data);
        delete this->data;
    };
public:
    TX(shared_ptr<QTransform<qbit>> target) {
        this->children.push_back(target);
    };
};

class TH : public QTransform<qbit> {
private:
    void forward() {
        this->data = new qbit(0);
        qgate::cx(this->children[0]->data, this->data);
        qgate::h(this->data);
    };
    void backward() {
        qgate::h(this->data);
        qgate::cx(this->children[0]->data, this->data);
        qgate::measure(this->data);
        delete this->data;
    };
public:
    TH(shared_ptr<QTransform<qbit>> target) {
        this->children.push_back(target);
    };
};

class TAND : public QTransform<qbit> {
private:
    void forward() {
        this->data = new qbit(0);
        qgate::mcx({this->children[0]->data, this->children[1]->data}, this->data);
    };
    void backward() {
        qgate::mcx({this->children[0]->data, this->children[1]->data}, this->data);
        qgate::measure(this->data);
        delete this->data;
    };
public:
    TAND(shared_ptr<QTransform<qbit>> target1, shared_ptr<QTransform<qbit>> target2) {
        this->children.push_back(target1);
        this->children.push_back(target2);
    };
};

class TOR : public QTransform<qbit> {
private:
    void forward() {
        this->data = new qbit(0);
        qgate::x(this->children[0]->data);
        qgate::x(this->children[1]->data);
        qgate::mcx({this->children[0]->data, this->children[1]->data}, this->data);
        qgate::x(this->children[0]->data);
        qgate::x(this->children[1]->data);

        qgate::x(this->data);
    };
    void backward() {
        qgate::x(this->data);

        qgate::x(this->children[0]->data);
        qgate::x(this->children[1]->data);
        qgate::mcx({this->children[0]->data, this->children[1]->data}, this->data);
        qgate::x(this->children[0]->data);
        qgate::x(this->children[1]->data);

        qgate::measure(this->data);
        delete this->data;
    };
public:
    TOR(shared_ptr<QTransform<qbit>> target1, shared_ptr<QTransform<qbit>> target2) {
        this->children.push_back(target1);
        this->children.push_back(target2);
    };
};

class TXOR : public QTransform<qbit> {
private:
    void forward() {
        this->data = new qbit(0);
        qgate::cx(this->children[0]->data, this->data);
        qgate::cx(this->children[1]->data, this->data);
    }
    void backward() {
        qgate::cx(this->children[1]->data, this->data);
        qgate::cx(this->children[0]->data, this->data);
        qgate::measure(this->data);
        delete this->data;
    }
public:
    TXOR(shared_ptr<QTransform<qbit>> target1, shared_ptr<QTransform<qbit>> target2) {
        this->children.push_back(target1);
        this->children.push_back(target2);
    };
};


#endif  // QTRANSFORM_H
