#include <cstring>
#include <sys/stat.h>

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
    struct stat st;
    if (stat(filename.c_str(), &st) == 0) {
        return (st.st_size / BlockSize - 1);
    }
    cerr << "Failed to get file tail" << endl;
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
    for (auto &block : blockBuffer) {
        if (block.filename == filename) {
            block.reset();
        }
    }
    if (remove(filename.c_str())) {
        cerr << "Fail to remove file: " << filename << endl;
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
    fp.open(filename, ios::in | ios::out | ios::binary);
    if (!fp.good())
        cerr << "Fail to open file: " << filename << "." << endl;
    fp.seekg(ios_base::end);
    //int blockOffset = fp.tellg() / BlockSize;
    int blockOffset = getBlockTail(filename) + 1;
    cout << "Detected blockOffset: " << blockOffset << endl;
    if (offset >= blockOffset) {
        if (!allocate) { return nullptr; }
        if (blockOffset != offset) {
            cerr << fp.tellg() << " " << blockOffset << " " << offset << endl;
            cerr << "Requesting way beyond the tail!" << endl;
            return nullptr;
        }
        cout << "Requesting new block..." << endl;
    }

    Block &block = getFreeBlock();
    block.mark(filename, offset);
    blockMap.insert(TypeBlockMap::value_type(make_pair(filename, offset), block));

    fp.seekg(offset * BlockSize, ios::beg);
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