#ifndef MINISQL_BUFFERMANAGER_H
#define MINISQL_BUFFERMANAGER_H
#define BLOCK_HEADER_SIZE 255	//待定 随便写的先
#define MINISQL_BLOCK_INDEX 1
#define MINISQL_BLOCK_TABLE 2
#define BLOCK_SIZE 4096
#define MAX_BLOCKS 50

#include <string>
#include <iostream>
#include <fstream>
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

class BufferManager {

private:
    string block_address;			//记录块的首地址
    string filename;				//记录块对应的表或索引的名字
    //int offset;						//记录块对应的记录在文件中的偏移量
    unsigned int ID;
    unsigned int offset;			//记录块对应的记录是文件中的第几块
    unsigned int index_table;		//判断该内存对应的是索引还是表
    //块对应的是表，则值为TABLE;若是索引，则值为INDEX;若空为0
    bool is_written;				//该块是否被写过，用于判断是否需要写回
    bool being_used;				//记录该块是否正被占用
    int  LRU_count;					//记录该块最近使用的情况，用于LRU判断
    char* content[BLOCK_SIZE];
public:
    static BufferManager *ptr_buffer;	//遍历用的指针
    static BufferManager* block_now;
    static BufferManager* block_for_insert;

public:
    BufferManager() = default;
    ~BufferManager() = default;
    char* get_specified_block(string filename, int rank);
    //char* get_next_block(string filename);
    //void setDirty(const char *dest);		//可以不用外部调用 buffer内部可做
    char* get_next_block_for_insert();
    char* get_block(string filename, unsigned int offset);
    char* get_blank_block_addr(unsigned int table_index);
    unsigned int get_blank_block_ind();
    //返回一个空的block（用于新增），类型为table_index

    void flush_all_blocks();			//把所有block的数据写回文件内
    void flush_one_block();				//把这个block的数据写回文件内
    void initialize_blocks();			//初始化
    void setDirty(unsigned int offset) {} // dummy delete definition
    static int get_LRU();				//最近使用次数 返回LRU
    static void using_block(int);		//标记第i块block为正在使用
    static void create_table(string);
    static void removeFile(string filename);
    //临时测试用
    void set_file_name(string filename_in);
    //临时测试用
    void mark_is_written();						//标记this->is_written为true，被修改过
    void mark_being_used();						//标记this->being_used为true，正在使用
    void end_being_used();						//标记this->being_used为false，使用完成
    void mark_block(string filename, unsigned int offset);	//标记此块的filename和offset

};

#endif //MINISQL_BUFFERMANAGER_H