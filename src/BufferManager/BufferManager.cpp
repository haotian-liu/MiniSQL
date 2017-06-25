#include <cstring>

#include "BufferManager.h"
#include "../CatalogManager/CatalogManager.h"

using namespace std;

void BufferManager::clear_block(int i)
{
    blockBuffer[i].filename = "";
    memset(blockBuffer[i].content, 0, BlockSize);
    blockBuffer[i].is_written = false;
    blockBuffer[i].being_used = false;
    blockBuffer[i].offset = 0;
    blockBuffer[i].ID = i;
    blockBuffer[i].LRU_count = 0;
    blockBuffer[i].block_address = reinterpret_cast<char *>(static_cast<void *>(&blockBuffer[i].content));
    //cout << "&blockBuffer[" << i << "].content:" << &blockBuffer[i].content << endl;
    //const char* tmp = static_cast<const char *>(blockBuffer[i].content);
    //printf(" blockBuffer[i].block_address : %s\n", blockBuffer[i].block_address);
    //cout << "static_cast:" << blockBuffer[i].block_address <<endl;
}

char* BufferManager::get_specified_block(string filename, int offset)
{
    unsigned int number, i, mark,count=0;
    for (int i = 0;i < MaxBlocks; i++)
    {
        if (blockBuffer[i].filename == filename && blockBuffer[i].offset == offset)//如果此块在buffer中 直接返回
        {
            using_block(i);
            return (char*)&(blockBuffer[i]);
        }
    }
    //如果该块不在buffer中 则从文件中读取
    i = get_blank_block_ind();
    mark_block(i,filename, offset);

    fstream fp;
    fp.open(filename, ios::in | ios::out | ios::app | ios::binary);
    if (!fp.good())
    {
        cerr << filename << "file open err" << endl;
        exit(0);
    }
    fp.seekg(ios::beg + offset*BlockSize);
    fp.read(blockBuffer[i].block_address, BlockSize);
    fp.close();
    using_block(i);
    return (char*)&(blockBuffer[i]);
}

void BufferManager::removeFile(string filename_in)  //删除整个文件 buffer和file一起
{
    const char * temp = filename_in.c_str();
    for (int i = 0; i<MaxBlocks; i++)
    {
        if (blockBuffer[i].filename == filename_in)
        {
            int total_size = CatalogManager::get_total_size_of_attr(filename_in);
            blockBuffer[i].filename = nullptr;
            blockBuffer[i].is_written = false;
            blockBuffer[i].being_used = false;
        }
    }
    if (remove(temp))
    {
        printf("Could not delete the file &s /n", temp);
        exit(0);
    }
}


//新建立一个空文件
//没有自己加后缀，默认输入的in string已经是有后缀的文件了？
void BufferManager::create_table(string in)
{
    ofstream f1(in);
}


void BufferManager::initialize_blocks()
{
    for (int i = 0; i<MaxBlocks; i++)
        clear_block(i);
}


//首先查找表中是否有空位写入
//如果没有空位 用LRU找一块block清除后写入
unsigned int BufferManager::get_blank_block_ind()
{
    int i,res;
    char * p;
    for (i = 0; i < MaxBlocks; i++)
    {
        if (blockBuffer[i].filename!="")
            break;
    }
    if (i < MaxBlocks) //如果找到空块
    {
        res = i;
    }

    else if (i >= MaxBlocks)
    {
        res = get_LRU();
        flush_one_block(res);
    }
    using_block(res);
    mark_being_used(i);
    clear_block(res);
    return res;
}

//char * BufferManager::get_blank_block(unsigned int table_index)
char * BufferManager::get_blank_block_addr()
{
    int i, res;
    char * p;
    for (i = 0; i < MaxBlocks; i++)
    {
        if (blockBuffer[i].filename != "")
            break;
    }
    if (i < MaxBlocks) //如果找到空块
    {
        res = i;
    }

    else if (i >= MaxBlocks)
    {
        res = get_LRU();
        flush_one_block(res);
    }
    using_block(res);
    mark_being_used(res);
    return (char*)&(blockBuffer[res]);
}
//提供给index和record manager来获取 修改 增加块

