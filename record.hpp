#ifndef RECORD_HPP
#define RECORD_HPP

#include <iostream>

using namespace std;

class Record
{
private:

public:
    char nombre[20];
    char apellido[20];
    int phone;
    char email[20];

    Record();
    ~Record();

    friend ostream & operator << (ostream&, Record&);

    friend istream & operator >> (istream&, Record&);
};

Record::Record()
{
}

Record::~Record()
{
}

ostream& operator << (ostream& stream, Record& record)
{
    stream.write((char *) &record, sizeof(Record));
	return stream;
}

istream& operator >> (istream& stream, Record& record)
{
	stream.read((char *) &record, sizeof(Record));
	return stream;
}

#endif