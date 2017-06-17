//
// Created by Haotian on 17/6/5.
//

#include "IndexManager.h"

IndexManager::IndexManager() {

}

IndexManager::~IndexManager() {

}

bool IndexManager::create(string filename, int type) {
    return false;
}

bool IndexManager::drop(string filename, int type) {
    return false;
}

int IndexManager::search(string filename, string key, int type) {
    return 0;
}

bool IndexManager::insert(string filename, string key, int offset, int type) {
    return false;
}

bool IndexManager::removeKey(string filename, string key, int type) {
    return false;
}
