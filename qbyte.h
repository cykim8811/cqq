
#ifndef QBYTE_H
#define QBYTE_H

#include "qbit.h"
#include "qcircuit.h"
#include <vector>
#include <numeric>


QCircuit increment(int start, int end, int anc_idx, int cond_idx) {
    QCircuit circuit;
    circuit.cx(cond_idx, anc_idx);

    for (int i=start; i<end-1; i++) {
        circuit.mcx({cond_idx, anc_idx}, i);

        vector<int> mask(i-start);
        std::iota(mask.begin(), mask.end(), start);
        circuit.x(mask);

        mask.push_back(i);
        mask.push_back(cond_idx);

        circuit.mcx(mask, anc_idx);

        mask.pop_back();
        mask.pop_back();

        circuit.x(mask);
    }
    circuit.mcx({cond_idx, anc_idx}, end-1);
    
    vector<int> mask1((end-start)-1);
    std::iota(mask1.begin(), mask1.end(), start);
    circuit.x(mask1);
    if (end-start > 1) {
        vector<int> mask(end-start - 1);
        std::iota(mask.begin(), mask.end(), start);
        mask.push_back(cond_idx);
        circuit.mcx(mask, anc_idx);
    } else {
        circuit.cx(cond_idx, anc_idx);
    }
    circuit.x(mask1);
    
    
    return circuit;
}

QCircuit add(int target_start, int target_end, int value_start, int value_end, int anc1_idx, int anc2_idx, int cond_idx) {
    QCircuit circuit;
    for (int i=0; i<value_end-value_start; i++) {
        circuit.mcx({value_start + i, cond_idx}, anc1_idx);
        circuit.add_gate(make_unique<QCircuit>(increment(target_start + i, target_end, anc2_idx, anc1_idx)));
        circuit.mcx({value_start + i, cond_idx}, anc1_idx);
    }
    return circuit;
}

QCircuit swap(int a, int b, int count, int cond_idx) {
    QCircuit circuit;
    for (int i=0; i<count; i++) {
        circuit.mcx({cond_idx, a+i}, b+i);
        circuit.mcx({cond_idx, b+i}, a+i);
        circuit.mcx({cond_idx, a+i}, b+i);
    }
    return circuit;
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

