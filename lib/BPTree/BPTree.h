//
// Created by Haotian on 17/6/5.
//

#ifndef MINISQL_BPTREE_H
#define MINISQL_BPTREE_H

#include <iostream>
#include <vector>
#include <string>
#include "../../include/DataStructure.h"

using namespace std;

template<typename T>
class BPTreeNode {
public:
    BPTreeNode() = default;
    BPTreeNode(int degree, bool isLeaf);

    ~BPTreeNode() {}

    bool search(const T &key, int &index) const;
    BPTreeNode *split(T &key);
    int add(const T &key);
    int add(const T &key, int offset);
    void removeAt(int index);
    bool isRoot() const { return parent == nullptr; }

    bool isLeaf;
    int degree, cnt;
    BPTreeNode *parent, *sibling;
    vector<T> keys;
    vector<int> keyOffset;
    vector<BPTreeNode<T>*> children;

    void debug(int id) {
        cout << "Keys [" << id << "]: ";
        for (int i=0; i<this->cnt; i++) {
            cout << keys[i] << " ";
        }
        cout << endl;
    }

private:
    bool binarySearch(const T &key, int &index) const;
};

template<typename T>
BPTreeNode<T>::BPTreeNode(int degree, bool isLeaf) : degree(degree), isLeaf(isLeaf), cnt(0), parent(nullptr),
                                                  sibling(nullptr) {
    children.resize(degree + 1);
    keys.resize(degree);
    keyOffset.resize(degree);
}

