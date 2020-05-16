#ifndef BTREE_HPP
#define BTREE_HPP

#include "node.hpp"
#include "pagemanager.hpp"
#include "constants.hpp"

using namespace constants;

template <class T>
class BPlusTree
{
private:
    typedef Node<int> Index;

    long root;
    int position;
    PageManager<Index> *pm;

public:
    BPlusTree(PageManager<Index>*);
    ~BPlusTree();

    long root_address();

    int root_pos();

    location find(T);

    void insert(T, long, int);

    int split(Page<Index>, long, int, location&);

    void print();
};

template <class T>
BPlusTree<T>::BPlusTree(PageManager<Index>* pm)
{
    this->pm = pm;
    root = -1;
    position = -1;
}

template <class T>
BPlusTree<T>::~BPlusTree()
{
}

template <class T>
long BPlusTree<T>::root_address()
{
    return root;
}

template <class T>
int BPlusTree<T>::root_pos()
{
    return position;
}

template <class T>
location BPlusTree<T>::find(T value)
{
    Page<Index> page = pm->load_page(root);
    Index index = page.get(position);

    location next = {root, position};

    while (!index.is_leaf())
    {
        next = index.find_in_node(value);
        page = pm->load_page(next.address);
        index = page.get(next.pos);
    }

   return index.find_value(value);
}

template <class T>
void BPlusTree<T>::insert(T value, long addr, int pos)
{
    Page<Index> page;
    Index index;

    if (root == -1)
    {
        index.insert(value, addr, pos);
        page.insert(index);
        pm->save_page(page, 0);

        root = 0;
        position = 0;

        return;
    }

    vector<location> path;

    page = pm->load_page(root);
    index = page.get(position);

    path.push_back({root, position});
    cout << "data to valor " << value << " "<< addr << " " << pos << endl;
    location next = {root, position};
    cout << "root data" << endl;
    for (int k = 0; k < index.size(); k++)
        cout << index.keys[k] << endl;
    while (!index.is_leaf())
    {
        for (int k = 0; k < index.child_count; k++)
            cout << "Hijos :" <<index.children[k].address << " " << index.children[k].pos << endl;
        next = index.find_in_node(value);
        path.push_back(next);
        page = pm->load_page(next.address);
        index = page.get(next.pos);

        //cout << "Hijos :" <<next.address << " " << next.pos << endl;
        cout << "leaf" <<endl;
    }
    cout << "before data" << endl;
    for (int k = 0; k < index.size(); k++)
        cout << index.keys[k] << endl;
    cout << endl;
    index.insert(value, addr, pos);
    page.insert(index, next.pos);
    pm->save_page(page, next.address);
    
    cout << "data to add " << value << endl;
    cout << "after data" << endl;
    for (int k = 0; k < index.size(); k++)
        cout << index.keys[k] << endl;
    cout << endl;
    location parent_loc;

    Page<Index> parent;
    Index parent_index;

    cout << "Direccion " << next.address << endl;
    cout << "Posicion "<<next.pos << endl << endl;
        
    for (int i = path.size() - 1; i >= 0; i--)
    {
        next = path[i];
        page = pm->load_page(next.address);
        index = page.get(next.pos);
        
        if (!index.is_overflow()) break;

        cout << "create new node " << i<<endl;
        location right;
        
        int mid = split(page, next.address, next.pos, right);

        if (next.address == root && next.pos == position)
        {
            cout << "case --- 0" << endl;
            
            parent_loc.address = pm->last_page();
            parent = pm->load_page(parent_loc.address);

            parent_index = Index();
            parent_index.insert_split(mid, next, right);

            cout << "pagina a crear o usar "<< parent_loc.address << endl;

            if (parent.is_overflow())
            {
                parent = Page<Index>();
                parent_loc.pos = parent.insert(parent_index);
                parent_loc.address = pm->save_bot(parent);
            }
            else {
                parent_loc.pos = parent.insert(parent_index);
                pm->save_page(parent, parent_loc.address);
            }
            root = parent_loc.address;
            position = parent_loc.pos;

            cout << "root n " << root << endl;
            cout << "pos n " << position << endl;
            cout << "sssssssssssssssssssssssssssssssssssssssssse cambio el root"<< endl;
            break;
        }

        if (i == 0) break;

        parent_loc = path[i - 1];
        parent = pm->load_page(parent_loc.address);

        parent_index = parent.get(parent_loc.pos);

        cout << "Parent "<<parent_loc.address <<" "<< parent_loc.pos <<endl;
        cout << "Parent "<<parent_index.size() <<endl;

        for (int k = 0; k < parent_index.size(); k++)
            cout << parent_index.keys[k] << endl;
            cout << endl;

        parent_index.insert_split(mid, next, right);
        parent.insert(parent_index, parent_loc.pos);
        pm->save_page(parent, parent_loc.address);
    }
}

template <class T>
int BPlusTree<T>::split(Page<Index> page, long addr, int pos, location& nright)
{
    Index left, right, index = page.get(pos);
    location loc;
    
    long mid_key;
    int mid = index.size() / 2;

    mid_key = index.keys[mid];

    for (int i = mid + !index.is_leaf(); i < index.size(); i++)
    {
        loc = index.children[i];
        right.insert(index.keys[i], loc.address, loc.pos);
    }

    right.children[right.child_count++] = index.children[BLOCKING_FACTOR];

    for (int i = 0; i < mid; i++)
    {
        loc = index.children[i];
        left.insert(index.keys[i], loc.address, loc.pos);
    }

    left.children[left.child_count++] = index.children[mid];

    Page<Index> last_page;
    nright.address = pm->last_page();
    last_page = pm->load_page(nright.address);

    if (last_page.is_overflow())
    {
        last_page = Page<Index>();
        nright.pos = last_page.insert(right);
        nright.address = pm->save_bot(last_page);
        cout << "OVEEEEEEEEEEEEEEEEEEEEEEEEEEEER"<< nright.address << endl;
        pm->save_page(last_page, nright.address);
    } else {
        nright.pos = last_page.insert(right);
        pm->save_page(last_page, nright.address);
    }

    if (index.is_leaf())
        left.children[left.child_count - 1] = nright;

    page = pm->load_page(addr);
    page.insert(left, pos);
    pm->save_page(page, addr);

    return mid_key;
}

#endif