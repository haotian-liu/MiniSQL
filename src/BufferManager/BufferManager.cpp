#include <cstring>

#include "BufferManager.h"
#include "../CatalogManager/CatalogManager.h"

using namespace std;

BufferManager::BufferManager() {
    blockBuffer.resize(MaxBlocks);
    maxLRU = 0;
    int id = 0;
    for (auto &block : blockBuffer) {
        block.id = id;
    }
}

unsigned int BufferManager::getBlockTail(string filename) {
    return 0;
}

void BufferManager::setDirty(unsigned int offset) {
    //
}

void BufferManager::setBusy(int id) {
    ++maxLRU;
    Block &block = blockBuffer[id];
    block.busy = true;
    block.LRU_count = maxLRU;
}

Block &BufferManager::getLRU() {
    int max = maxLRU;
    Block *detect = nullptr;
    for (auto &block : blockBuffer) {
        if (block.busy) { continue; }
        if (block.LRU_count <= max) {
            max = block.LRU_count;
            detect = &block;
        }
    }
    if (detect == nullptr) {
        cerr << "No LRU block found!";
    }
    return *detect;
}

// remove the buffer node with file instance
void BufferManager::removeFile(string filename) {
    const char *temp = filename.c_str();
    for (int i = 0; i < MaxBlocks; i++) {
        if (blockBuffer[i].filename == filename) {
            int total_size = CatalogManager::get_total_size_of_attr(filename);
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
int BufferManager::getFreeBlockId() {
    Block bb;
    for (auto &block : blockBuffer) {
        if (!block.dirty && !block.busy) {
            block.reset();
            setBusy(block.id);
            return block.blockID;
        }
    }

    bb = getLRU();
    bb.flush().reset();
    setBusy(bb.id);

    return bb.id;
}

char *BufferManager::getBlock(string filename, unsigned int offset, bool allocate) {
    unsigned int number, i, mark;
    for (int i = 0; i < MaxBlocks; i++) {
        if (blockBuffer[i].filename == filename &&
            blockBuffer[i].blockID == offset) {
            setBusy(i);
            return (char *) (&blockBuffer[i]);
        }
    }
    //如果该块不在buffer中 则从文件中读取
    i = getFreeBlockId();
    mark_block(i, filename, offset);

    fstream fp;
    fp.open(filename, ios::in | ios::out | ios::app | ios::binary);
    if (!fp.good())
        cerr << filename << "file open err" << endl;
    fp.seekg(ios::beg + offset * BlockSize);
    fp.read((char *) (blockBuffer[i].content), BlockSize);
    fp.close();
    setBusy(i);
    return blockBuffer[i].content;
}

void BufferManager::mark_block(int ind, string filename_in, unsigned int offset) {
    blockBuffer[ind].filename = filename_in;
    blockBuffer[ind].blockID = offset;
}

void BufferManager::setFree(int ind) {
    blockBuffer[ind].busy = false;
}

int BufferManager::get_LRU() {
    int max = -1, num = -1;
    for (int i = 0; i < MaxBlocks; i++) {
        if (blockBuffer[i].LRU_count > max) {
            max = blockBuffer[i].LRU_count;
            num = i;
        }
    }
    return num;
}

void BufferManager::flush_all_blocks() {
    for (auto &block : blockBuffer) {
        if (block.dirty) {
            block.flush();
        }
        block.reset();
    }
}