#include <iostream>
using namespace std;
#ifndef _LIST_
#define _LIST_
#define rank int

template <typename T>
class node{
public:
    T data;
    node<T>* pred;
    node<T>* succ;
    node<T>() : pred(NULL), succ(NULL) {}
    node<T>(const node<T> & p) : data(p.data), pred(p.pred), succ(p.succ) {}
    node<T>(T e, node<T>* p = NULL, node<T>* s = NULL) : data(e), pred(p), succ(s) {}
    node<T>* insertAsPred(T const& e);
    node<T>* insertAsSucc(T const& e);
    bool operator>(node<T> const & n) { return data > n.data; }
    bool operator<(node<T> const & n) { return data > n.data; }
    bool operator==(node<T> const & n) { return data == n.data; }
    bool operator!=(node<T> const & n) { return data != n.data; }
};

template <typename T>
node<T>* node<T>::insertAsPred(T const& e){
    node<T>* x = new node<T>(e, pred, this);
    pred->succ = x;
    pred = x;
    return x;
}

template <typename T>
node<T>* node<T>::insertAsSucc(T const& e){
    node<T>* x = new node<T>(e, this, succ);
    succ->pred = x;
    succ = x;
    return x;
}

template <typename T>
class list{
private:
    int _size;
    node<T>* header;
    node<T>* tailer;
    void init();
    void clear();
    void copyNodes(node<T>* p, int n);
    node<T>* selectMax(node<T>* p, int n);
    node<T>* merge(node<T>* p, int n, list<T> & L, node<T>* q, int m);
public:
    list<T>(){ init();}
    list<T>(list<T> const & L);
    ~list<T>() {clear(); delete header; delete tailer;}
    T& operator[](rank r);
    node<T>* insertA(node<T>* p, T const& e);
    node<T>* insertB(node<T>* p, T const& e);
    node<T>* insertAsFirst(T const& e);
    node<T>* insertAsLast(T const& e);
    T remove(node<T>* p);
    int size() const {return _size;}
    node<T>* first() const {return header->succ;}
    node<T>* last() const {return tailer->pred;}
    node<T>* find(T const & e, int n, node<T>* p);
    node<T>* find(T const & e);
    int deduplicate();
    template<typename VST> void traverse(VST& visit);
    void uniquify();
    node<T>* search(T const & e, int n, node<T>* p);
    node<T>* search(T const & e);
    void selectionSort(int n, node<T>* p);
    void insertionSort(int n, node<T>* p);
    void mergeSort(int n, node<T>* & p);
    void sort(char s);
};

template <typename T>
list<T>::list(list<T> const & L){
    copyNodes(L.first(), L._size);
}

template <typename T>
void list<T>::init(){
    header = new node<T>;
    tailer = new node<T>;
    header->succ = tailer;
    header->pred = NULL;
    tailer->pred = header;
    tailer->succ = NULL;
    _size = 0;
}

template<typename T>
void list<T>::clear(){
    while(0 < _size){
        remove(header->succ);
    }
}

template<typename T>
void list<T>::copyNodes(node<T>* p, int n){
    init();
    while(n--){
        insertAsLast(p->data);
        p = p->succ;
    }
}

template <typename T>
node<T>* list<T>::selectMax(node<T>* p, int n){
    node<T>* cur = p;
    node<T>* max = cur;
    for(cur = p; 1 < n; n--){
        if(max->data <= cur->data){
            max = cur;
        }
        cur = cur->succ;
    }
    return max;
}

template <typename T>
T& list<T>::operator[](rank r){
    try{
        if(r < 0 || _size <= r) throw(r);
        node<T>* p = header->succ;
        while(r--){
            p = p->succ;
        }
        return p->data;
    }
    catch(rank e){
        cout << "OUT_OF_RANGE ";
        cout << "Rank " << e ;
        cout << " Size " << _size << endl;
        exit(-1);
    }
}

