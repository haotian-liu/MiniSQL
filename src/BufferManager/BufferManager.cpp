#include <cstring>

#include "BufferManager.h"
#include "../CatalogManager/CatalogManager.h"

using namespace std;

void BufferManager::clearBlock(int i) {
    blockBuffer[i].reset();
}

unsigned int BufferManager::getBlockTail(string filename) {
    return 0;
}

void BufferManager::setDirty(unsigned int offset) {
    //
}

// remove the buffer node with file instance
void BufferManager::removeFile(string filename_in) {
    const char *temp = filename_in.c_str();
    for (int i = 0; i < MaxBlocks; i++) {
        if (blockBuffer[i].filename == filename_in) {
            int total_size = CatalogManager::get_total_size_of_attr(filename_in);
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

void BufferManager::createTable(string in) {
    ofstream f1(in);
}


void BufferManager::initialize_blocks() {
    for (int i = 0; i < MaxBlocks; i++)
        clearBlock(i);
}


//首先查找表中是否有空位写入
//如果没有空位 用LRU找一块block清除后写入
unsigned int BufferManager::get_blank_block_ind() {
    int i, res;
    char *p;
    for (i = 0; i < MaxBlocks; i++) {
        if (blockBuffer[i].filename != "")
            break;
    }
    if (i < MaxBlocks) //如果找到空块
    {
        res = i;
    } else if (i >= MaxBlocks) {
        res = get_LRU();
        flush_one_block(res);
    }
    using_block(res);
    setBusy(i);
    clearBlock(res);
    return res;
}

char *BufferManager::getBlock(string filename, unsigned int offset, bool allocate) {
    unsigned int number, i, mark;
    for (int i = 0; i < MaxBlocks; i++) {
        if (blockBuffer[i].filename == filename &&
            blockBuffer[i].blockID == offset) {
            using_block(i);
            return (char *) (&blockBuffer[i]);
        }
    }
    //如果该块不在buffer中 则从文件中读取
    i = get_blank_block_ind();
    mark_block(i, filename, offset);

    fstream fp;
    fp.open(filename, ios::in | ios::out | ios::app | ios::binary);
    if (!fp.good())
        cerr << filename << "file open err" << endl;
    fp.seekg(ios::beg + offset * BlockSize);
    fp.read((char *) (blockBuffer[i].content), BlockSize);
    fp.close();
    using_block(i);
    return (char *) (&blockBuffer[i]);
}

void BufferManager::mark_block(int ind, string filename_in, unsigned int offset) {
    blockBuffer[ind].filename = filename_in;
    blockBuffer[ind].blockID = offset;
}

void BufferManager::setBusy(int ind) {
    blockBuffer[ind].busy = true;
}

void BufferManager::setFree(int ind) {
    blockBuffer[ind].busy = false;
}

void BufferManager::using_block(int n) {
    blockBuffer[n].busy = true;
    for (int i = 0; i < MaxBlocks; i++) {
        if (i != n && !(blockBuffer[i].busy))     //不在被使用的块 count++
            blockBuffer[i].LRU_count++;
        else if (i == n)
            blockBuffer[i].LRU_count = 0;
    }
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

void BufferManager::flush_one_block(int ind) {
    blockBuffer[ind].flush();
}

void BufferManager::flush_all_blocks() {
    for (int i = 0; i < MaxBlocks; i++) {
        flush_one_block(i);
        if (blockBuffer[i].address != "\0")
            blockBuffer[i].address = nullptr;
        if (blockBuffer[i].filename != "\0")
            blockBuffer[i].filename = nullptr;
    }
}