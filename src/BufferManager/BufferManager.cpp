#include <cstring>

#include "BufferManager.h"
#include "../CatalogManager/CatalogManager.h"

using namespace std;

BufferManager *ptr_buffer = NULL;


BufferManager* BufferManager::ptr_buffer=nullptr;	//遍历用的指针
BufferManager* BufferManager::block_now = nullptr;
BufferManager* BufferManager::block_for_insert = nullptr;

char* BufferManager::get_specified_block(string filename, int offset)
{
    unsigned int number, i, mark,count=0;
    for (int i = 0;i < MAX_BLOCKS; i++)
    {
        if (filename == ptr_buffer[i].filename && ptr_buffer[i].offset == offset)
        {
            using_block(i);
            return (char*)&(ptr_buffer[i]);
        }
    }
    //如果该块不在buffer中 则从文件中读取
    i = get_blank_block_ind();
    ptr_buffer[i].mark_block(filename, offset);

    fstream fp;
    fp.open(filename, ios::in | ios::out | ios::app | ios::binary);
    if (!fp.good())
        cerr << filename << "file open err" << endl;
    fp.seekg(ios::beg + offset*BLOCK_SIZE);
    fp.read(const_cast<char *>(ptr_buffer[i].block_address.c_str()), BLOCK_SIZE);
    fp.close();
    //block_now = &(ptr_buffer[i]);
    using_block(i);
    return (char*)&(ptr_buffer[i]);
}

