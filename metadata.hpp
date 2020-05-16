#ifndef METADATA_HPP
#define METADATA_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include "constants.hpp"
#include "table.hpp"

using namespace constants;
using namespace std;

class Metadata
{
private:
    vector<Table> tables;

public:
    Metadata();
    ~Metadata();

    void save();

    void load();

    void add(Table);

    Table* get(string);

    friend ostream& operator << (ostream&, Metadata&);

    friend istream& operator >> (istream&, Metadata&);
};

Metadata::Metadata()
{
}

Metadata::~Metadata()
{
}

void Metadata::save()
{
    ofstream file;
    file.open(METADATA_NAME, ios::binary | ios::trunc);

    file << *this;

    file.close();
}

void Metadata::load()
{
    ifstream file;
    file.open(METADATA_NAME, ios::binary);

    file >> *this;

    file.close();
}

void Metadata::add(Table table)
{
    tables.push_back(table);
    save();
}

Table* Metadata::get(string table_name)
{
    for (int i = 0; i < tables.size(); i++)
    {
        if (table_name.compare(tables[i].name()) == 0)
            return &tables[i];
    }
    return nullptr;
}

ostream& operator << (ostream& stream, Metadata& data)
{
    int vector_size = data.tables.size();

    stream.write((char*) &vector_size, sizeof(int));

    for (int i = 0; i < vector_size; i++)
        stream.write((char*) &data.tables[i], sizeof(Table));
    
	return stream;
}

istream& operator >> (istream& stream, Metadata& data)
{
	int vector_size;
    
    stream.read((char*) &vector_size, sizeof(int));

    data.tables.resize(vector_size);

    for (int i = 0; i < vector_size; i++)
        stream.read((char*) &data.tables[i], sizeof(Table));

	return stream;
}


#endif