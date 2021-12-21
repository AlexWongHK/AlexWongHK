#include <iostream>
using namespace std;
#include "vector.h"
#include "list.h"

#ifndef _QUEUE_
#define _QUEUE_
#define rank int

template <typename T>
class queue: public list<T>{
public:
    queue<T>(): list<T>() {}
    int size() { return list<T>::size(); }
    bool empty() { return size() == 0; }
    void enqueue(T const & e) { list<T>::insertAsLast(e); }
    T dequeue() { return list<T>::remove(list<T>::first()); }
    T & front() { return list<T>::first()->data; }
    T & back() { return list<T>::last()->data; }
};

template <typename T>
class queue_v: public vector<T>{
    queue_v<T>(): vector<T>() {}
    int size() { return vector<T>::size(); }
    bool empty() { return size() == 0; }
    void enqueue(T const & e) { vector<T>::insert(e); }
    T dequeue() { return vector<T>::remove(vector<T>::size() - 1); }
    T & front() { return vector<T>::get(0); }
    T & back() { return vector<T>::get(vector<T>::size() - 1); }
};

#endif