#ifndef MINISQL_BUFFERMANAGER_H
#define MINISQL_BUFFERMANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include "../../include/DataStructure.h"

using namespace std;
using namespace MINISQL_BASE;

struct Block {
    string filename;
    int id; // id in buffer list
    unsigned int blockID; // block offset
    bool dirty; // whether need to write to disk
    bool busy; // whether is free or not
    int LRU_count;
    char *address; // ? what is the use
    char content[BlockSize];

    Block() { reset(); }

    void reset() {
        dirty = busy = false;
        memset(content, 0, BlockSize);
        address = content;
    }

    void mark(string filename, unsigned int blockID, int ID) {
        this->filename = filename;
        this->blockID = blockID;
        this->id = ID;
    }

    Block &flush() {
        fstream fp;
        fp.open(filename, ios::in | ios::out | ios::app | ios::binary);
        fp.seekg(ios::beg + blockID * BlockSize);
        fp.write(content, BlockSize);
        fp.close();
        return *this;
    }
};


class BufferManager {
public:
    BufferManager();

    ~BufferManager() = default;

    unsigned int getBlockTail(string filename);

    void setDirty(unsigned int offset); // fixme parameter is wrong!!

    char *getBlock(string filename, unsigned int offset, bool allocate = false);

    void flush_all_blocks(); // write all content in block to disk

    void createFile(string);

    Block &getLRU();

    void removeFile(string filename);

    void setFree(int id);

    void mark_block(int ind, string filename, unsigned int offset);

private:
    std::vector<Block> blockBuffer;

    void setBusy(int id); // mark as using

    // find block id which is available
    int getFreeBlockId();

    int get_LRU();

    int maxLRU;
};

#endif //MINISQL_BUFFERMANAGER_H