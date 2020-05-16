#ifndef PAGEMANAGER_HPP
#define PAGEMANAGER_HPP

#include <iostream>
#include <fstream>
#include "node.hpp"
#include "page.hpp"

using namespace std;

template <class T>
class PageManager
{
private:
    string file_name;
    Page<T>* page;
    long address;

public:
    PageManager(string);
    ~PageManager();

    Page<T> load_page(long);

    void save_page(Page<T>&, long);

    long save_bot(Page<T>);

    long last_page();

    long new_page();
};

template <class T>
PageManager<T>::PageManager(string file_name)
{
    this->file_name = file_name;
    this->page = nullptr;
    this->address = -1;
}

template <class T>
PageManager<T>::~PageManager()
{
}

template <class T>
Page<T> PageManager<T>::load_page(long addr)
{
    if (address == addr) return *page;

    address = addr;

    delete page;
    page = new Page<T>();

    ifstream file;

    file.open(file_name, ios::binary);
    file.seekg(address);

    file >> *page;
    file.close();

    return *page;
}

template <class T>
void PageManager<T>::save_page(Page<T>& npage, long addr)
{
    if (page == nullptr) page = new Page<T>();

    *page = npage;
    address = addr;

    ofstream file;

    file.open(file_name, ios::binary);
    file.seekp(addr);
    file << npage;
    file.flush();
    file.close();
}

template <class T>
long PageManager<T>::save_bot(Page<T> npage)
{
    long addr = new_page();

    ofstream file;
    file.open(file_name, ios::binary | ios::app);
    
    file << npage;

    file.flush();
    file.close();

    cout << "Tamaño " << addr - new_page() << endl;
    return addr;
}

template <class T>
long PageManager<T>::last_page()
{
    long last_pos;
    last_pos = new_page() - 916;
    if (last_pos < 0) return 0;
    return last_pos;
}

template <class T>
long PageManager<T>::new_page()
{
    long last_pos;

    ifstream file;
    file.open(file_name, ios::in | ios::binary | ios::ate);
    file.seekg(0, ios::end);
    last_pos = file.tellg();
    file.close();

    return last_pos;
}

#endif