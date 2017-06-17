//
// Created by Haotian on 17/6/5.
//

#include "IndexManager.h"

IndexManager::IndexManager() {

}

IndexManager::~IndexManager() {

}

bool IndexManager::create(string filename, int type) {
    // fixme !!! add char size later !!!
    int itemSize = MINISQL_BASE::getSize(type);
    int treeDegree = MINISQL_BASE::getDegree(type);
    switch (type) {
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

bool IndexManager::drop(string filename, int type) {
    switch (type) {
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

int IndexManager::search(string filename, string key, int type) {
    switch (type) {
        case MINISQL_TYPE_INT:
            return intIndexMap.find(filename)->second->find(std::atoi(key.c_str()));
        case MINISQL_TYPE_FLOAT:
            return floatIndexMap.find(filename)->second->find(std::atof(key.c_str()));
        case MINISQL_TYPE_CHAR:
            return charIndexMap.find(filename)->second->find(key);
        default:
            cerr << "Undefined type!" << endl;
            break;
    }
}

bool IndexManager::insert(string filename, string key, int offset, int type) {
    switch (type) {
        case MINISQL_TYPE_INT:
            return intIndexMap.find(filename)->second->insert(std::atoi(key.c_str()), offset);
        case MINISQL_TYPE_FLOAT:
            return floatIndexMap.find(filename)->second->insert(std::atof(key.c_str()), offset);
        case MINISQL_TYPE_CHAR:
            return charIndexMap.find(filename)->second->insert(key, offset);
        default:
            cerr << "Undefined type!" << endl;
            break;
    }
}

bool IndexManager::removeKey(string filename, string key, int type) {
    switch (type) {
        case MINISQL_TYPE_INT:
            return intIndexMap.find(filename)->second->remove(std::atoi(key.c_str()));
        case MINISQL_TYPE_FLOAT:
            return floatIndexMap.find(filename)->second->remove(std::atof(key.c_str()));
        case MINISQL_TYPE_CHAR:
            return charIndexMap.find(filename)->second->remove(key);
        default:
            cerr << "Undefined type!" << endl;
            break;
    }
}
