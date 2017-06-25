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
    int ID; // ID in buffer list
    unsigned int blockID; // block offset
    bool dirty; // whether need to write to disk
    bool busy; // whether is free or not
    int LRU_count;
    char *address; // ? what is the use
    char content[BlockSize];

    void reset() {
        dirty = busy = false;
        LRU_count = 0;
        memset(content, 0, BlockSize);
    }

    void mark(string filename, unsigned int blockID, int ID) {
        this->filename = filename;
        this->blockID = blockID;
        this->ID = ID;
    }

    void flush() {
        fstream fp;
        fp.open(filename, ios::in | ios::out | ios::app | ios::binary);
        fp.seekg(ios::beg + blockID * BlockSize);
        fp.write(content, BlockSize);
        fp.close();
    }
};


class BufferManager {
public:
    BufferManager() = default;

    ~BufferManager() = default;

    unsigned int getBlockTail(string filename);
    void setDirty(unsigned int offset);

    char *getBlock(string filename, unsigned int offset, bool allocate = false);

    void flush_all_blocks(); // write all content in block to disk
    int get_LRU();
    void using_block(int ind);            //标记第i块block为正在使用
    void create_table(string);

    void removeFile(string filename);

    void setFree(int ind);                        //标记this->being_used为false，使用完成
    void mark_block(int ind, string filename, unsigned int offset);    //标记此块的filename和offset
private:
    std::vector<Block> blockBuffer;

    void clearBlock(int i);

    void initialize_blocks(); // initialize all blocks
    void setBusy(int ind); // mark as using

    // find block ID which is available
    unsigned int get_blank_block_ind();
    void flush_one_block(int ind); // flush block with ID
};

#endif //MINISQL_BUFFERMANAGER_H