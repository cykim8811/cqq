
#include <iostream>
#include <vector>
#include <algorithm>
#include "quantum.h"

using namespace std;

struct History {
    int id;
    shared_ptr<QGate> gate;
    bool removed;
    History(int id, shared_ptr<QGate> gate) : id(id), gate(gate), removed(false) {};
};

class QContext {
public:
    QMemory memory;
    vector<History> history;
    int max_id = 0;
    QContext(int size) : memory(size), history(), max_id(0) {};

    int forward(shared_ptr<QGate> gate) {
        memory.forward(gate);
        history.push_back(History(max_id, gate));
        return max_id++;
    };

    void backward(int id) {
        auto it = find_if(history.begin(), history.end(), [id](History h) { return h.id == id; });
        if (it == history.end()) {
            throw invalid_argument("No such gate with id " + to_string(id));
        }

        // iter until 'it', and if the history is 'removed', apply forward
        for (auto i = history.begin(); i != it; i++) {
            if (i->removed) {
                memory.forward(i->gate);
            }
        }

        // apply backward
        memory.backward(it->gate);
        it->removed = true;

        // backward the forwarded gates
        for (auto i = (it - 1); i != history.begin() - 1; i--) {
            if (i->removed) {
                memory.backward(i->gate);
            }
        }
    };
};


int main() {
    QContext qc(2);
    int idx0 = qc.forward(make_shared<HGate>(0));
    int idx1 = qc.forward(make_shared<CXGate>(0, 1));

    qc.backward(idx0);
    qc.backward(idx1);

    qc.memory._display();

}



