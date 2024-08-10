
#include <vector>
#include <iostream>
#include "qbyte.h"

using namespace std;

void increment(vector<qbit*>& qbits) {
    qbit *ancilla = new qbit(true);

    for (int i=0; i<qbits.size()-1; i++) {
        qgate::cx(ancilla, qbits[i]);
        qgate::x(vector<qbit*>(qbits.begin(), qbits.begin()+i));
        qgate::mcx(vector<qbit*>(qbits.begin(), qbits.begin()+i+1), ancilla);
        qgate::x(vector<qbit*>(qbits.begin(), qbits.begin()+i));
    }
    qgate::cx(ancilla, qbits[qbits.size()-1]);

    qgate::x(ancilla);
    
    for (int i=0; i<qbits.size()-1; i++) qgate::x(qbits[i]);
    if (qbits.size() > 1) qgate::mcx(vector<qbit*>(qbits.begin(), qbits.end()-1), ancilla);
    else qgate::x(ancilla);
    for (int i=0; i<qbits.size()-1; i++) qgate::x(qbits[i]);

    delete ancilla;
}

void increment(vector<qbit*>& qbits, qbit* condition) {
    qbit *ancilla = new qbit(true);

    for (int i=0; i<qbits.size()-1; i++) {
        qgate::mcx({condition, ancilla}, qbits[i]);
        for (int j=0; j<i; j++) qgate::cx(condition, qbits[j]);
        vector<qbit*> qbits_to_increment(qbits.begin(), qbits.begin()+i+1);
        qbits_to_increment.push_back(condition);
        qgate::mcx(qbits_to_increment, ancilla);
        for (int j=0; j<i; j++) qgate::cx(condition, qbits[j]);
    }
    qgate::mcx({condition, ancilla}, qbits[qbits.size()-1]);

    qgate::cx(condition, ancilla);
    
    for (int i=0; i<qbits.size()-1; i++) qgate::cx(condition, qbits[i]);
    if (qbits.size() > 1) {
        vector<qbit*> qbits_to_increment(qbits.begin(), qbits.end()-1);
        qbits_to_increment.push_back(condition);
        qgate::mcx(qbits_to_increment, ancilla);
    } else {
        qgate::cx(condition, ancilla);
    }
    for (int i=0; i<qbits.size()-1; i++) qgate::cx(condition, qbits[i]);

    delete ancilla;
}


qbyte_t::qbyte_t() {
    for (int i=0; i<qbyte_t::size; i++) {
        qbits.push_back(new qbit(0));
    }
}

qbyte_t::qbyte_t(int value) {
    for (int i=0; i<qbyte_t::size; i++) {
        qbits.push_back(new qbit((value >> i) & 1));
    }
}

qbyte_t::~qbyte_t() {
    for (int i=0; i<qbyte_t::size; i++) {
        delete qbits[i];
    }
}

qbyte_t::operator int() {
    int value = 0;
    for (int i=0; i<qbyte_t::size; i++) {
        value |= (qgate::measure(qbits[i]) << i);
    }
    return value;
}

qbyte_t::operator char() {
    return (char) int(*this);
}

// void qbyte_t::operator+=(int value) {
//     for (int i=0; i<qbyte_t::size; i++) {
//         if ((value >> i) & 1) {
//             vector<qbit*> qbits_to_increment(qbits.begin()+i, qbits.begin()+qbyte_t::size);
//             increment(qbits_to_increment);
//         }
//     }
// }

void qbyte_t::operator+=(qbyte_t& qbyte) {
    for (int i=0; i<qbyte_t::size; i++) {
        vector<qbit*> qbits_to_increment(qbits.begin()+i, qbits.begin()+qbyte_t::size);
        increment(qbits_to_increment, qbyte.qbits[i]);
    }
}

void qbyte_t::h() {
    for (int i=0; i<qbyte_t::size; i++) {
        qgate::h(qbits[i]);
    }
}



