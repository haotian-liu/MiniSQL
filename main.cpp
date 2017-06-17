#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "lib/BPTree/BPTree.h"
using namespace std;


int main() {
    BPTree<int> bpTree("test.file", 4, 4);
    const int testCnt = 10;
    bool t[testCnt] = {false};
    srand(time(NULL));
    for (int i=0; i<testCnt; i++) {
        int random;
//        random = rand() % testCnt;
        random = i;
        if (t[random]) continue;
        t[random] = true;
        bpTree.insert(random, testCnt - random);
    }


    for (int i=0; i<testCnt; i++) {
        if (!t[i]) continue;
        cout << i << " " << bpTree.find(i) << endl;
        bpTree.remove(i);
        if (i == 4) break;
    }
    return 0;
}
