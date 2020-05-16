#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include "constants.hpp"

using namespace constants;
using namespace std;

template <class T>
class Node
{
private:
    int leaf;

public:
    int keys_count;
    int child_count;

    T keys[BTREE_ORDER];
    location children[BTREE_ORDER + 1];
    
    Node();
    ~Node();

    void insert(T, long, int);

    location find_in_node(T&);

    location find_value(T);

    void insert_split(int, location, location);

    int size();

    bool is_leaf();

    bool is_overflow();
};

template <class T>
Node<T>::Node()
{
    leaf = -1;
    keys_count = 0;
    child_count = 0;
}

template <class T>
Node<T>::~Node()
{
}

template <class T>
void Node<T>::insert(T value, long addr, int pos)
{
    int count = keys_count - 1;

    while (value < keys[count] && count != -1)
    {
        keys[count + 1] = keys[count];
        children[count + 1] = children[count];
        count--;
    }
    
    keys[count + 1] = value;

    location loc = {addr, pos};
    children[count + 1] = loc;
    keys_count++;
    child_count++;
}

template <class T>
int Node<T>::size()
{
    return keys_count;
}

template <class T>
location Node<T>::find_in_node(T& value)
{
    int count = keys_count - 1;

    while (value < keys[count] && count != -1)
        count--;
    
    return children[count + 1];
}

template <class T>
location Node<T>::find_value(T value)
{

    for (int i = 0; i < keys_count; i++)
    {
        if (keys[i] == value) return children[i];
    }
    
    return {-1, -1};
}

template <class T>
void Node<T>::insert_split(int mid, location left, location right)
{
    int count = keys_count - 1;

    if (count == -1)
        child_count++;

    while (mid < keys[count] && count != -1)
    {
        keys[count + 1] = keys[count];
        children[count + 2] = children[count + 1];
        count--;
    }

    keys[count + 1] = mid;

    children[count + 1] = left;
    children[count + 2] = right;

    keys_count++;
    child_count++;

    leaf = 0;
}

template <class T>
bool Node<T>::is_leaf()
{
    return leaf == -1;
}

template <class T>
bool Node<T>::is_overflow()
{
    return keys_count >= BTREE_ORDER;
}

#endif