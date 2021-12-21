#include <iostream>
#include "release.h"
#include "stack.h"
#include "queue.h"
using namespace std;
#ifndef _TREE_
#define _TREE_
#define stature(p) ((p)? (p)->height : -1)
#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x) && ( & (x) == (x).parent->lc))
#define IsRChild(x) (!IsRoot(x) && ( & (x) == (x).parent->rc))
#define HasParent(x) (IsRoot(x))
#define HasLChild(x) ((x).lc)
#define HasRChild(x) ((x).rc)
#define HasChild(x) (HasLChild(x) || HasRChild(x))
#define HasBothChild(x) (HasLChild(x) && HasRChild(x))
#define IsLeaf(x) (!HasChild(x))
#define sibling(p) (IsLChild(*(p))? (p)->parent->rc : (p)->parent->lc)
#define uncle(x) (IsLChild(*((x)->parent))? (x)->parent->parent->rc : (x)->parent->parent->lc)
#define FromParentTo(x) (IsRoot(x)? _root:(IsLChild(x)? ((x).parent->lc):(x).parent->rc))

template <typename T>
class binNode{
public:
    T data;
    int height;
    binNode<T>* parent;
    binNode<T>* lc;
    binNode<T>* rc;
    binNode<T>() : parent(NULL), lc(NULL), rc(NULL), height(0) {}
    binNode<T>(const binNode<T> & x) : data(x.data), parent(x.parent), lc(x.lc), rc(x.rc), height(x.height) {}
    binNode<T>(T e, binNode<T>* p = NULL, binNode<T>* l = NULL, binNode<T>* r = NULL, int h = 0) : data(e), parent(p), lc(l), rc(r), height(h) {}
    int size();
    binNode<T>* insertAsLC(T const & e);
    binNode<T>* insertAsRC(T const & e);
    binNode<T>* succ();
    template<typename VST> void travPre(VST& visit);
    template<typename VST> void travIn(VST& visit);
    template<typename VST> void travPost(VST& visit);
    template<typename VST> void travLevel(VST& visit);
    bool operator>(binNode<T> const & bn) { return data > bn.data; }
    bool operator<(binNode<T> const & bn) { return data > bn.data; }
    bool operator==(binNode<T> const & bn) { return data == bn.data; }
    bool operator!=(binNode<T> const & bn) { return data != bn.data; }
};

template <typename T>
int binNode<T>::size(){
    int s = 1;
    if(lc) s += lc->size();
    if(rc) s += rc->size();
    return s;
}

template <typename T>
binNode<T>* binNode<T>::insertAsLC(T const & e){
    lc = new binNode<T>(e, this);
    return lc;
}

template <typename T>
binNode<T>* binNode<T>::insertAsRC(T const & e){
    rc = new binNode<T>(e, this);
    return rc;
}

template <typename T>
binNode<T>* binNode<T>::succ(){
    binNode<T>* s = this;
    if(rc){
        s = s->rc;
        while(HasLChild(*s)) s = s->lc;
    }
    else{
        while(IsRChild(*s)){
            s = s->parent;
        }
        s = s->parent;
    }
    return s;
}

template <typename T, typename VST>
void travPre_R(binNode<T>* x, VST& visit){
    if(!x) return;
    visit(x->data);
    travPre_R(x->lc, visit);
    travPre_R(x->rc, visit);
}

template <typename T, typename VST>
void travIn_R(binNode<T>* x, VST& visit){
    if(!x) return;
    travIn_R(x->lc, visit);
    visit(x->data);
    travIn_R(x->rc, visit);
}

template <typename T, typename VST>
void travPost_R(binNode<T>* x, VST& visit){
    if(!x) return;
    travPost_R(x->lc, visit);
    travPost_R(x->rc, visit);
    visit(x->data);
}

template <typename T, typename VST>
static void visitAloneLeftBranch(binNode<T>* x, VST& visit, stack<binNode<T>*>& S){
    while(x){
        visit(x->data);
        S.push(x->rc);
        x = x->lc;
    }
}

template <typename T, typename VST>
void travPre_I(binNode<T>* x, VST& visit){
    stack<binNode<T>*> S;
    while(true){
        visitAloneLeftBranch(x, visit, S);
        if(S.empty()) break;
        x = S.pop();
    }
}

template <typename T>
static void goAloneLeftBranch(binNode<T>* x, stack<binNode<T>*>& S){
    while(x){
        S.push(x);
        x = x->lc;
    }
}

template <typename T, typename VST>
void travIn_I(binNode<T>* x, VST& visit){
    stack<binNode<T>*> S;
    while(true){
        goAloneLeftBranch(x, S);
        if(S.empty()) break;
        x = S.pop();
        visit(x->data);
        x = x->rc;
    }
}

template <typename T, typename VST>
void travIn_I2(binNode<T>* x, VST& visit){
    stack<binNode<T>*> S;
    while(true){
        if(x){
            S.push(x);
            x = x->lc;
        }
        else if(!S.empty()){
            x = S.pop();
            visit(x->data);
            x = x->rc;
        }
        else break;
    }
}

