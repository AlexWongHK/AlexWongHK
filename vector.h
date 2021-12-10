#include <iostream>
#define rank int
#define DEFAULT_CAPACITY 4
using namespace std;

template <typename T>
class vector{
private:
    int _size;
    int _capacity;
    T* _elem;
public:
    vector<T>(int c = DEFAULT_CAPACITY){ _elem = new T[_capacity = c]; _size = 0;}
    ~vector<T>(){delete [] _elem;}
    void expand();
    void shrink();
    int size(){ return _size; }
    int capacity(){ return _capacity; }
    T get(rank r); //{return _elem[r];}
    T & operator[](rank r);
    rank put(rank r, T e);
    rank insert(rank r, T e);
    rank remove(rank lo, rank hi);
    T remove(rank r);
    rank find(T e, rank lo, rank hi);
    rank deduplicate();
    template<typename VST> void traverse(VST & visit);
    rank uniquify();
    rank binSearch(T e, rank lo, rank hi);
    rank binSearch_I1(T e, rank lo, rank hi);
    rank binSearch_I2(T e, rank lo, rank hi);
    //rank fibsearch(T e, rank lo, rank hi){} to be complete
    void bubbleSort(rank lo, rank hi);
    void mergeSort(rank lo, rank hi);
    void merge(rank lo, rank mi, rank hi);
};

template <typename T>
void vector<T>::expand(){
    if ( _size < _capacity) return;
    T* _oldElem = _elem;
    _elem = new T[_capacity <<= 1];
    for ( int i = 0; i < _size ; i++){
        _elem[i] = _oldElem[i];
    }
    delete [] _oldElem;
}

template <typename T>
void vector<T>::shrink(){
    if ( (_capacity >> 1) < _size || _size < DEFAULT_CAPACITY) return;
    T* _oldElem = _elem;
    _elem = new T[_capacity >>= 1];
    for ( int i = 0; i < _size ; i++){
        _elem[i] = _oldElem[i];
    }
    delete [] _oldElem;
}

template <typename T>
T vector<T>::get(rank r){
    try{
        if(r < 0 || _size <= r) throw(r);
        return _elem[r];
    }
    catch(int e){
        cout << "OUT_OF_RANGE ";
        cout << "Rank " << e ;
        cout << " Size " << _size << endl;
        exit(-1);
    }
}

template <typename T> 
T & vector<T>::operator[](rank r){
    try{
        if(r < 0 || _size <= r) throw(r);
        return _elem[r];
    }
    catch(int e){
        cout << "OUT_OF_RANGE ";
        cout << "Rank " << e ;
        cout << " Size " << _size << endl;
        exit(-1);
    }
}

template <typename T>
rank vector<T>::put(rank r, T e){
    _elem[r] = e;
    return (r);
}

template <typename T>
rank vector<T>::insert(rank r, T e){
    expand();
    for(int i = _size; i > r; i--){
        _elem[i] = _elem[i-1];
    }
    _elem[r] = e;
    _size++;
    return r;
}

template <typename T>
rank vector<T>::remove(rank lo, rank hi){
    if (lo == hi) return 0;
    while (hi < _size){
        _elem[lo++] = _elem[hi++];
    }
    _size = lo;
    shrink();
    return hi - lo;
}

template <typename T>
T vector<T>::remove(rank r){
    T e = _elem[r];
    remove(r, r+1);
    return e;
}

template <typename T>
rank vector<T>::find(T e, rank lo, rank hi){
    while((lo < hi--) && (e != _elem[hi]));
    return hi;
}

template <typename T>
rank vector<T>::deduplicate(){
    int _oldsize = _size;
    rank i = 1;
    while (i < _size){
        if (find(_elem[i], 0, i) < 0) i++;
        else remove(i);
    }
    return _oldsize - _size;
}

template <typename T> template <typename VST>
void vector<T>:: traverse(VST & visit){
    for(int i = 0; i < _size ; i++) visit(_elem[i]);
}

template <typename T>
rank vector<T>::uniquify(){
    rank i = 0, j = 0;
    while(++j < _size){
        if (_elem[i] != _elem[j]){
            _elem[++i] = _elem[j];
        }
    }
    _size = ++i;
    shrink();
    return j - i;
}

template <typename T>
rank vector<T>::binSearch(T e, rank lo, rank hi){
    while ( lo < hi ){
        rank mi = (lo + hi) >> 1;
        if (e < _elem[mi]) hi = mi;
        else if (_elem[mi] < e) lo = mi + 1;
        else return mi;
    }
    return -1;
}

template <typename T>
rank vector<T>::binSearch_I1(T e, rank lo, rank hi){
    while(1 < hi - lo){
        rank mi = (lo + hi) >> 1;
        e < _elem[mi] ? hi = mi:lo = mi;
    }
    return e == _elem[lo]? _elem[lo]:-1;
}

template <typename T>
rank vector<T>::binSearch_I2(T e, rank lo, rank hi){
    while(lo < hi){
        rank mi = (lo + hi) >> 1;
        e < _elem[mi] ? hi = mi:lo = mi+1;
    }
    return lo - 1;
}

template<typename T>
void vector<T>::bubbleSort(rank lo, rank hi){
    for(bool sorted = false; sorted = !sorted;){
        while(lo < --hi){
            for(int i = lo; i < hi; i++){
                if (_elem[i] > _elem[i+1]){
                    swap(_elem[i], _elem[i+1]);
                    sorted = false;
                }
            }
        }
    }
}

template<typename T>
void vector<T>::mergeSort(rank lo, rank hi){
    if(hi - lo < 2) return;
    rank mi = (lo + hi) >> 1;
    mergeSort(lo, mi);
    mergeSort(mi, hi);
    merge(lo, mi, hi);
}

template<typename T>
void vector<T>::merge(rank lo, rank mi, rank hi){
    rank i = 0;
    T* A = _elem + lo;
    rank j = 0, lb = mi - lo;
    T* B = new T[lb];
    for(rank i = 0; i < lb; i++) B[i] = A[i];
    rank k = 0, lc = hi - mi;
    T* C = _elem + mi;

    while((j < lb) && (k < lc)){
        A[i++] = (B[j] <= C[k]? B[j++]:C[k++]);
    }
    while(j < lb){
        A[i++] = B[j++];
    }
    delete[] B;
}