
#include <iostream>
#include <chrono>
#include <algorithm>
#include "qbyte.h"

using namespace std;
using namespace qgate;

void oracle(qbyte_t& a) {
    a -= 27;
    a.x();
    a.z();
    a.x();
    a += 27;
}

void diffuser(qbyte_t& a) {
    a.h();
    a.x();
    a.z();
    a.x();
    a.h();
}

int main() {
    auto start = chrono::high_resolution_clock::now();

    int shots = 20;

    vector<int> results(256, 0);

    for (int i=0; i<shots; i++) {
        qbyte_t a = 0;
        
        a.h();

        for (int i=0; i<13; i++) {
            oracle(a);
            diffuser(a);
        }

        results[(int)a]++;
    }


    vector<pair<int, int>> sortedResults;
    for (int i = 0; i < results.size(); i++) {
        sortedResults.push_back(make_pair(i, results[i]));
    }

    sort(sortedResults.begin(), sortedResults.end(), [](pair<int, int> a, pair<int, int> b) {
        return a.second > b.second;
    });

    for (int i = 0; i < min(10, (int)sortedResults.size()); i++) {
        if (sortedResults[i].second == 0) break;
        cout << sortedResults[i].first << ": " << sortedResults[i].second << endl;
    }

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    double dc = duration.count();
    if (dc > 1000) cout << "Time taken: " << dc / 1000.0 << " ms" << endl;
    else if (dc > 1000000) cout << "Time taken: " << dc / 1000000.0 << " s" << endl;
    else cout << "Time taken: " << dc << " µs" << endl;

    return 0;
}

