
#ifndef QBYTE_H
#define QBYTE_H

#include "qbit.h"


class qbyte_t {
    private:
        const static int size = 8;
    public:
        vector<qbit*> qbits;
        qbyte_t();
        qbyte_t(int value);
        ~qbyte_t();

        operator int();
        operator char();

        // void operator+=(int value);
        void operator+=(qbyte_t& qbyte);

        void h();
};

// void increment(vector<qbit*> qbits);
// void increment_condition(vector<qbit*> qbits, qbit* condition);

#endif // QBYTE_H