char * BufferManager::get_block(string filename, unsigned int offset, bool allocate)
{
    unsigned int number,i,mark;
    //int total_size = CatalogManager::get_total_size_of_attr(filename);
    //int ID = total_size / BLOCK_SIZE;
    for(int i=0;i<MaxBlocks;i++)
    {
        if(blockBuffer[i].filename==filename &&
           blockBuffer[i].offset == offset)
        {
            using_block(i);
            return (char*)(&blockBuffer[i]);
        }
    }
    //如果该块不在buffer中 则从文件中读取
    i = get_blank_block_ind();
    mark_block(i,filename, offset);

    fstream fp;
    fp.open(filename, ios::in | ios::out | ios::app | ios::binary);
    if (!fp.good())
        cerr << filename<<"file open err" << endl;
    fp.seekg(ios::beg + offset*BlockSize);
    fp.read((char*)(blockBuffer[i].content), BlockSize);
    fp.close();
    using_block(i);
    return (char*)(&blockBuffer[i]);
}

void BufferManager::mark_block(int ind,string filename_in, unsigned int offset)
{
    blockBuffer[ind].filename = filename_in;
    blockBuffer[ind].offset = offset;
}

void BufferManager::mark_is_written(int ind)
{
    blockBuffer[ind].is_written = true;
}

void BufferManager::mark_being_used(int ind)
{
    blockBuffer[ind].being_used = true;
}

void BufferManager::end_being_used(int ind)
{
    blockBuffer[ind].being_used = false;
}

void BufferManager::using_block(int n)
{
    blockBuffer[n].being_used = true;
    for(int i=0;i<MaxBlocks;i++)
    {
        if(i!=n && !(blockBuffer[i].being_used))     //不在被使用的块 count++
            blockBuffer[i].LRU_count ++;
        else if(i==n)
            blockBuffer[i].LRU_count = 0;
    }
}

int BufferManager::get_LRU()
{
    int max = -1,num=-1;
    for(int i=0;i<MaxBlocks;i++)
    {
        if(blockBuffer[i].LRU_count > max)
        {
            max = blockBuffer[i].LRU_count;
            num = i;
        }
    }
    return num;
}

void BufferManager::flush_one_block(int ind)
{
    //cout << "flush one block" << endl;
    //cout << "ID:"<< ind << endl;
    //cout << "block_address" << blockBuffer[ind].block_address << endl;
    //cout << "LRU:" << blockBuffer[ind].LRU_count << endl;
    char* tmp = blockBuffer[ind].block_address;
    fstream fp;
    fp.open(blockBuffer[ind].filename, ios::in | ios::out | ios::app | ios::binary);
    //if (!fp.good())
    //  cerr << blockBuffer[ind].filename << " open err" << endl;
    fp.seekg(ios::beg +(blockBuffer[ind].offset)*BlockSize);
    fp.write(tmp, BlockSize);
    fp.close();
    //}
    /*else if (is_written && index_table == MINISQL_BLOCK_TABLE)
    {
        string filename_new = filename + ".tb";
        char* tmp = const_cast<char *>(block_address.c_str());
        fstream fp;
        fp.open(filename_new, ios::in | ios::out | ios::app | ios::binary);
        if (!fp.good())
            cerr << filename << " open err" << endl;
        fp.seekg(ios::beg + offset*BLOCK_SIZE);
        fp.write(tmp, BLOCK_SIZE);
        fp.close();
    }*/
}

void BufferManager::flush_all_blocks()
{
    for(int i=0;i<MaxBlocks;i++)
    {
        flush_one_block(i);
        if(blockBuffer[i].block_address != "\0")
            blockBuffer[i].block_address = nullptr;
        if(blockBuffer[i].filename != "\0")
            blockBuffer[i].filename = nullptr;
    }
}