template <typename T>
node<T>* list<T>::insertA(node<T>* p, T const& e){
    _size++;
    return p->insertAsSucc(e);
}

template <typename T>
node<T>* list<T>::insertB(node<T>* p, T const& e){
    _size++;
    return p->insertAsPred(e);
}

template <typename T>
node<T>* list<T>::insertAsFirst(T const& e){
    _size++;
    return header->insertAsSucc(e);
}

template <typename T>
node<T>* list<T>::insertAsLast(T const& e){
    _size++;
    return tailer->insertAsPred(e);
}

template <typename T>
T list<T>::remove(node<T>* p){
    T e = p->data;
    p->pred->succ = p->succ;
    p->succ->pred = p->pred;
    delete p;
    _size--;
    return e;
}

template <typename T>
node<T>* list<T>::find(T const & e, int n, node<T>* p){
    while(0 < n--){
        if(e == (p->pred)->data) return p;
    }
    return NULL;
}

template <typename T>
node<T>* list<T>::find(T const & e){
    return find(e, _size, tailer);
}

template <typename T>
int list<T>::deduplicate(){
    int _oldsize = _size;
    node<T>* p = first();
    for(int r = 0; p != tailer; p = p->succ){
        if(node<T>* q = find(p->data, r, p)){
            remove(q);
        }
        else{
            r++;
        }
    }
    return _oldsize - _size;
}

template <typename T> template <typename VST>
void list<T>::traverse(VST& visit){
    node<T>* p = header;
    while((p = p->succ) != tailer) visit(p->data);
}

template <typename T>
void list<T>::uniquify(){
    if(_size < 2) return;
    node<T>* p = first();
    while(tailer != p){
        if(p->data == p->succ->data){
            remove(p->succ);
        }
        else{
            p = p->succ;
        }
    }
}

template <typename T>
node<T>* list<T>::search(T const & e, int n, node<T>* p){
    do{
        p = p->pred;
        n--;
    } while((-1 < n) && e < (p->data));
    return p;
}

template <typename T>
node<T>* list<T>::search(T const & e){
    return search(e, _size, tailer);
}

template<typename T>
void list<T>::selectionSort(int n, node<T>* p){
    node<T>* head = p->pred;
    node<T>* tail = p;
    for(int i = 0; i < n; i++) tail = tail->succ;
    while(1 < n){
        insertB(tail, remove(selectMax(head->succ, n)));
        tail = tail->pred;
        n--;
    }
}

template <typename T>
void list<T>::insertionSort(int n, node<T>* p){
    for(int r = 0; r < n; r++){
        insertA(search(p->data, r, p), p->data);
        p = p->succ;
        remove(p->pred);
    }
}

template <typename T>
void list<T>::mergeSort(int n, node<T>* & p){
    if(n < 2) return;
    int m = n >> 1;
    node<T>* q = p;
    for(int i = 0; i < m; i++) q = q->succ;

    mergeSort(m, p);
    mergeSort(n - m, q);
    p = merge(p, m, *this, q, n - m);
}

template <typename T>
node<T>* list<T>::merge(node<T>* p, int n, list<T> & L, node<T>* q, int m){
    int count = 1;
    node<T>* pp = p->pred;
    while((0 < m)){
        if((p->data <= q->data) && (0 < n)){
            count++;
            if (q == (p = p->succ)) break;
            n--;
        }
        else{
            count++;
            insertB(p, L.remove((q = q->succ)->pred));
            m--;
        }
    }
    return pp->succ;
}

template <typename T>
void list<T>::sort(char s){
    switch(s){
        case 'm':
            mergeSort(_size, header->succ);
            break;
        case 'i':
            insertionSort(_size, header->succ);
            break;
        case 's':
            selectionSort(_size, header->succ);
            break;
        default:
            cout << "ILLEGAL_INPUT" << endl;
    }
}

#endif