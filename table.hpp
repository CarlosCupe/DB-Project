#ifndef TABLE_HPP
#define TABLE_HPP

#include <iostream>
#include <bits/stdc++.h> 
#include "btree.hpp"
#include "page.hpp"

using namespace std;

class Table
{
private:
    char table_name[25];
    long root_index;
    int pos_index;

public:
    Table();
    Table(string);
    ~Table();

    string name();

    string file_name();

    string index_name();

    void btree_index();

    void print();

    Record find(int);
};


Table::Table()
{
    root_index = -1;
    pos_index = -1;
}

Table::Table(string nname)
{
    strcpy(this->table_name, nname.c_str());
    root_index = -1;
}

Table::~Table()
{
}

string Table::name()
{
    return table_name;
}

string Table::file_name()
{
    string file = table_name;
    return file + ".bin";
}

string Table::index_name()
{
    string file = table_name;
    return file + "_index.bin";
}

void Table::btree_index()
{
    PageManager<Node<int>> pm(index_name());
    PageManager<Record> data(file_name());

    BPlusTree<int> btree(&pm);
    
    int data_size = data.last_page() / sizeof(Page<Record>) + 1;

    Page<Record> page;
    Page<Node<int>> a;

    Record record;

    Node<int> b;

    for (int i = 0; i < data_size; i++)
    {
        page = data.load_page(i * sizeof(Page<Record>));
        
        for (int j = 0; j < page.size(); j++)
        {
            cout << ">>>>>>> Case "<< j << endl;
            record = page.get(j);
            btree.insert(record.phone, i * sizeof(Page<Record>), j);
        }
    }

    root_index = btree.root_address();
    pos_index = btree.root_pos();
}

void Table::print()
{
    PageManager<Record> pm(file_name());

    Page<Record> page = pm.load_page(0);

    Record record = page.get(0);

    cout << record.phone << endl;

}

Record Table::find(int value)
{
    PageManager<Node<int>> pm(index_name());
    PageManager<Record> data(file_name());

    BPlusTree<int> btree(&pm);
    location loc = btree.find(value);

    if (loc.address == -1) return Record();

    Page<Record> page = data.load_page(loc.address);
    return page.get(loc.pos);
}

#endif