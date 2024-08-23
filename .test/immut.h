
#include <iostream>

using namespace std;

class var {
public:
    int value;
    var(): value(0) {}
    ~var() {
        if (value != 0) {
            cerr << "\033[1;31mError: value is not equal to zero\033[0m" << endl;
        }
    }
    operator int() const {
        return value;
    }
};



// var* addi(var *a, int b) {
//     var *c = new var();
//     c->value = a->value + b;
//     return c;
// }


// int main() {
//     var *a = new var();
//     var *b = new var();
//     var *a1 = addi(a, 3);
//     var *b1 = addi(b, 5);
//     var *c = add(a1, b1);
//     var *d = cpy(c);   // xor cpy
//     delete c;               // undo addition
//     delete b;               // undo creation
//     delete a;               // undo creation
    
//     delete d;               // create a, create b, c = add (a, b), xorcpy, delete c, delete b, delete a
// }


