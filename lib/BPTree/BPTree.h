//
// Created by Haotian on 17/6/5.
//

#ifndef MINISQL_BPTREE_H
#define MINISQL_BPTREE_H

#include <iostream>
#include <vector>
#include <string>
using namespace std;

template <typename T>
struct BPTreeNode {
    BPTreeNode(int degree, bool isLeaf);
    ~BPTreeNode();
    bool isRoot() const { return parent == nullptr; }
    bool search(T key, int &index) const;
    BPTreeNode *split(T &key);
    int add(T &key);
    bool removeAt(int index);

    bool isLeaf;
    int degree, cnt;
    BPTreeNode *parent;
    vector<T> keys;
    vector<BPTreeNode> children;
};

template <typename T>
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
