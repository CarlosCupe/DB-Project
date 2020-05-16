#ifndef PAGE_HPP
#define PAGE_HPP

#include <iostream>

#include "record.hpp"
#include "constants.hpp"

using namespace constants;

template <class T>
class Page;

template <class T>
istream& operator >> (istream&, Page<T>&);

template <class T>
ostream& operator << (ostream&, Page<T>&);

template <class T>
class Page
{
private:
    T records[BLOCKING_FACTOR];
    int count;

public:
    Page();
    ~Page();

    int insert(T);

    void insert(T, int);

    int size();

    bool is_overflow();

    T get(int);

    friend ostream& operator << <> (ostream&, Page&);

    friend istream& operator >> <> (istream&, Page&);
};

template <class T>
Page<T>::Page()
{
    count = 0;
}

template <class T>
Page<T>::~Page()
{
}

template <class T>
int Page<T>::insert(T record)
{
    records[count++] = record;

    return count - 1;
}

template <class T>
void Page<T>::insert(T record, int pos)
{
    records[pos] = record;
}

template <class T>
int Page<T>::size()
{
    return count;
}

template <class T>
bool Page<T>::is_overflow()
{
    return count == BLOCKING_FACTOR;
}

template <class T>
T Page<T>::get(int pos)
{
    return records[pos];
}

template <class T>
ostream& operator << (ostream& stream, Page<T>& page)
{
    stream.write((char*) &page.count , sizeof(int));

    for (int i = 0; i < BLOCKING_FACTOR; i++)
        stream.write((char*) &page.records[i], sizeof(T));
    stream.flush();
	return stream;
}

template <class T>
istream& operator >> (istream& stream, Page<T>& page)
{
	stream.read((char*) &page.count, sizeof(int));
    for (int i = 0; i < BLOCKING_FACTOR; i++)
        stream.read((char*) &page.records[i], sizeof(T));
	return stream;
}

#endif