template <typename T>
static void gotoHLVFL(stack<binNode<T>*>& S){
    while(binNode<T>* x = S.top()){
        if(HasLChild(*x)){
            if(HasRChild(*x)) S.push(x->rc);
            S.push(x->lc);
        }
        else{
            S.push(x->rc);
        }
    }
    S.pop();
}

template <typename T, typename VST>
void travPost_I(binNode<T>* x, VST& visit){
    stack<binNode<T>*> S;
    if(x) S.push(x);
    while(!S.empty()){
        if(S.top() != x->parent) gotoHLVFL(S);
        x = S.pop();
        visit(x->data);
    }
}

template <typename T> template <typename VST>
void binNode<T>::travPre(VST& visit){
    travPre_I(this, visit);
}

template <typename T> template <typename VST>
void binNode<T>::travIn(VST& visit){
    travIn_I(this, visit);
}

template <typename T> template <typename VST>
void binNode<T>::travPost(VST& visit){
    travPost_I(this, visit);
}

template <typename T> template <typename VST>
void binNode<T>::travLevel(VST& visit){
    queue<binNode<T>*> Q;
    Q.enqueue(this);
    while(!Q.empty()){
        binNode<T>* x = Q.dequeue();
        visit(x->data);
        if(HasLChild(*x)) Q.enqueue(x->lc);
        if(HasRChild(*x)) Q.enqueue(x->rc);
    }
}

template <typename T>
class binTree{
protected:
    int _size;
    binNode<T>* _root;
    virtual int updateHeight(binNode<T>* x);
    void updateHeightAbove(binNode<T>* x);
public:
    binTree<T>() : _size(0), _root(NULL) {}
    binTree<T>(const binTree<T> &BT) {}
    ~binTree<T>() {if(0 < _size) remove(_root); }
    int size() const { return _size; }
    bool empty() const { return _root; }
    binNode<T>* root() const { return _root; }
    binNode<T>* insertAsRoot(T const & e);
    binNode<T>* insertAsLC(binNode<T>* x, T const & e);
    binNode<T>* insertAsRC(binNode<T>* x, T const & e);
    binNode<T>* attachAsLC(binNode<T>* x, binTree<T>* &S);
    binNode<T>* attachAsRC(binNode<T>* x, binTree<T>* &S);
    int remove(binNode<T>* x);
    binTree<T>* secede(binNode<T>* x);
    template<typename VST> void travPre(VST& visit) { if(_root) _root->travPre(visit); }
    template<typename VST> void travIn(VST& visit) { if(_root) _root->travIn(visit); }
    template<typename VST> void travPost(VST& visit) { if(_root) _root->travPost(visit); }
    template<typename VST> void travLevel(VST& visit) { if(_root) _root->travLevel(visit); }
};

template <typename T>
int binTree<T>::updateHeight(binNode<T>* x){
    return x->height = 1 + max(stature(x->lc), stature(x->rc));
}

template <typename T>
void binTree<T>::updateHeightAbove(binNode<T>* x){
    while(x){
        updateHeight(x);
        x = x->parent;
    }
}

template <typename T>
binNode<T>* binTree<T>::insertAsRoot(T const & e){
    _size = 1;
    return _root = new binNode<T>(e);
}

template <typename T>
binNode<T>* binTree<T>::insertAsLC(binNode<T>* x, T const & e){
    _size++;
    x->insertAsLC(e);
    updateHeightAbove(x);
    return x->lc;
}

template <typename T>
binNode<T>* binTree<T>::insertAsRC(binNode<T>* x, T const & e){
    _size++;
    x->insertAsRC(e);
    updateHeightAbove(x);
    return x->rc;
}

template <typename T>
binNode<T>* binTree<T>::attachAsLC(binNode<T>* x, binTree<T>* &S){
    if(x->lc = S->root){
        x->lc->parent = x;
    }
    _size += S->_size;
    updateHeightAbove(x);
    S->_root = NULL;
    S->_size = 0;
    return x;
}

template <typename T>
binNode<T>* binTree<T>::attachAsRC(binNode<T>* x, binTree<T>* &S){
    if(x->rc = S->root){
        x->rc->parent = x;
    }
    _size += S->_size;
    updateHeightAbove(x);
    S->_root = NULL;
    S->_size = 0;
    release(S);
    S = NULL;
    return x;
}

template <typename T>
int binTree<T>::remove(binNode<T>* x){
    FromParentTo(*x) = NULL;
    updateHeightAbove(x->parent);
    int n = removeAt(x);
    _size -= n;
    return n;
}

template <typename T>
static int removeAt(binNode<T>* x){
    if (!x) return 0;
    int n = 1 + removeAt(x->lc) + removeAt(x->rc);
    release(x->data);
    release(x);
    return n;
}

template <typename T>
binTree<T>* binTree<T>::secede(binNode<T>* x){
    FromParentTo(*x) = NULL;
    updateHeightAbove(x->parent);
    binTree<T>* S = new binTree<T>;
    S->_root = x;
    x->parent = NULL;
    S->_size = x->size();
    _size -= S->_size;
    return S;
}

#endif