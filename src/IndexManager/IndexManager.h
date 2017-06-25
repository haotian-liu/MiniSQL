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

    bool create(const string &filename, const SqlValueType &type);

    bool drop(const string &filename, const SqlValueType &type);

    int search(const string &filename, const Element &e);

    int searchHead(const string &filename, int attrType);

    int searchNext(const string &filename, int attrType);

    bool finishSearch(const string &filename, int attrType);

    bool insert(const string &filename, const Element &e, int offset);

    bool removeKey(const string &filename, const Element &e);

private:
    typedef map<string, BPTree<int> *> intMap;
    typedef map<string, BPTree<float> *> floatMap;
    typedef map<string, BPTree<string> *> charMap;
    typedef map<string, NodeSearchParse<int>> intOMap;
    typedef map<string, NodeSearchParse<float>> floatOMap;
    typedef map<string, NodeSearchParse<string>> charOMap;

    intMap intIndexMap;
    floatMap floatIndexMap;
    charMap charIndexMap;
    intOMap intOffsetMap;
    floatOMap floatOffsetMap;
    charOMap charOffsetMap;

    BPTree<int> *intBPTree;
    BPTree<float> *floatBPTree;
    BPTree<string> *charBPTree;

    intMap::iterator intBPIterator;
    floatMap::iterator floatBPIterator;
    charMap::iterator charBPIterator;
};


#endif //MINISQL_INDEXMANAGER_H
