
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

    friend void swap(QByte &first, QByte &second) noexcept {
        swap(first.transform, second.transform);
    };

    QByte& operator=(QByte q) noexcept {
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

    operator int() {
        return (int)(*transform->data);
    };

};




#endif  // QUBYTE_H

