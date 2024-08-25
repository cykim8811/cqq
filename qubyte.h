
#ifndef QUBYTE_H
#define QUBYTE_H

#include "qbyte.h"
#include "qutransform.h"
#include <vector>

template<int N>
class QByte {
public:
    shared_ptr<QTransform<QuantumRegister<N>>> transform;
    QByte(shared_ptr<QTransform<QuantumRegister<N>>> _transform): transform(move(_transform)) {
        transform->apply();
    };

    QByte(): transform(make_shared<TBCreate<N>>()) {
        transform->apply();
    };

    QByte(int value): transform(make_shared<TBCreate<N>>(value)) {
        transform->apply();
    };

    ~QByte() {
        transform->kill();
    };

    QByte(const QByte &q): transform(make_shared<TBCopy<N>>(q.transform)) {
        transform->apply();
    };

    friend void swap(QByte &first, QByte &second) {
        swap(first.transform, second.transform);
    };

    QByte& operator=(QByte q) {
        swap(*this, q);
        return *this;
    };

    QByte operator+(int value) {
        return QByte(make_shared<TBAddi<N>>(transform, value));
    };

    QByte operator+(QByte q) {
        return QByte(make_shared<TBAdd<N>>(transform, q.transform));
    };

    QByte operator-(int value) {
        return QByte(make_shared<TBSubi<N>>(transform, value));
    };

    QByte operator-(QByte q) {
        return QByte(make_shared<TBSub<N>>(transform, q.transform));
    };

    QByte operator-() {
        return QByte(make_shared<TBNeg<N>>(transform));
    };

    QByte operator~() {
        return QByte(make_shared<TBNot<N>>(transform));
    };

    QByte operator&(QByte q) {
        return QByte(make_shared<TBAnd<N>>(transform, q.transform));
    };

    QByte operator^(QByte q) {
        return QByte(make_shared<TBXOR<N>>(transform, q.transform));
    };

    QByte operator^=(QByte q) {
        QByte xor_qubit = QByte(make_shared<TBXOR<N>>(transform, q.transform));
        swap(*this, xor_qubit);
        return *this;
    };

    explicit operator int() {
        return (int)(*transform->data);
    };

};


typedef QByte<8> qbyte_t;
typedef QByte<8> qchar_t;
typedef QByte<16> qshort_t;
typedef QByte<32> qint_t;
typedef QByte<64> qlong_t;
typedef QByte<64> qlonglong_t;

typedef QByte<1> qint1_t;
typedef QByte<2> qint2_t;
typedef QByte<4> qint4_t;
typedef QByte<8> qint8_t;
typedef QByte<16> qint16_t;
typedef QByte<64> qint64_t;

typedef QByte<1> qbool_t;
typedef QByte<1> qbit_t;
typedef QByte<2> qdibit_t;
typedef QByte<4> qnibl_t;

template<int N>
QByte<N> h(QByte<N> &q) {
    return QByte<N>(make_shared<TBH<N>>(q.transform));
};

template<int N>
QByte<N> z(QByte<N> q) {
    return QByte<N>(make_shared<TBZ<N>>(q.transform));
};

template<typename T>
T h(int value) {
    T v = T(value);
    return h(v);
};


#endif  // QUBYTE_H

