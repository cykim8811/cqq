
#ifndef QBYTE_H
#define QBYTE_H

#include "qbit.h"

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


template <int N>
class QuantumRegister {
    private:
        const static int size = N;
    public:
        vector<qbit*> qbits;
        QuantumRegister() {
            for (int i=0; i<QuantumRegister<N>::size; i++) {
                qbits.push_back(new qbit(0));
            }
        }
        QuantumRegister(QuantumRegister<N>& other) {
            for (int i=0; i<QuantumRegister<N>::size; i++) {
                qbits.push_back(new qbit(0));
            }
            *this = other;
        }
        QuantumRegister(int value) {
            for (int i=0; i<QuantumRegister<N>::size; i++) {
                qbits.push_back(new qbit((value >> i) & 1));
            }
        }
        ~QuantumRegister() {
            for (int i=0; i<QuantumRegister<N>::size; i++) {
                delete qbits[i];
            }
        }

        operator int() {
            int value = 0;
            for (int i=0; i<QuantumRegister<N>::size; i++) {
                value |= (qgate::measure(qbits[i]) << i);
            }
            return value;
        }

        void operator=(int value) {
            for (int i=0; i<QuantumRegister<N>::size; i++) {
                int res = qgate::measure(qbits[i]);
                if (res != ((value >> i) & 1)) {
                    qgate::x(qbits[i]);
                }
            }
        }

        void operator=(QuantumRegister<N>& other) {
            *this = 0;
            for (int i=0; i<QuantumRegister<N>::size; i++) {
                qgate::cx(other.qbits[i], qbits[i]);
            }
        }

        void operator+=(int value) {
            for (int i=0; i<QuantumRegister<N>::size; i++) {
                if ((value >> i) & 1) {
                    vector<qbit*> qbits_to_increment(qbits.begin()+i, qbits.begin()+QuantumRegister<N>::size);
                    increment(qbits_to_increment);
                }
            }
        }


        void operator+=(QuantumRegister<N>& other) {
            for (int i=0; i<QuantumRegister<N>::size; i++) {
                vector<qbit*> qbits_to_increment(qbits.begin()+i, qbits.begin()+QuantumRegister<N>::size);
                increment(qbits_to_increment, other.qbits[i]);
            }
        }
        
        void operator-=(int value) {
            *this += -value;
        }

        void operator-=(QuantumRegister<N>& other) {
            other.negate();
            *this += other;
            other.negate();
        }

        void operator^=(int value) {
            for (int i=0; i<QuantumRegister<N>::size; i++) {
                if ((value >> i) & 1) {
                    qgate::x(qbits[i]);
                }
            }
        }

        void operator^=(QuantumRegister<N>& other) {
            for (int i=0; i<QuantumRegister<N>::size; i++) {
                qgate::cx(other.qbits[i], qbits[i]);
            }
        }

        QuantumRegister<N> operator+(int value) {
            QuantumRegister<N> new_qbyte(*this);
            new_qbyte += value;
            return new_qbyte;
        }

        void negate() {
            for (int i=0; i<QuantumRegister<N>::size; i++) {
                qgate::x(qbits[i]);
            }
            increment(qbits);
        }

        void x() {
            for (int i=0; i<QuantumRegister<N>::size; i++) {
                qgate::x(qbits[i]);
            }
        }
        
        void z() {
            qgate::mcz(qbits);
        }

        void h() {
            for (int i=0; i<QuantumRegister<N>::size; i++) {
                qgate::h(qbits[i]);
            }
        }

        void _display() {
            qgate::entangle(qbits);
            qbits[0]->_display();
        }

        template <int TN>
        explicit operator QuantumRegister<TN>() {
            QuantumRegister<TN> new_qbyte(0);
            for (int i=0; i<min(QuantumRegister<N>::size, QuantumRegister<TN>::size); i++) {
                qgate::cx(qbits[i], new_qbyte.qbits[i]);
            }
            return new_qbyte;
        }
        
};

typedef QuantumRegister<8> qbyte_t;
typedef QuantumRegister<8> qchar_t;
typedef QuantumRegister<16> qshort_t;
typedef QuantumRegister<32> qint_t;
typedef QuantumRegister<64> qlong_t;
typedef QuantumRegister<64> qlonglong_t;

typedef QuantumRegister<1> qint1_t;
typedef QuantumRegister<2> qint2_t;
typedef QuantumRegister<4> qint4_t;
typedef QuantumRegister<8> qint8_t;
typedef QuantumRegister<16> qint16_t;
typedef QuantumRegister<64> qint64_t;

typedef QuantumRegister<1> qbool_t;
typedef QuantumRegister<1> qbit_t;
typedef QuantumRegister<2> qdibit_t;
typedef QuantumRegister<4> qnibl_t;



#endif // QBYTE_H

