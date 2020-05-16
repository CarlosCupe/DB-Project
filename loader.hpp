#ifndef LOADER_HPP
#define LOADER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cstring>
#include "record.hpp"
#include "page.hpp"
#include "table.hpp"

using namespace std;

class Loader
{
private:
    string file_name;
    char separator;

public:
    Loader(string, char);
    ~Loader();

    Table load_to_table(string);
};

Loader::Loader(string file_name, char separator)
{
    this->file_name = file_name;
    this->separator = separator;
}

Loader::~Loader()
{
}

Table Loader::load_to_table(string table_name)
{
    ifstream data_file;
    data_file.open(file_name);

    string bin_name = table_name + ".bin";
    
    ofstream bin_file;
    bin_file.open(bin_name, ios::out | ios::binary);

    vector<string> row;
    string line, word;

    Record record;
    Page<Record> *page = new Page<Record>();

    getline(data_file, line);

    while (getline(data_file, line))
    {
        stringstream ss(line); 
        
        row.clear();

        while (getline(ss, word, separator))
            row.push_back(word);
        
        strcpy(record.nombre, row[0].c_str());
        strcpy(record.apellido, row[1].c_str());
        strcpy(record.email, row[3].c_str());

        try
        {
            record.phone = std::stoi(row[2]);
        }
        catch (int e) 
        {
            cout << "An exception occurred. Exception Nr. " << e << '\n';
        }

        page->insert(record);
        if (page->is_overflow())
        {
            bin_file << *page;
            delete page;
            page = new Page<Record>();
        }
    }

    if (page->size() != 0)
    {
        bin_file << *page;
        delete page;
    }

    data_file.close();
    bin_file.close();

    Table table(table_name);

    return table;
}


#endif