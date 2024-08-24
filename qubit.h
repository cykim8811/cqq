
#ifndef QUBIT_H
#define QUBIT_H

#include "quantum.h"
#include "qtransform.h"


class Qubit {
public:
    shared_ptr<QTransform<qbit>> transform;
    Qubit(shared_ptr<QTransform<qbit>> _transform): transform(move(_transform)) {
        transform->apply();
    };

    Qubit(): transform(make_shared<TCreate>()) {
        transform->apply();
    };

    ~Qubit() {
        transform->kill();
    };

    Qubit(const Qubit &q): transform(make_shared<TCopy>(q.transform)) {
        transform->apply();
    };

    friend void swap(Qubit &first, Qubit &second) noexcept {
        swap(first.transform, second.transform);
    };

    Qubit& operator=(Qubit q) noexcept {
        swap(*this, q);
        return *this;
    };

    Qubit operator~() {
        return Qubit(make_shared<TX>(transform));
    };

    Qubit operator^(Qubit q) {
        return Qubit(make_shared<TXOR>(transform, q.transform));
    };

    Qubit operator&(Qubit q) {
        return Qubit(make_shared<TAND>(transform, q.transform));
    };

    Qubit operator|(Qubit q) {
        return Qubit(make_shared<TOR>(transform, q.transform));
    };

    operator int() {
        return qgate::measure(transform->data);
    };

    operator bool() {
        return qgate::measure(transform->data);
    };

    static Qubit h(Qubit q) {
        return Qubit(make_shared<TH>(q.transform));
    };

};




#endif
