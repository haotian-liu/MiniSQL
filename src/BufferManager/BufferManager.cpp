#include <cstring>

#include "BufferManager.h"

using namespace std;

BufferManager::BufferManager() {
    blockBuffer.resize(MaxBlocks);
    maxLRU = 0;
    int id = 0;
    for (auto &block : blockBuffer) {
        block.id = id;
    }
}

int BufferManager::getBlockTail(string filename) {
    fstream fp;
    fp.open(filename, ios::in | ios::binary);
    if (!fp.good())
        cerr << filename << " open failed." << endl;
    fp.seekg(0, ios_base::end);
    int k = fp.tellg();
    return (k / BlockSize - 1);
}

void BufferManager::setDirty(const string &filename, unsigned int blockID) {
    Block &block = findBlockPair(filename, blockID);
    block.dirty = true;
}

void BufferManager::setBusy(int id) {
    ++maxLRU;
    Block &block = blockBuffer[id];
    block.busy = true;
    block.LRUCount = maxLRU;
}

Block &BufferManager::getLRU() {
    int max = maxLRU;
    Block *detect = nullptr;
    for (auto const &block : blockBuffer) {
        if (block.busy) { continue; }
        if (block.LRUCount <= max) {
            max = block.LRUCount;
            detect = const_cast<Block *>(&block);
        }
    }
    if (detect == nullptr) { cerr << "No LRU block found!"; }
    return *detect;
}

// remove the buffer node with file instance
void BufferManager::removeFile(string filename) {
    const char *temp = filename.c_str();
    for (int i = 0; i < MaxBlocks; i++) {
        if (blockBuffer[i].filename == filename) {
            blockBuffer[i].filename = nullptr;
            blockBuffer[i].dirty = false;
            blockBuffer[i].busy = false;
        }
    }
    if (remove(temp)) {
        printf("Could not delete the file &s /n", temp);
        exit(0);
    }
}

void BufferManager::createFile(string in) {
    ofstream f1(in);
}

// Find clean blocks, if failed use LRU replacement
Block &BufferManager::getFreeBlock() {
    for (auto &block : blockBuffer) {
        if (!block.dirty && !block.busy) {
            block.reset();
            setBusy(block.id);
            return block;
        }
    }

    Block &bb = getLRU();
    bb.flush().reset();
    setBusy(bb.id);

    return bb;
}

char *BufferManager::getBlock(string filename, unsigned int offset, bool allocate) {
    for (auto &block : blockBuffer) {
        if (block.filename == filename && block.blockID == offset) {
            setBusy(block.id);
            blockMap.insert(TypeBlockMap::value_type(make_pair(filename, offset), block));
            return block.content;
        }
    }

    fstream fp;
    fp.open(filename, ios::in | ios::out | ios::ate | ios::binary);
    if (!fp.good())
        cerr << filename << "file open err" << endl;
    fp.seekg(0, ios_base::end);
    int blockOffset = fp.tellg() / BlockSize;
    cout << "Detected blockOffset: " << blockOffset << endl;
    if (offset >= blockOffset) {
        if (!allocate) { return nullptr; }
        if (blockOffset != offset) {
            cerr << "Requesting way beyond the tail!" << endl;
            return nullptr;
        }
        cout << "Requesting new block..." << endl;
    }

    Block &block = getFreeBlock();
    block.mark(filename, offset);
    blockMap.insert(TypeBlockMap::value_type(make_pair(filename, offset), block));

    fp.seekg(ios::beg + offset * BlockSize);
    fp.read(block.content, BlockSize);
    fp.close();
    setBusy(block.id);
    return block.content;
}

void BufferManager::setFree(string filename, unsigned int blockID) {
    Block &block = findBlockPair(filename, blockID);
    block.busy = false;
    blockMap.erase(make_pair(block.filename, block.blockID));
}

Block &BufferManager::findBlockPair(string filename, unsigned int blockID) const {
    auto parent = blockMap.find(make_pair(filename, blockID));
    if (parent == blockMap.end()) {
        cerr << "Element not found!";
    }
    return parent->second;
}

void BufferManager::flushAllBlocks() {
    for (auto &block : blockBuffer) {
        if (block.dirty) {
            block.flush();
        }
        block.reset();
    }
}