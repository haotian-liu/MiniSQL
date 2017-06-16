//
// Created by Haotian on 17/6/5.
//

#ifndef MINISQL_BPTREE_H
#define MINISQL_BPTREE_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

template<typename T>
class BPTreeNode {
public:
    BPTreeNode(int degree, bool isLeaf);

    ~BPTreeNode() {}

    bool search(T key, int &index) const;
    BPTreeNode *split(T &key);
    int add(T &key);
    void removeAt(int index);
    bool isRoot() const { return parent == nullptr; }

    bool isLeaf;
    int degree, cnt;
    BPTreeNode *parent, *sibling;
    vector<T> keys;
    vector<int> keyOffset;
    vector<BPTreeNode> children;

private:
    bool binarySearch(T key, int &index) const;
};

BPTreeNode::BPTreeNode(int degree, bool isLeaf) : degree(degree), isLeaf(isLeaf), cnt(0), parent(nullptr),
                                                  sibling(nullptr) {
    children.resize(degree + 1);
    keys.resize(degree);
    keyOffset.resize(degree);
}

template<typename T>
bool BPTreeNode::search(T key, int &index) const {
    if (cnt == 0) {
        index = 0;
        return false;
    }
    if (keys[0] > key) {
        index = 0;
        return false;
    }
    if (keys[cnt - 1] < key) {
        index = cnt;
        return false;
    }
    return binarySearch(key, index);
}

template<typename T>
bool BPTreeNode::binarySearch(T key, int &index) const {
    int left = 0, right = cnt - 1, pos;
    while (left <= right) {
        pos = left + (right - left) / 2;
        if (keys[pos] < key) {
            left = pos + 1;
        } else {
            right = pos - 1;
        }
    }
    index = left;
    return keys[index] == key;
}

template<typename T>
BPTreeNode *BPTreeNode::split(T &key) {
    BPTreeNode* newNode = new BPTreeNode(degree, isLeaf);
    int minimal;

    if (isLeaf) {
        minimal = degree / 2;
        key = keys[minimal];
        for (int i=minimal; i<degree; i++) {
            newNode->keys[i - minimal] = keys[i];
            newNode->keyOffset[i - minimal] = keyOffset[i];
        }
        newNode->sibling = this->sibling;
        this->sibling = newNode;
    } else {
        minimal = (degree + 1) / 2;
        key = keys[minimal];
        for (int i=minimal; i<=degree; i++) {
            newNode->keys[i - minimal] = keys[i];
            newNode->children[i - minimal] = this->children[i];
            newNode->children[i - minimal]->parent = newNode;
            this->children[i] = nullptr;
        }
    }
    newNode->parent = this->parent;
    this->cnt = minimal;
    newNode->cnt = degree - minimal;
    return newNode;
}

template<typename T>
int BPTreeNode::add(T &key) {
    int index;
    bool keyExists = search(key, index);
    if (keyExists) {
        cerr << "Key is not unique!" << endl;
        exit(10);
    }
    for (int i=cnt; i>index; i++) {
        keys[i] = keys[i-1];
        children[i+1] = children[i];
    }
    keys[index] = key;
    children[index + 1] = nullptr;
    cnt++;
    return index;
}

void BPTreeNode::removeAt(int index) {
    for (int i=index; i<cnt-1; i++) {
        keys[i] = keys[i+1];
    }
    if (isLeaf) {
        for (int i=index; i<cnt-1; i++) {
            keyOffset[i] = keyOffset[i+1];
        }
    } else {
        for (int i=index + 1; i<cnt; i++) {
            children[i] = children[i+1];
        }
    }
    cnt--;
}

template<typename T>
class BPTree {
public:
    BPTree(string fileName, int sizeofKey, int degree);
    ~BPTree();

    int find(T &key);
    bool insert(T &key, int offset);
    bool remove(T &key);

private:
    string fileName;
    BPTreeNode root, leftHead;
    int sizeofKey, level, keyCount, nodeCount, degree;
};


#endif //MINISQL_BPTREE_H
