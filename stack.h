#include <iostream>
using namespace std;
#include "list.h"
#include "vector.h"
#ifndef _STACK_
#define _STACK_
#define rank int

template <typename T>
class stack: public list<T>{
public:
    stack<T>(): list<T>() {}
    int size() { return list<T>::size(); }
    void push(T const & e) { list<T>::insertAsLast(e); }
    T pop() { return list<T>::remove(list<T>::last()); }
    T & top() { return list<T>::last()->data; }
    bool empty() { return (size() == 0); }
};

template <typename T>
class stack_v: public vector<T>{
public:
    stack_v<T>(): vector<T>() {}
    int size() { return vector<T>::size(); }
    void push(T const & e) { vector<T>::insert(e); }
    T pop() { return vector<T>::remove(size() - 1); }
    T & top() { return *this[size() - 1]; }
    bool empty() { return (size() == 0); }
};

#endif