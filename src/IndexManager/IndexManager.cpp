//
// Created by Haotian on 17/6/5.
//

#include "IndexManager.h"

IndexManager::~IndexManager() {

}

bool IndexManager::create(const string &filename, const SqlValueType &type) {
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

bool IndexManager::drop(const string &filename, const SqlValueType &type) {
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

int IndexManager::search(const string &filename, const Element &e) {
    NodeSearchParse<int> intNode;
    NodeSearchParse<float> floatNode;
    NodeSearchParse<string> charNode;
    switch (e.type.M()) {
        case MINISQL_TYPE_INT:
            intNode = intIndexMap.find(filename)->second->findNode(e.i);
            intOffsetMap[filename] = intNode;
            return intNode.node->keyOffset[intNode.index];
        case MINISQL_TYPE_FLOAT:
            floatNode = floatIndexMap.find(filename)->second->findNode(e.r);
            floatOffsetMap[filename] = floatNode;
            return floatNode.node->keyOffset[floatNode.index];
        case MINISQL_TYPE_CHAR:
            charNode = charIndexMap.find(filename)->second->findNode(e.str);
            charOffsetMap[filename] = charNode;
            return charNode.node->keyOffset[charNode.index];
        default:
            cerr << "Undefined type!" << endl;
            return -1;
    }
}

int IndexManager::searchNext(const string &filename, int attrType) {
    NodeSearchParse<int> intNode;
    NodeSearchParse<float> floatNode;
    NodeSearchParse<string> charNode;

    switch (attrType) {
        case MINISQL_TYPE_INT:
            intNode = intOffsetMap.find(filename)->second;
            intNode.index++;
            if (intNode.index == intNode.node->cnt) {
                intNode.node = intNode.node->sibling;
                intNode.index = 0;
            }
            intOffsetMap[filename] = intNode;
            if (intNode.node != nullptr) {
                return intNode.node->keyOffset[intNode.index];
            }
            break;
        case MINISQL_TYPE_FLOAT:
            floatNode = floatOffsetMap.find(filename)->second;
            floatNode.index++;
            if (floatNode.index == floatNode.node->cnt) {
                floatNode.node = floatNode.node->sibling;
                floatNode.index = 0;
            }
            floatOffsetMap[filename] = floatNode;
            if (floatNode.node != nullptr) {
                return floatNode.node->keyOffset[floatNode.index];
            }
            break;
        case MINISQL_TYPE_CHAR:
            charNode = charOffsetMap.find(filename)->second;
            charNode.index++;
            if (charNode.index == charNode.node->cnt) {
                charNode.node = charNode.node->sibling;
                charNode.index = 0;
            }
            charOffsetMap[filename] = charNode;
            if (charNode.node != nullptr) {
                return charNode.node->keyOffset[charNode.index];
            }
            break;
        default:
            cerr << "Undefined type!" << endl;
            return -1;
    }
    return -1;
}

bool IndexManager::finishSearch(const string &filename, int attrType) {
    switch (attrType) {
        case MINISQL_TYPE_INT:
            intOffsetMap.erase(filename);
            break;
        case MINISQL_TYPE_FLOAT:
            floatOffsetMap.erase(filename);
            break;
        case MINISQL_TYPE_CHAR:
            charOffsetMap.erase(filename);
            break;
        default:
            cerr << "Undefined type!" << endl;
            break;
    }
    return true;
}

bool IndexManager::insert(const string &filename, const Element &e, int offset) {
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

bool IndexManager::removeKey(const string &filename, const Element &e) {
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

int IndexManager::searchHead(const string &filename, int attrType) {
    NodeSearchParse<int> intNode;
    NodeSearchParse<float> floatNode;
    NodeSearchParse<string> charNode;
    switch (attrType) {
        case MINISQL_TYPE_INT:
            intNode.node = intIndexMap.find(filename)->second->getHeadNode();
            intNode.index = 0;
            intOffsetMap[filename] = intNode;
            return intNode.node->keyOffset[intNode.index];
        case MINISQL_TYPE_FLOAT:
            floatNode.node = floatIndexMap.find(filename)->second->getHeadNode();
            floatNode.index = 0;
            floatOffsetMap[filename] = floatNode;
            return floatNode.node->keyOffset[floatNode.index];
        case MINISQL_TYPE_CHAR:
            charNode.node = charIndexMap.find(filename)->second->getHeadNode();
            charNode.index = 0;
            charOffsetMap[filename] = charNode;
            return charNode.node->keyOffset[charNode.index];
        default:
            cerr << "Undefined type!" << endl;
            break;
    }
}
