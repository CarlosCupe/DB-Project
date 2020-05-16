#ifndef DBMANAGER_HPP
#define DBMANAGER_HPP

#include <iostream>
#include "metadata.hpp"
#include "loader.hpp"

class DBManager
{
private:
    Metadata metadata;
    
public:
    DBManager();
    ~DBManager();

    void load_raw_data(string, string, char);

    void create_index(string, string);
    
    Record find(string, int);

    void print();
};

DBManager::DBManager()
{
    metadata.load();
}

DBManager::~DBManager()
{
    metadata.save();
}

void DBManager::load_raw_data(string table_name, string file_name, char separator)
{
    Table *ptr = metadata.get(table_name);

    if (ptr != nullptr) return;

    Loader loader(file_name, separator);

    Table table = loader.load_to_table(table_name);

    metadata.add(table);
}

void DBManager::create_index(string table_name, string attr) 
{
    Table *ptr = metadata.get(table_name);

    if (ptr == nullptr) return;

    ptr->btree_index();
    metadata.save();
}

Record DBManager::find(string table_name, int value)
{
    Table *ptr = metadata.get(table_name);

    if (ptr == nullptr) return Record();

    return ptr->find(value);
}

#endif