template<typename T>
bool BPTreeNode<T>::search(const T &key, int &index) const {
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
bool BPTreeNode<T>::binarySearch(const T &key, int &index) const {
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
BPTreeNode<T> *BPTreeNode<T>::split(T &key) {
    BPTreeNode<T>* newNode = new BPTreeNode<T>(degree, isLeaf);
    int minimal = (degree - 1) / 2;

    if (isLeaf) {
        key = keys[minimal + 1];
        for (int i=minimal + 1; i<degree; i++) {
            newNode->keys[i - minimal - 1] = keys[i];
            newNode->keyOffset[i - minimal - 1] = keyOffset[i];
        }
        newNode->sibling = this->sibling;
        this->sibling = newNode;
        this->cnt = minimal + 1;
    } else {
        key = keys[minimal];
        for (int i=minimal + 1; i<=degree; i++) {
            newNode->children[i - minimal - 1] = this->children[i];
            this->children[i]->parent = newNode;
            this->children[i] = nullptr;
        }
        for (int i=minimal + 1; i<degree; i++) {
            newNode->keys[i - minimal - 1] = keys[i];
        }
        this->cnt = minimal;
    }
    newNode->parent = this->parent;
    newNode->cnt = degree - minimal - 1;
    return newNode;
}

template<typename T>
int BPTreeNode<T>::add(const T &key) {
    int index;
    bool keyExists = search(key, index);
    if (keyExists) {
        cerr << "Key is not unique!" << endl;
        exit(10);
    }
    for (int i=cnt; i>index; i--) {
        keys[i] = keys[i-1];
        children[i+1] = children[i];
    }
    keys[index] = key;
    children[index + 1] = nullptr;
    cnt++;
    return index;
}

template<typename T>
int BPTreeNode<T>::add(const T &key, int offset) {
    int index;
    bool keyExists = search(key, index);
    if (keyExists) {
        cerr << "Key is not unique!" << endl;
        exit(10);
    }
    for (int i=cnt; i>index; i--) {
        keys[i] = keys[i-1];
        keyOffset[i] = keyOffset[i-1];
    }
    keys[index] = key;
    keyOffset[index] = offset;
    cnt++;
    return index;
}

template<typename T>
void BPTreeNode<T>::removeAt(int index) {
    for (int i=index; i<cnt-1; i++) {
        keys[i] = keys[i+1];
    }
    if (isLeaf) {
        for (int i=index; i<cnt-1; i++) {
            keyOffset[i] = keyOffset[i+1];
        }
        keyOffset[cnt - 1] = 0;
        keys[cnt - 1] = T();
    } else {
        for (int i=index + 1; i<cnt; i++) {
            children[i] = children[i+1];
        }
        keys[cnt - 1] = T();
        children[cnt] = nullptr;
    }
    cnt--;
}

template<typename T>
struct NodeSearchParse {
    int index;
    BPTreeNode<T>* node;
};

template<typename T>
class BPTree {
public:
    typedef BPTreeNode<T>* TreeNode;

    BPTree(string fileName, int sizeofKey, int degree);
    ~BPTree();

    TreeNode getHeadNode() const { return head; }
    int find(const T &key);
    NodeSearchParse<T> findNode(const T &key);
    bool insert(const T &key, int offset);
    bool remove(const T &key);

private:
    string fileName;
    TreeNode root, head;
    int sizeofKey, level, keyCount, nodeCount, degree;

    void initBPTree();
    bool findKeyFromNode(TreeNode node, const T &key, NodeSearchParse<T> &res);
    void cascadeInsert(TreeNode node);
    bool cascadeDelete(TreeNode node);

    bool deleteBranchLL(TreeNode node, TreeNode parent, TreeNode sibling, int index);
    bool deleteBranchLR(TreeNode node, TreeNode parent, TreeNode sibling, int index);
    bool deleteBranchRL(TreeNode node, TreeNode parent, TreeNode sibling, int index);
    bool deleteBranchRR(TreeNode node, TreeNode parent, TreeNode sibling, int index);

    bool deleteLeafLL(TreeNode node, TreeNode parent, TreeNode sibling, int index);
    bool deleteLeafLR(TreeNode node, TreeNode parent, TreeNode sibling, int index);
    bool deleteLeafRL(TreeNode node, TreeNode parent, TreeNode sibling, int index);
    bool deleteLeafRR(TreeNode node, TreeNode parent, TreeNode sibling, int index);

    void debug(TreeNode node, int id) {
        node->debug(id);
        if (!node->isLeaf) {
            for (int i=0; i<=node->cnt; i++) {
                debug(node->children[i], i);
            }
        }
    }
};

template<typename T>
BPTree<T>::BPTree(string fileName, int sizeofKey, int degree) : fileName(fileName), sizeofKey(sizeofKey), degree(degree), keyCount(0), nodeCount(0), level(0), root(
        nullptr), head(nullptr) {
    initBPTree();
}

template<typename T>
BPTree<T>::~BPTree() {

}

template<typename T>
void BPTree<T>::initBPTree() {
    root = new BPTreeNode<T>(degree, true);
    keyCount = 0;
    level = 1;
    nodeCount = 1;
    head = root;
}

template<typename T>
bool BPTree<T>::findKeyFromNode(TreeNode node, const T &key, NodeSearchParse<T> &res) {
    int index;
    if (node->search(key, index)) {
        if (node->isLeaf) {
            res.index = index;
        } else {
            node = node->children[index + 1];
            while (!node->isLeaf) { node = node->children[0]; }
            res.index= 0;
        }
        res.node = node;
        return true;
    } else {
        if (node->isLeaf) {
            res.node = node;
            res.index = index;
            return false;
        } else {
            return findKeyFromNode(node->children[index], key, res);
        }
    }
}

template<typename T>
int BPTree<T>::find(const T &key) {
    NodeSearchParse<T> res;
    if (!root) { return -1; }
    if (findKeyFromNode(root, key, res)) { return res.node->keyOffset[res.index]; }
    else { return -1; }
}

template<typename T>
NodeSearchParse<T> BPTree<T>::findNode(const T &key) {
    NodeSearchParse<T> res;
    if (!root) { return res; }
    if (findKeyFromNode(root, key, res)) { return res; }
    else { return res; }
}

template<typename T>
bool BPTree<T>::insert(const T &key, int offset) {
    NodeSearchParse<T> res;
    if (!root) { initBPTree(); }
    if (findKeyFromNode(root, key, res)) {
        cerr << "Insert duplicate key!" << endl;
        return false;
    }
    res.node->add(key, offset);
    if (res.node->cnt == degree) {
        cascadeInsert(res.node);
    }
    keyCount++;
    return true;
}

template<typename T>
void BPTree<T>::cascadeInsert(BPTree::TreeNode node) {
    T key;
    TreeNode sibling = node->split(key);
    nodeCount++;

    if (node->isRoot()) {
        TreeNode root = new BPTreeNode<T>(degree, false);
        level++;
        nodeCount++;
        this->root = root;
        node->parent = root;
        sibling->parent = root;
        root->add(key);
        root->children[0] = node;
        root->children[1] = sibling;
    } else {
        TreeNode parent = node->parent;
        int index = parent->add(key);

        parent->children[index + 1] = sibling;
        sibling->parent = parent;
        if (parent->cnt == degree) {
            cascadeInsert(parent);
        }
    }
}

template<typename T>
bool BPTree<T>::remove(const T &key) {
    NodeSearchParse<T> res;
    if (!root) {
        cerr << "Dequeuing empty BPTree!" << endl;
        return false;
    }
    if (!findKeyFromNode(root, key, res)) {
        cerr << "Key not found!" << endl;
        return false;
    }
    if (res.node->isRoot()) {
        res.node->removeAt(res.index);
        keyCount--;
        return cascadeDelete(res.node);
    } else {
        if (res.index == 0 && head != res.node) {
            // cascadingly update parent node
            int index;
            TreeNode currentParent = res.node->parent;
            bool keyFound = currentParent->search(key, index);
            while (!keyFound) {
                if (!currentParent->parent) { break; }
                currentParent = currentParent->parent;
                keyFound = currentParent->search(key, index);
            }
            currentParent->keys[index] = res.node->keys[1];
            res.node->removeAt(res.index);
            keyCount--;
            return cascadeDelete(res.node);
        } else {
            res.node->removeAt(res.index);
            keyCount--;
            return cascadeDelete(res.node);
        }
    }
}

template<typename T>
bool BPTree<T>::cascadeDelete(BPTree::TreeNode node) {
    int minimal = degree / 2, minimalBranch = (degree - 1) / 2;
    if ((node->isLeaf && node->cnt >= minimal) // leaf node
        || (node->isRoot() && node->cnt) // root node
        || (!node->isLeaf && !node->isRoot() && node->cnt >= minimal) // branch node
            ) {
        return true; // no need to update
    }

    if (node->isRoot()) {
        if (root->isLeaf) {
            // tree completely removed
            root = nullptr;
            head = nullptr;
        } else {
            // reduce level by one
            root = node->children[0];
            root->parent = nullptr;
        }
        delete node;
        nodeCount--;
        level--;
        return true;
    }


    TreeNode currentParent = node->parent, sibling;
    int index;

    if (node->isLeaf) {
        // merge if it is leaf node
        currentParent->search(node->keys[0], index);
        if (currentParent->children[0] != node && currentParent->cnt == index + 1) {
            // rightest, also not first, merge with left sibling
            sibling = currentParent->children[index];
            if (sibling->cnt > minimal) {
                // transfer rightest of left to the leftest to meet the requirement
                return deleteLeafLL(node, currentParent, sibling, index);
            } else {
                // have to merge and cascadingly merge
                return deleteLeafLR(node, currentParent, sibling, index);
            }
        } else {
            // can merge with right brother
            if (currentParent->children[0] == node) {
                // on the leftest
                sibling = currentParent->children[1];
            } else {
                // normally
                sibling = currentParent->children[index + 2];
            }
            if (sibling->cnt > minimal) {
                // add the leftest of sibling to the right
                return deleteLeafRL(node, currentParent, sibling, index);
            } else {
                // merge and cascadingly delete
                return deleteLeafRR(node, currentParent, sibling, index);
            }
        }
    } else {
        // merge if it is branch node
        currentParent->search(node->children[0]->keys[0], index);
        if (currentParent->children[0] != node && currentParent->cnt == index + 1) {
            // can only be updated with left sibling
            sibling = currentParent->children[index];
            if (sibling->cnt > minimalBranch) {
                // add rightest key to the first node to avoid cascade operation
                return deleteBranchLL(node, currentParent, sibling, index);
            } else {
                // delete this and merge
                return deleteBranchLR(node, currentParent, sibling, index);
            }
        } else {
            // update with right sibling
            if (currentParent->children[0] == node) {
                sibling = currentParent->children[1];
            } else {
                sibling = currentParent->children[index + 2];
            }

            if (sibling->cnt > minimalBranch) {
                // add first key of sibling to the right
                return deleteBranchRL(node, currentParent, sibling, index);
            } else {
                // merge the sibling to current node
                return deleteBranchRR(node, currentParent, sibling, index);
            }
        }
    }
}

template<typename T>
bool BPTree<T>::deleteBranchLL(BPTree::TreeNode node, BPTree::TreeNode parent, BPTree::TreeNode sibling, int index) {
    node->children[node->cnt + 1] = node->children[node->cnt];
    for (int i=node->cnt; i>0; i--) {
        node->children[i] = node->children[i-1];
        node->keys[i] = node->keys[i-1];
    }
    node->children[0] = sibling->children[sibling->cnt];
    node->keys[0] = parent->keys[index];
    parent->keys[index] = sibling->keys[sibling->cnt - 1];
    node->cnt++;
//                !!!! fix this
//                if(sibling->children[sibling->cnt])
    sibling->children[sibling->cnt]->parent = node;
    sibling->removeAt(sibling->cnt - 1);
    return true;
}

template<typename T>
bool BPTree<T>::deleteBranchLR(BPTree::TreeNode node, BPTree::TreeNode parent, BPTree::TreeNode sibling, int index) {
    sibling->keys[sibling->cnt] = parent->keys[index]; // add one node
    parent->removeAt(index);
    sibling->cnt++;
    for (int i=0; i<node->cnt; i++) {
        node->children[i]->parent = sibling;
        sibling->children[sibling->cnt + i] = node->children[i];
        sibling->keys[sibling->cnt + i] = node->keys[i];
    }
    // rightest children
    sibling->children[sibling->cnt + node->cnt] = node->children[node->cnt];
    sibling->children[sibling->cnt + node->cnt]->parent = sibling;
    sibling->cnt += node->cnt;

    delete node;
    nodeCount--;

    return cascadeDelete(parent);
}

template<typename T>
bool BPTree<T>::deleteBranchRL(BPTree::TreeNode node, BPTree::TreeNode parent, BPTree::TreeNode sibling, int index) {
    sibling->children[0]->parent = node;
    node->children[node->cnt + 1] = sibling->children[0];
    node->keys[node->cnt] = sibling->children[0]->keys[0];
    node->cnt++;

    if (node == parent->children[0]) {
        parent->keys[0] = sibling->keys[0];
    } else {
        parent->keys[index + 1] = sibling->keys[0];
    }

    sibling->children[0] = sibling->children[1];
    sibling->removeAt(0);
    return true;
}

template<typename T>
bool BPTree<T>::deleteBranchRR(BPTree::TreeNode node, BPTree::TreeNode parent, BPTree::TreeNode sibling, int index) {

    node->keys[node->cnt] = parent->keys[index];
    if (node == parent->children[0]) {
        parent->removeAt(0);
    } else {
        parent->removeAt(index + 1);
    }
    node->cnt++;
    for (int i=0; i<sibling->cnt; i++) {
        sibling->children[i]->parent = node;
        node->children[node->cnt + i] = sibling->children[i];
        node->keys[node->cnt + i] = sibling->keys[i];
    }
    // rightest child
    sibling->children[sibling->cnt]->parent = node;
    node->children[node->cnt + sibling->cnt] = sibling->children[sibling->cnt];
    node->cnt += sibling->cnt;

    delete sibling;
    nodeCount--;

    return cascadeDelete(parent);
}

template<typename T>
bool BPTree<T>::deleteLeafLL(BPTree::TreeNode node, BPTree::TreeNode parent, BPTree::TreeNode sibling, int index) {
    for (int i=node->cnt; i>0; i--) {
        node->keys[i] = node->keys[i-1];
        node->keyOffset[i] = node->keyOffset[i-1];
    }
    node->keys[0] = sibling->keys[sibling->cnt - 1];
    node->keyOffset[0] = sibling->keyOffset[sibling->cnt - 1];
    sibling->removeAt(sibling->cnt - 1);

    node->cnt++;
    parent->keys[index] = node->keys[0];

    return true;
}

template<typename T>
bool BPTree<T>::deleteLeafLR(BPTree::TreeNode node, BPTree::TreeNode parent, BPTree::TreeNode sibling, int index) {
    parent->removeAt(index);
    for (int i=0; i<node->cnt; i++) {
        sibling->keys[i + sibling->cnt] = node->keys[i];
        sibling->keyOffset[i + sibling->cnt] = node->keyOffset[i];
    }
    sibling->cnt += node->cnt;
    sibling->sibling = node->sibling;

    delete node;
    nodeCount--;

    return cascadeDelete(parent);
}

template<typename T>
bool BPTree<T>::deleteLeafRL(BPTree::TreeNode node, BPTree::TreeNode parent, BPTree::TreeNode sibling, int index) {
    node->keys[node->cnt] = sibling->keys[0];
    node->keyOffset[node->cnt] = sibling->keyOffset[0];
    node->cnt++;
    sibling->removeAt(0);
    if (parent->children[0] == node) {
        parent->keys[0] = sibling->keys[0]; // if it is leftest, change key at index zero
    } else {
        parent->keys[index + 1] = sibling->keys[0]; // or next sibling should be updated
    }
    return true;
}

template<typename T>
bool BPTree<T>::deleteLeafRR(BPTree::TreeNode node, BPTree::TreeNode parent, BPTree::TreeNode sibling, int index) {
    for (int i=0; i<sibling->cnt; i++) {
        node->keys[node->cnt + i] = sibling->keys[i];
        node->keyOffset[node->cnt + i] = sibling->keyOffset[i];
    }
    if (node == parent->children[0]) {
        parent->removeAt(0); // if leftest, merge with first sibling
    } else {
        parent->removeAt(index + 1); // or merge with next
    }
    node->cnt += sibling->cnt;
    node->sibling = sibling->sibling;
    delete sibling;
    nodeCount--;
    return cascadeDelete(parent);
}

#endif //MINISQL_BPTREE_H
