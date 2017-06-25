#include <string>
#include <list>

#include "CatalogManager.h"

using namespace std;

AttrPointer CatalogManager::head_attr = nullptr;
AttrPointer CatalogManager::tail_attr = nullptr;
TablePointer CatalogManager::head_table = nullptr;
TablePointer CatalogManager::tail_table = nullptr;


void CatalogManager::insert_attrs_after_create_table(string attr_name, int type,
                                                     int length, int offset, bool if_primary_key, bool if_unique)
{
    Attr tmp;
    tmp.attr_name = attr_name;
    tmp.type = type;
    tmp.length = length;
    tmp.offset = offset;
    tmp.if_primary_key = if_primary_key;
    tmp.if_unique = if_unique;

    if (!tail_table->Attrs)//该张表的第一个属性插入
    {
        tail_table->Attrs = &tmp;
        head_attr = tail_attr = &tmp;
        head_attr->Next = tail_attr->Next = nullptr;
    } else//非该表的第一个属性插入 直接放到属性链表最后
    {
        tail_attr->Next = &tmp;
        tail_attr = tail_attr->Next;
    }
    tail_table->num_of_attrs++;
}

//void CatalogManager::create_new_table(const string& table_name,			//字符串类型 表名
//									int num_of_attrs,			//表中属性的数目
//									AttrPointer primary_key)
//{
//	_Table tmp;
//	tmp.table_name = table_name;
//	tmp.num_of_attrs = num_of_attrs;
//	tmp.num_of_records = 0;
//	tmp.Attrs = nullptr;
//	tmp.primary_key = primary_key;
//	if (!head_table)	//如果是第一张表
//	{
//		tail_table = head_table = &tmp;
//		head_table->Next = nullptr;
//		head_table->Attrs = nullptr;
//	}
//	else//不是第一张表 后续插入
//	{
//		tail_table->Next = &tmp;
//		tail_table = &tmp;
//	}
//	//tail_table->num_of_attrs++;
//}

//void CatalogManager::create_new_table(const std::string &table_name,
//	int num_of_attrs,
//	std::pair<std::string, SqlValueType> primary_key
//)
//{
//	Table tb;
//	tb.Name = table_name;
//
//	tables.push_back(tb);
//
//	//flush
//	for (auto &it : tables)
//	{
//		it.Name
//	}
//}



//void CatalogManager::write_into_file()
//{
//	string filename = "catalog";
//
//	TablePointer table_tmp = head_table;
//	AttrPointer attr_tmp = head_attr;
//
//	ifstream fin(filename);
//	string s;
//	while (fin >> s)
//	{
//		cout << s << endl;
//	}
//
//	while (head_table != nullptr)
//	{
//		//写table相关
//		fin >> table_tmp->table_name;
//		attr_tmp = head_table->Attrs;
//		while (attr_tmp != nullptr)
//		{
//			fin >> s;
//		}
//	}
//
//}

TablePointer CatalogManager::find_table_by_name(const string &table_name_in)
{
    TablePointer tmp = head_table;
    while (tmp != NULL)
    {
        if (tmp->table_name == table_name_in)
            return tmp;
        else
            tmp = tmp->Next;
    }
    return NULL;
}

AttrPointer CatalogManager::find_attr_by_name(const string &table_name_in, const string &attr_name_in)
{
    TablePointer ptr_table = CatalogManager::find_table_by_name(table_name_in);
    AttrPointer tmp;
    if (ptr_table != NULL)
    {
        tmp = ptr_table->Attrs;
        while (ptr_table->Attrs != NULL)
        {
            if (tmp->attr_name == attr_name_in)
                return tmp;
            else
                tmp = tmp->Next;
        }
    }
    return NULL;
}

bool CatalogManager::if_table_exists(const string &table)
{
    return (find_table_by_name(table) != NULL);
}

