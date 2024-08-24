
#include "quantum.h"

int main() {
    unique_ptr<Qubit> q0 = make_unique<Qubit>(make_shared<Transform>("q0"));
    unique_ptr<Qubit> q1 = make_unique<Qubit>(make_shared<Transform>("q1"));
    unique_ptr<Qubit> q2 = make_unique<Qubit>(make_shared<UnaryTransform>("q2", q1.get()));
    unique_ptr<Qubit> q3 = make_unique<Qubit>(make_shared<BinaryTransform>("q3", q0.get(), q2.get()));
    q0.reset();
    q2.reset();
    q1.reset();
    q3.reset();
}

