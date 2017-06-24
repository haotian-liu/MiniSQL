#include <cstring>

#include "BufferManager.h"
#include "../CatalogManager/CatalogManager.h"

using namespace std;

void BufferManager::clear_block(int i)
{
    ptr_buffer[i].filename = "";
    memset(ptr_buffer[i].content, 0, FILE_BLOCK_SIZE);
    ptr_buffer[i].is_written = false;
    ptr_buffer[i].being_used = false;
    ptr_buffer[i].offset = 0;
    ptr_buffer[i].ID = i;
    ptr_buffer[i].LRU_count = 0;
    ptr_buffer[i].block_address = reinterpret_cast<char *>(static_cast<void *>(&ptr_buffer[i].content));
    //cout << "&ptr_buffer[" << i << "].content:" << &ptr_buffer[i].content << endl;
    //const char* tmp = static_cast<const char *>(ptr_buffer[i].content);
    //printf(" ptr_buffer[i].block_address : %s\n", ptr_buffer[i].block_address);
    //cout << "static_cast:" << ptr_buffer[i].block_address <<endl;
}
int BufferManager::get_block_for_insert(bool choose)
{
    int ind;
    if (choose)
    {
        ind = get_blank_block_ind();
        ptr_for_insert = &(ptr_buffer[ind]);
    }
    mark_is_written(ind);
    return ptr_for_insert->ID;
    //return (char*)(block_for_insert);
}

char* BufferManager::get_specified_block(string filename, int offset)
{
    unsigned int number, i, mark,count=0;
    for (int i = 0;i < MAX_BLOCKS; i++)
    {
        if (ptr_buffer[i].filename == filename && ptr_buffer[i].offset == offset)//如果此块在buffer中 直接返回
        {
            using_block(i);
            return (char*)&(ptr_buffer[i]);
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
    fp.seekg(ios::beg + offset*FILE_BLOCK_SIZE);
    fp.read(ptr_buffer[i].block_address, FILE_BLOCK_SIZE);
    fp.close();
    using_block(i);
    return (char*)&(ptr_buffer[i]);
}

void BufferManager::removeFile(string filename_in)	//删除整个文件 buffer和file一起
{
    const char * temp = filename_in.c_str();
    for (int i = 0; i<MAX_BLOCKS; i++)
    {
        if (ptr_buffer[i].filename == filename_in)
        {
            int total_size = CatalogManager::get_total_size_of_attr(filename_in);
            ptr_buffer[i].filename = nullptr;
            ptr_buffer[i].is_written = false;
            ptr_buffer[i].being_used = false;
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
    for (int i = 0; i<MAX_BLOCKS; i++)
        clear_block(i);
    ptr_for_insert = &(ptr_buffer[MAX_BLOCKS - 1]);
}


//首先查找表中是否有空位写入
//如果没有空位 用LRU找一块block清除后写入
unsigned int BufferManager::get_blank_block_ind()
{
    int i,res;
    char * p;
    for (i = 0; i < MAX_BLOCKS; i++)
    {
        if (ptr_buffer[i].filename!="")
            break;
    }
    if (i < MAX_BLOCKS)	//如果找到空块
    {
        res = i;
    }

    else if (i >= MAX_BLOCKS)
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
    for (i = 0; i < MAX_BLOCKS; i++)
    {
        if (ptr_buffer[i].filename != "")
            break;
    }
    if (i < MAX_BLOCKS)	//如果找到空块
    {
        res = i;
    }

    else if (i >= MAX_BLOCKS)
    {
        res = get_LRU();
        flush_one_block(res);
    }
    using_block(res);
    mark_being_used(res);
    return (char*)&(ptr_buffer[res]);
}
//提供给index和record manager来获取 修改 增加块

char * BufferManager::get_block(string filename, unsigned int offset, bool allocate)
{
    unsigned int number,i,mark;
    //int total_size = CatalogManager::get_total_size_of_attr(filename);
    //int ID = total_size / BLOCK_SIZE;
    for(int i=0;i<MAX_BLOCKS;i++)
    {
        if(ptr_buffer[i].filename==filename &&
           ptr_buffer[i].offset == offset)
        {
            using_block(i);
            return (char*)(&ptr_buffer[i]);
        }
    }
    //如果该块不在buffer中 则从文件中读取
    i = get_blank_block_ind();
    mark_block(i,filename, offset);

    fstream fp;
    fp.open(filename, ios::in | ios::out | ios::app | ios::binary);
    if (!fp.good())
        cerr << filename<<"file open err" << endl;
    fp.seekg(ios::beg + offset*FILE_BLOCK_SIZE);
    fp.read((char*)(ptr_buffer[i].content), FILE_BLOCK_SIZE);
    fp.close();
    using_block(i);
    return (char*)(&ptr_buffer[i]);
}

void BufferManager::mark_block(int ind,string filename_in, unsigned int offset)
{
    ptr_buffer[ind].filename = filename_in;
    ptr_buffer[ind].offset = offset;
}

void BufferManager::mark_is_written(int ind)
{
    ptr_buffer[ind].is_written = true;
}

void BufferManager::mark_being_used(int ind)
{
    ptr_buffer[ind].being_used = true;
}

void BufferManager::end_being_used(int ind)
{
    ptr_buffer[ind].being_used = false;
}

void BufferManager::using_block(int n)
{
    ptr_buffer[n].being_used = true;
    for(int i=0;i<MAX_BLOCKS;i++)
    {
        if(i!=n && !(ptr_buffer[i].being_used))		//不在被使用的块 count++
            ptr_buffer[i].LRU_count ++;
        else if(i==n)
            ptr_buffer[i].LRU_count = 0;
    }
}

int BufferManager::get_LRU()
{
    int max = -1,num=-1;
    for(int i=0;i<MAX_BLOCKS;i++)
    {
        if(ptr_buffer[i].LRU_count > max)
        {
            max = ptr_buffer[i].LRU_count;
            num = i;
        }
    }
    return num;
}

void BufferManager::flush_one_block(int ind)
{
    //cout << "flush one block" << endl;
    //cout << "ID:"<< ind << endl;
    //cout << "block_address" << ptr_buffer[ind].block_address << endl;
    //cout << "LRU:" << ptr_buffer[ind].LRU_count << endl;
    char* tmp = ptr_buffer[ind].block_address + BLOCK_HEADER_SIZE;
    fstream fp;
    fp.open(ptr_buffer[ind].filename, ios::in | ios::out | ios::app | ios::binary);
    //if (!fp.good())
    //	cerr << ptr_buffer[ind].filename << " open err" << endl;
    fp.seekg(ios::beg +(ptr_buffer[ind].offset)*FILE_BLOCK_SIZE);
    fp.write(tmp, FILE_BLOCK_SIZE);
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
    for(int i=0;i<MAX_BLOCKS;i++)
    {
        flush_one_block(i);
        if(ptr_buffer[i].block_address != "\0")
            ptr_buffer[i].block_address = nullptr;
        if(ptr_buffer[i].filename != "\0")
            ptr_buffer[i].filename = nullptr;
    }
}