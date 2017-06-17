//
// Created by Haotian on 17/6/5.
//

#ifndef MINISQL_INDEXMANAGER_H
#define MINISQL_INDEXMANAGER_H

#include <string>
#include <map>
#include "../../include/DataStructure.h"
#include "../../lib/BPTree/BPTree.h"
using namespace std;

class IndexManager {
public:
    IndexManager();
    ~IndexManager();
    bool create(string filename, int type);
    bool drop(string filename, int type);
    int search(string filename, string key, int type);
    bool insert(string filename, string key, int offset, int type);
    bool removeKey(string filename, string key, int type);
private:
    typedef map<string, BPTree<int>*> intMap;
    typedef map<string, BPTree<float>*> floatMap;
    typedef map<string, BPTree<string>*> charMap;

    intMap intIndexMap;
    floatMap floatIndexMap;
    charMap charIndexMap;
};


#endif //MINISQL_INDEXMANAGER_H
