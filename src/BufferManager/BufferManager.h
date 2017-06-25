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
    unsigned int ID; // ID in buffer list
    unsigned int blockID; // block offset
    bool dirty;    // whether need to write to disk
    bool busy; // whether is free or not
    int LRU_count;
    char *address;            //记录块的首地址,从文件读入数据时用
    char content[BlockSize];
};


class BufferManager {
public:
    BufferManager() = default;

    ~BufferManager() = default;

    unsigned int get_last_block(string filename) { return 0; } // dummy def for last block retrieval
    void setDirty(unsigned int offset) {} // dummy delete definition
    char *get_specified_block(string filename, int rank);

    char *getBlock(string filename, unsigned int offset, bool allocate = false);

    char *get_blank_block_addr();

    unsigned int get_blank_block_ind();
    //返回一个空的block（用于新增），类型为table_index

    void flush_all_blocks();                //把所有block的数据写回文件内并清空blocks
    void flush_one_block(int ind);            //把这个block的数据写回文件内并清空该block
    int get_LRU();                    //最近使用次数 返回LRU
    void using_block(int ind);            //标记第i块block为正在使用
    void create_table(string);

    void removeFile(string filename);

    void mark_being_used(int ind);                        //标记this->being_used为true，正在使用
    void end_being_used(int ind);                        //标记this->being_used为false，使用完成
    void mark_block(int ind, string filename, unsigned int offset);    //标记此块的filename和offset
private:
    std::vector<Block> blockBuffer;

    void clearBlock(int i);

    void initialize_blocks();                //初始化
};

#endif //MINISQL_BUFFERMANAGER_H