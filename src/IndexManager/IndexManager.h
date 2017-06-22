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
using namespace MINISQL_BASE;

class IndexManager {
public:
    IndexManager() = default;

    // fixme!!! destructor needs to be implemented to release memory of B+Tree instances
    ~IndexManager();
    bool create(string filename, SqlValueType type);
    bool drop(string filename, SqlValueType type);
    int search(string filename, Element &e);
    int searchNext(string filename);
    bool finishSearch(string filename);
    bool insert(string filename, Element &e, int offset);
    bool removeKey(string filename, Element &e);
private:
    typedef map<string, BPTree<int>*> intMap;
    typedef map<string, BPTree<float>*> floatMap;
    typedef map<string, BPTree<string>*> charMap;
    typedef map<string, BPTree::NodeSearchParse> offsetMap;

    intMap intIndexMap;
    floatMap floatIndexMap;
    charMap charIndexMap;
    offsetMap indexOffsetMap;

    BPTree<int> *intBPTree;
    BPTree<float> *floatBPTree;
    BPTree<string> *charBPTree;

    intMap::iterator intBPIterator;
    floatMap::iterator floatBPIterator;
    charMap::iterator charBPIterator;
};


#endif //MINISQL_INDEXMANAGER_H
