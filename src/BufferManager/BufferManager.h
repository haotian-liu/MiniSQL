#ifndef MINISQL_BUFFERMANAGER_H
#define MINISQL_BUFFERMANAGER_H
#define BLOCK_HEADER_SIZE 255	//待定 随便写的先
#define MINISQL_BLOCK_INDEX 1
#define MINISQL_BLOCK_TABLE 2
#define BLOCK_SIZE 4096
#define MAX_BLOCKS 5
#define FILE_BLOCK_SIZE (BLOCK_SIZE - BLOCK_HEADER_SIZE)

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/*实例化：调用initialize_blocks(); 得到ptr_buffer[0~MAX_BLOCKS],分别指向每一个block

/*插入*/
/*新建table或index的时候,首先 BufferManager::create_table(string filename),创建对应名称的文件
/*其次char *getFileBlock(string filename); 返回当前block的地址，可能未满可以根据used位来判断，如果已满：
/*用char* get_next_block_for_insert(); 获取下一块的block;如果为null，则需要重新申请一块block↓
/*用get_blank_block(unsigned int table_index); 返回空块的地址（如果没有就用LRU替换
/*
/*查找*/
/*根据文件名查找block的时候，用BufferManager::getFileBlock(string filename)，返回第一个储存该文件的block地址
/*while(没有找到) 调用BufferManager::get_next_block(string filename); 返回下一块储存该文件的block地址
*/
struct Block {
    string filename;				//记录块对应的表或索引的名字
    unsigned int ID;				//此block在buffer中的位置，可用ptr_buffer[ID]调用
    unsigned int offset;			//记录块对应的记录是文件中的第几块
    //块对应的是表，则值为TABLE;若是索引，则值为INDEX;若空为0
    bool is_written;				//该块是否被写过，用于判断是否需要写回
    bool being_used;				//记录该块是否正被占用
    int  LRU_count;					//记录该块最近使用的情况，用于LRU判断
    char* block_address;			//记录块的首地址,从文件读入数据时用
    char content[BLOCK_SIZE];
};


class BufferManager {
public:
    Block ptr_buffer[MAX_BLOCKS];	//遍历用的指针
    Block* ptr_for_insert;
    /**/void  clear_block(int i);
    unsigned int get_last_block(string filename) { return 0; } // dummy def for last block retrieval
    void setDirty(unsigned int offset) {} // dummy delete definition
    /**/char* get_specified_block(string filename, int rank);
    /**/int   get_block_for_insert(bool if_get_new_if_unavailable);
    /**/char* get_block(string filename, unsigned int offset, bool allocate=false);
    /**/char* get_blank_block_addr();
    /**/unsigned int get_blank_block_ind();
    //返回一个空的block（用于新增），类型为table_index

    /**/void flush_all_blocks();				//把所有block的数据写回文件内并清空blocks
    /**/void flush_one_block(int ind);			//把这个block的数据写回文件内并清空该block
    /**/void initialize_blocks();				//初始化
    /**/int  get_LRU();					//最近使用次数 返回LRU
    /**/void using_block(int ind);			//标记第i块block为正在使用
    void create_table(string);
    void removeFile(string filename);
    /**/void mark_is_written(int ind);						//标记this->is_written为true，被修改过
    /**/void mark_being_used(int ind);						//标记this->being_used为true，正在使用
    /**/void end_being_used(int ind);						//标记this->being_used为false，使用完成
    /**/void mark_block(int ind,string filename, unsigned int offset);	//标记此块的filename和offset
    BufferManager() = default;
    ~BufferManager() = default;
};

#endif //MINISQL_BUFFERMANAGER_H