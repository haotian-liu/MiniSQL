#ifndef _catalog_H_
#define _catalog_H_

#include <list>

#include "../../include/DataStructure.h"

using namespace MINISQL_BASE;
using namespace std;

/*每次新建表的时候调用creat_new_table(string table_name,int num_of_attrs) 输入表名和attr数目
/*然后依次插入attr，用insert_attrs_after_creat_tabl(.....)
/*建立完成之后可以后续查询
/*
/*
*/

typedef struct struct_attr *AttrPointer;
typedef struct struct_attr
{
    string attr_name;            //字符串类型 属性的名称
    int type;                    //类型
    int length;                    //属性长度
    int offset;                    //在table里面的偏移量
    bool if_primary_key;        //是否是主键
    bool if_unique;                //是否唯一
    AttrPointer Next;            //指向下一个属性
} Attr;

typedef struct struct_table *TablePointer;
typedef struct struct_table
{
    string table_name;            //字符串类型 表名
    int num_of_attrs;            //表中属性的数目
    int num_of_records;            //表中记录的数目
    AttrPointer Attrs;            //指向第一个属性的指针
    AttrPointer primary_key;    //指向主键的指针
    TablePointer Next;            //指向下一个表
} Table;


class CatalogManager
{
public:

    //void create_new_table(const string& table_name,			//字符串类型 表名
    //	int num_of_attrs,									//表中属性的数目
    //	AttrPointer primary_key = nullptr);					//指向主键的指针


    void create_new_table(const std::string &table_name,
                          int num_of_attrs,
                          std::pair<std::string, SqlValueType> primary_key
    );


    void insert_attrs_after_create_table(string attr_name,            //字符串类型 属性的名称
                                         int type,                    //类型
                                         int length,                    //属性长度
                                         int offset,                    //在table里面的偏移量
                                         bool if_primary_key,        //是否是主键
                                         bool if_unique);            //是否唯一)


    static AttrPointer head_attr;
    static AttrPointer tail_attr;
    static TablePointer head_table;
    static TablePointer tail_table;
    //成员变量是所有表的头尾指针
    //和所有属性的头尾指针

    CatalogManager()
    {
        //head_attr = tail_attr = NULL;
        //head_table = tail_table = NULL;
    };

    ~CatalogManager() = default;

    //void write_into_file();
    static int get_total_size_of_attr(const string &table_name_in);

    static TablePointer find_table_by_name(const string &table_name);                        //输入表名 返回表指针
    static AttrPointer find_attr_by_name(const string &table_name_in, const string &attr_name_in);    //输入表名和属性名 返回属性指针

    bool if_table_exists(const string &table);                                        //判断该表是否存在
    bool if_attr_exists(const string &table_name, const string &attr_name);                //判断该属性在该表中是否存在
    bool if_attr_primary_key(const string &table_name, const string &attr_name);            //判断是否主键
    bool if_attr_unique(const string &table_name, const string &attr_name);                //判断是否唯一
    AttrPointer find_primary_key(const string &table_name_in);                        //输入表名，得到主键指针
    AttrPointer get_attr_by_num(const string &table_name, int attr_num);            //输入表名，得到第n个属性(从1开始编号
    int get_attr_type_by_name(const string &table_name, const string &attr_name);            //输入表名和属性名 返回属性类型
    int get_attr_type_by_num(const string &table_name, int attr_num);                //输入表名和序号 返回属性类型
    int get_attr_length_by_name(const string &table_name, const string &attr_name);        //输入表名和属性名 返回属性长度
    int get_attr_offset_by_name(const string &table_name, const string &attr_name);        //输入表名和属性名 返回属性偏移量
    int get_num_of_attr(const string &table_name);                                    //输入表名 得到属性数目
    int get_record_num(const string &table_name);                                    //输入表名 得到记录数目

private:

};

#endif