void BufferManager::set_file_name(string filename_in)
{
    this->filename = filename_in;
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

//返回存放filename的第一个block
//char * BufferManager::getFileBlock(string filename)
//{
//	unsigned int number, i, mark;
//	//int total_size = CatalogManager::get_total_size_of_attr(filename);
//	//int ID = total_size / BLOCK_SIZE;
//	for (int i = 0; i<MAX_BLOCKS; i++)
//	{
//		if (filename == ptr_buffer[i].filename)
//		{			
//			//cout << "ID = " << i << endl;
//			using_block(i);
//			return (char*)&(ptr_buffer[i]);
//		}
//	}
//	//如果该块不在buffer中 则从文件中读取
//	i = get_blank_block_ind(index_table);
//	ptr_buffer[i].mark_block(filename, offset);
//
//	fstream fp;
//	fp.open(filename, ios::in | ios::out | ios::app | ios::binary);
//	if (!fp.good())
//		cerr << filename << "file open err" << endl;
//	fp.seekg(ios::beg + offset*BLOCK_SIZE);
//	fp.read(const_cast<char *>(ptr_buffer[i].block_address.c_str()), BLOCK_SIZE);
//	fp.close();
//	block_now = &(ptr_buffer[i]);
//	using_block(i);
//	return (char*)(block_now);
//}

//返回继上次返回后面的第一个名为filename的block 
//如果不存在则返回nullptr
//char* BufferManager::get_next_block(string filename)
//{
//	BufferManager* tmp = block_now;
//	unsigned int ID_tmp = tmp->ID;
//	int i = ID_tmp + 1;
//	while (ptr_buffer[i].filename != filename && i <= MAX_BLOCKS)
//		i++;
//	if (i == MAX_BLOCKS)
//		return nullptr;
//	block_now = &(ptr_buffer[i]);
//	block_now->mark_is_written();
//	return (char*)block_now;
//}

//在需要插入的时候返回当前最后一个使用过的block
//可能存在已满的问题所以需要继续用get_next_block_for_insert
//char* BufferManager::get_block_for_insert()
//{
//	for (int i = 0; i<MAX_BLOCKS; i++)
//	{
//		if (!ptr_buffer[i].index_table && ptr_buffer[i].is_written==false)
//		{
//			using_block(i);
//			block_for_insert = &ptr_buffer[i];
//			return (char*)(block_for_insert);
//		}
//	}
//}

char* BufferManager::get_next_block_for_insert()
{
    BufferManager* tmp = block_for_insert;
    unsigned int ID_tmp = tmp->ID;
    if (ID_tmp < MAX_BLOCKS)
    {
        block_for_insert = &(ptr_buffer[ID_tmp + 1]);
        block_for_insert->mark_is_written();
        return (char*)block_for_insert;
    }
    else return nullptr;

}



//新建立一个空文件
//没有自己加后缀，默认输入的in string已经是有后缀的文件了？
void BufferManager::create_table(string in)
{
    ofstream f1(in);

}


void BufferManager::initialize_blocks()
{

    ptr_buffer = new BufferManager[MAX_BLOCKS];		//申请上线的内存量并依次初始化

    for (int i = 0; i<MAX_BLOCKS; i++)
    {
        ptr_buffer[i].block_address = "\0";
        ptr_buffer[i].index_table = 0;
        ptr_buffer[i].filename = "\0";
        memset(ptr_buffer[i].content, 0, BLOCK_SIZE);
        ptr_buffer[i].is_written = false;
        ptr_buffer[i].being_used = false;
        ptr_buffer[i].offset = 0;
        ptr_buffer[i].ID = i;
        ptr_buffer[i].LRU_count = 0;
    }

    BufferManager::ptr_buffer = ptr_buffer;	//遍历用的指针
    BufferManager::block_now = ptr_buffer;
    BufferManager::block_for_insert = ptr_buffer;
}


//首先查找表中是否有空位写入
//如果没有空位 用LRU找一块block清除后写入
unsigned int BufferManager::get_blank_block_ind()
{
    int i,res;
    char * p;
    for (i = 0; i < MAX_BLOCKS; i++)
    {
        if (!ptr_buffer[i].index_table && !ptr_buffer[i].being_used)
            break;
    }
    if (i < MAX_BLOCKS)	//如果找到空块
    {
        char* tmp = const_cast<char *>(block_address.c_str());
        res = i;
        ptr_buffer[res].block_address = new char[BLOCK_SIZE];
        for (p = tmp; p < ptr_buffer[res].block_address.c_str() + BLOCK_SIZE; p++)
        {
            *p = '\0';//初始化为空
        }
    }
    else if (i >= MAX_BLOCKS)
    {
        res = get_LRU();
        ptr_buffer[res].flush_one_block();
    }
    using_block(res);
    return res;
}

//char * BufferManager::get_blank_block(unsigned int table_index)
char * BufferManager::get_blank_block_addr(unsigned int table_index)
{
    int i, res;
    char * p;
    for (i = 0; i < MAX_BLOCKS; i++)
    {
        if (!ptr_buffer[i].index_table && !ptr_buffer[i].being_used)
            break;
    }
    if (i < MAX_BLOCKS)	//如果找到空块
    {
        char* tmp = const_cast<char *>(block_address.c_str());
        res = i;
        ptr_buffer[res].block_address = new char[BLOCK_SIZE];
        for (p = tmp; p < ptr_buffer[res].block_address.c_str() + BLOCK_SIZE; p++)
        {
            *p = '\0';//初始化为空
        }
    }
    else if (i >= MAX_BLOCKS)
    {
        res = get_LRU();
        ptr_buffer[res].flush_one_block();
    }
    using_block(res);
    ptr_buffer[res].index_table = table_index;
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
        if(filename==ptr_buffer[i].filename &&
           ptr_buffer[i].offset == offset &&
           ptr_buffer[i].index_table == index_table)
        {
            using_block(i);
            return (char*)(&ptr_buffer[i]);
        }
    }
    //如果该块不在buffer中 则从文件中读取
    i = get_blank_block_ind();
    ptr_buffer[i].mark_block(filename, offset);

    fstream fp;
    fp.open(filename, ios::in | ios::out | ios::app | ios::binary);
    if (!fp.good())
        cerr << filename<<"file open err" << endl;
    fp.seekg(ios::beg + offset*BLOCK_SIZE);
    fp.read(const_cast<char *>(ptr_buffer[i].block_address.c_str()), BLOCK_SIZE);
    fp.close();
    using_block(i);
    return (char*)(&ptr_buffer[i]);
}

void BufferManager::mark_block(string filename_in, unsigned int offset)
{
    this->filename = filename_in;
    this->offset = offset;
}

void BufferManager::mark_is_written()
{
    this->is_written = true;
}

void BufferManager::mark_being_used()
{
    this->being_used = true;
}

void BufferManager::end_being_used()
{
    this->being_used = false;
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

void BufferManager::flush_one_block()
{
    if (is_written && index_table==MINISQL_BLOCK_INDEX)
    {
        string filename_new = filename + ".ind";
        char* tmp = const_cast<char *>(block_address.c_str());
        fstream fp;
        fp.open(filename_new, ios::in | ios::out | ios::app | ios::binary);
        if (!fp.good())
            cerr << filename<< " open err" << endl;
        fp.seekg(ios::beg + offset*BLOCK_SIZE);
        fp.write(tmp, BLOCK_SIZE);
        fp.close();
    }
    else if (is_written && index_table == MINISQL_BLOCK_TABLE)
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
    }
}

void BufferManager::flush_all_blocks()
{
    for(int i=0;i<MAX_BLOCKS;i++)
    {
        ptr_buffer[i].flush_one_block();
        if(ptr_buffer[i].block_address != "\0")
            ptr_buffer[i].block_address = nullptr;
        if(ptr_buffer[i].filename != "\0")
            ptr_buffer[i].filename = nullptr;
    }
}