bool CatalogManager::if_attr_exists(const string &table_name_in, const string &attr_name_in)
{
    return (find_attr_by_name(table_name_in, attr_name_in) != NULL);
}

bool CatalogManager::if_attr_primary_key(const string &table_name, const string &attr_name)
{
    AttrPointer tmp_attr = find_attr_by_name(table_name, attr_name);
    return tmp_attr->if_primary_key;
}

bool CatalogManager::if_attr_unique(const string &table_name, const string &attr_name)
{
    AttrPointer tmp_attr = find_attr_by_name(table_name, attr_name);
    return tmp_attr->if_unique;
}

AttrPointer CatalogManager::find_primary_key(const string &table_name_in)
{
    TablePointer tmp_table = find_table_by_name(table_name_in);
    if (tmp_table != NULL)
        return tmp_table->primary_key;
    else
        return NULL;
}

int CatalogManager::get_attr_type_by_name(const string &table_name_in, const string &attr_name_in)
{
    AttrPointer tmp_attr = find_attr_by_name(table_name_in, attr_name_in);
    return tmp_attr->type;
}

AttrPointer CatalogManager::get_attr_by_num(const string &table_name_in, int attr_num)
{
    TablePointer tmp_table = find_table_by_name(table_name_in);
    if (tmp_table != NULL)
    {
        if (attr_num > 0)
        {
            AttrPointer tmp_attr = tmp_table->Attrs;
            while (--attr_num)
            {
                if (tmp_attr != NULL)
                    tmp_attr = tmp_attr->Next;
            }
            return tmp_attr;
        }
    }
    return NULL;
}

int CatalogManager::get_attr_type_by_num(const string &table_name_in, int attr_num)
{
    AttrPointer tmp_attr = get_attr_by_num(table_name_in, attr_num);
    return tmp_attr->type;
}

int CatalogManager::get_num_of_attr(const string &table_name_in)
{
    TablePointer tmp_table = find_table_by_name(table_name_in);
    int count = 0;
    if (tmp_table != NULL)
    {
        AttrPointer tmp_attr = tmp_table->Attrs;
        while (tmp_attr != NULL)
        {
            tmp_attr = tmp_attr->Next;
            count++;
        }
    }
    return count;
}

int CatalogManager::get_total_size_of_attr(const string &table_name_in)    //除了attr之外还有table_name等属性
{
    TablePointer tmp_table = find_table_by_name(table_name_in);
    int size = 0;
    if (tmp_table != NULL)
    {
        AttrPointer tmp_attr = tmp_table->Attrs;
        while (tmp_attr != NULL)
        {
            size += tmp_attr->length;
            tmp_attr = tmp_attr->Next;
        }
    }
    return size;
}

int CatalogManager::get_attr_length_by_name(const string &table_name_in, const string &attr_name_in)
{
    /*TablePointer tmp_table = find_table_by_name(table_name_in);*/
    AttrPointer tmp_attr = find_attr_by_name(table_name_in, attr_name_in);
    return tmp_attr->length;
}

int CatalogManager::get_attr_offset_by_name(const string &table_name_in, const string &attr_name_in)
{
    /*TablePointer tmp_table = find_table_by_name(table_name_in);*/
    AttrPointer tmp_attr = find_attr_by_name(table_name_in, attr_name_in);
    return tmp_attr->offset;
}

//int CatalogManager::get_sum_of_attr_length(const string& table_name_in)
//{
//	int sum=0;
//	TablePointer tmp_table = find_table_by_name(table_name_in);
//	AttrPointer tmp_attr = tmp_table->Attrs;
//	while (tmp_attr != NULL)
//	{
//		sum += tmp_attr->length;
//		tmp_attr = tmp_attr->Next;
//	}
//	return sum;
//}

int CatalogManager::get_record_num(const string &table_name_in)
{
    TablePointer table_tmp = find_table_by_name(table_name_in);
    return table_tmp->num_of_records;
}