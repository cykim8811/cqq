
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
        int final_value = (int) (*this->data);   // Measure
        // if (final_value != initial_value) {
        //     cerr << "Warning: back-calculation failed" << endl;
        // }
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
        cout << "Copying" << endl;
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

template<int N>
class TBNot: public QTransform<QuantumRegister<N>> {
private:
    void forward() {
        this->data = new QuantumRegister<N>();
        (*this->data) ^= (*this->children[0]->data);
        this->data->x();
    };
    void backward() {
        this->data->x();
        (*this->data) ^= (*this->children[0]->data);
        (int) (*this->data);   // Measure
        delete this->data;
    };
public:
    TBNot(const shared_ptr<QTransform<QuantumRegister<N>>>& target) {
        this->children.push_back(target);
    };
};

template<int N>
class TBAnd: public QTransform<QuantumRegister<N>> {
private:
    void forward() {
        this->data = new QuantumRegister<N>();
        for (int i = 0; i < N; i++) {
            qgate::mcx({this->children[0]->data->qbits[i], this->children[1]->data->qbits[i]}, this->data->qbits[i]);
        }
    };
    void backward() {
        for (int i = 0; i < N; i++) {
            qgate::mcx({this->children[0]->data->qbits[i], this->children[1]->data->qbits[i]}, this->data->qbits[i]);
        }
        (int) (*this->data);   // Measure
        delete this->data;
    };
public:
    TBAnd(const shared_ptr<QTransform<QuantumRegister<N>>>& target1, const shared_ptr<QTransform<QuantumRegister<N>>>& target2) {
        this->children.push_back(target1);
        this->children.push_back(target2);
    };
};

template<int N>
class TBOr: public QTransform<QuantumRegister<N>> {
private:
    void forward() {
        this->data = new QuantumRegister<N>();
        for (int i = 0; i < N; i++) {
            qgate::x(this->children[0]->data->qbits[i]);
            qgate::x(this->children[1]->data->qbits[i]);
            qgate::mcx({this->children[0]->data->qbits[i], this->children[1]->data->qbits[i]}, this->data->qbits[i]);
            qgate::x(this->children[1]->data->qbits[i]);
            qgate::x(this->children[0]->data->qbits[i]);
            qgate::x(this->data->qbits[i]);
        }
    };
    void backward() {
        for (int i = 0; i < N; i++) {
            qgate::x(this->data->qbits[i]);
            qgate::x(this->children[0]->data->qbits[i]);
            qgate::x(this->children[1]->data->qbits[i]);
            qgate::mcx({this->children[0]->data->qbits[i], this->children[1]->data->qbits[i]}, this->data->qbits[i]);
            qgate::x(this->children[1]->data->qbits[i]);
            qgate::x(this->children[0]->data->qbits[i]);
        }
        (int) (*this->data);   // Measure
        delete this->data;
    };
public:
    TBOr(const shared_ptr<QTransform<QuantumRegister<N>>>& target1, const shared_ptr<QTransform<QuantumRegister<N>>>& target2) {
        this->children.push_back(target1);
        this->children.push_back(target2);
    };
};

template<int N>
class TBXOR: public QTransform<QuantumRegister<N>> {
private:
    void forward() {
        this->data = new QuantumRegister<N>();
        (*this->data) += (*this->children[0]->data);
        (*this->data) ^= (*this->children[1]->data);
    };
    void backward() {
        (*this->data) ^= (*this->children[1]->data);
        (*this->data) -= (*this->children[0]->data);
        (int) (*this->data);   // Measure
        delete this->data;
    };
public:
    TBXOR(shared_ptr<QTransform<QuantumRegister<N>>>& target1, shared_ptr<QTransform<QuantumRegister<N>>>& target2) {
        this->children.push_back(target1);
        this->children.push_back(target2);
    };
};

template<int N>
class TBH: public QTransform<QuantumRegister<N>> {
private:
    void forward() {
        this->data = new QuantumRegister<N>();
        // swap
        for (int i = 0; i < N; i++) {
            qgate::cx(this->children[0]->data->qbits[i], this->data->qbits[i]);
            qgate::cx(this->data->qbits[i], this->children[0]->data->qbits[i]);
            qgate::cx(this->children[0]->data->qbits[i], this->data->qbits[i]);
        }
        this->data->h();
    };
    void backward() {
        this->data->h();
        // swap
        for (int i = 0; i < N; i++) {
            qgate::cx(this->children[0]->data->qbits[i], this->data->qbits[i]);
            qgate::cx(this->data->qbits[i], this->children[0]->data->qbits[i]);
            qgate::cx(this->children[0]->data->qbits[i], this->data->qbits[i]);
        }
        (int) (*this->data);   // Measure
        delete this->data;
    };
public:
    TBH(const shared_ptr<QTransform<QuantumRegister<N>>>& target) {
        this->children.push_back(target);
    };
};

template<int N>
class TBZ: public QTransform<QuantumRegister<N>> {
private:
    void forward() {
        this->data = new QuantumRegister<N>();
        // swap
        for (int i = 0; i < N; i++) {
            qgate::cx(this->children[0]->data->qbits[i], this->data->qbits[i]);
            qgate::cx(this->data->qbits[i], this->children[0]->data->qbits[i]);
            qgate::cx(this->children[0]->data->qbits[i], this->data->qbits[i]);
        }
        this->data->z();
    };
    void backward() {
        this->data->z();
        // swap
        for (int i = 0; i < N; i++) {
            qgate::cx(this->children[0]->data->qbits[i], this->data->qbits[i]);
            qgate::cx(this->data->qbits[i], this->children[0]->data->qbits[i]);
            qgate::cx(this->children[0]->data->qbits[i], this->data->qbits[i]);
        }
        (int) (*this->data);   // Measure
        delete this->data;
    };
public:
    TBZ(const shared_ptr<QTransform<QuantumRegister<N>>>& target) {
        this->children.push_back(target);
    };
};


#endif // QUTRANSFORM_H

