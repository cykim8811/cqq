
#include "quantum.h"
#include "qubit.h"
#include "qubyte.h"
#include "qtransform.h"

#include <iostream>

using namespace std;

int main() {
    qbool_t b = 1;
    qbool_t c = h(b);

    

    c.transform->data->_display();

    
    return 0;
}

