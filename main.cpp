#include <iostream>
#include <string>
#include <vector>
#include "lib/BPTree/BPTree.h"
using namespace std;


int main() {
    BPTree<int> bpTree("test.file", 4, 4);
    return 0;
}
