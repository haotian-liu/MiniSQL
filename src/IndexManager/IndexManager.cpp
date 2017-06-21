//
// Created by Haotian on 17/6/5.
//

#include "IndexManager.h"

IndexManager::~IndexManager() {

}

bool IndexManager::create(string filename, SqlValueType type) {
    int itemSize = type.getSize();
    int treeDegree = type.getDegree();
    switch (type.M()) {
        case MINISQL_TYPE_INT:
            intBPTree = new BPTree<int>(filename, itemSize, treeDegree);
            intIndexMap.insert(intMap::value_type(filename, intBPTree));
            break;
        case MINISQL_TYPE_FLOAT:
            floatBPTree = new BPTree<float>(filename, itemSize, treeDegree);
            floatIndexMap.insert(floatMap::value_type(filename, floatBPTree));
            break;
        case MINISQL_TYPE_CHAR:
            charBPTree = new BPTree<string>(filename, itemSize, treeDegree);
            charIndexMap.insert(charMap::value_type(filename, charBPTree));
            break;
        default:
            cerr << "Undefined type!" << endl;
            break;
    }
}

bool IndexManager::drop(string filename, SqlValueType type) {
    switch (type.M()) {
        case MINISQL_TYPE_INT:
            intBPIterator = intIndexMap.find(filename);
            delete intBPIterator->second;
            intIndexMap.erase(intBPIterator);
            break;
        case MINISQL_TYPE_FLOAT:
            floatBPIterator = floatIndexMap.find(filename);
            delete floatBPIterator->second;
            floatIndexMap.erase(floatBPIterator);
            break;
        case MINISQL_TYPE_CHAR:
            charBPIterator = charIndexMap.find(filename);
            delete charBPIterator->second;
            charIndexMap.erase(charBPIterator);
            break;
        default:
            cerr << "Undefined type!" << endl;
            break;
    }
}

int IndexManager::search(string filename, Element &e) {
    switch (e.type.M()) {
        case MINISQL_TYPE_INT:
            return intIndexMap.find(filename)->second->find(e.i);
        case MINISQL_TYPE_FLOAT:
            return floatIndexMap.find(filename)->second->find(e.r);
        case MINISQL_TYPE_CHAR:
            return charIndexMap.find(filename)->second->find(e.str);
        default:
            cerr << "Undefined type!" << endl;
            break;
    }
}

bool IndexManager::insert(string filename, Element &e, int offset) {
    switch (e.type.M()) {
        case MINISQL_TYPE_INT:
            return intIndexMap.find(filename)->second->insert(e.i, offset);
        case MINISQL_TYPE_FLOAT:
            return floatIndexMap.find(filename)->second->insert(e.r, offset);
        case MINISQL_TYPE_CHAR:
            return charIndexMap.find(filename)->second->insert(e.str, offset);
        default:
            cerr << "Undefined type!" << endl;
            break;
    }
}

bool IndexManager::removeKey(string filename, Element &e) {
    switch (e.type.M()) {
        case MINISQL_TYPE_INT:
            return intIndexMap.find(filename)->second->remove(e.i);
        case MINISQL_TYPE_FLOAT:
            return floatIndexMap.find(filename)->second->remove(e.r);
        case MINISQL_TYPE_CHAR:
            return charIndexMap.find(filename)->second->remove(e.str);
        default:
            cerr << "Undefined type!" << endl;
            break;
    }
}
