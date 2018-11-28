#ifndef INCLUDE_FIBHEAP

#include "FibHeap.h"

#endif

template<typename Type>
FibTree<Type> *FibTree<Type>::connect(FibTree *nchild) {
    ++degree;
    nchild->father = this;
    nchild->fatherIter = childrens.pushBack(nchild);
    return this;
}

template<typename Type>
FibTree<Type>::FibTree(Type *key):
        key(key), father(nullptr),
        fatherIter(nullptr), mark(false), degree(0) {}

template<typename Type>
FibTree<Type>::~FibTree() {
    if (key) delete key;
    for (auto *tree : childrens) {
        delete tree;
    }
}

template<typename Type>
const bool FibHeap<Type>::empty() const {
    return !size_;
}

template<typename Type>
FibHeap<Type>::FibHeap(): size_(0), mini(nullptr) {}

template<typename Type>
const Type FibHeap<Type>::getMin() const {
    Base::check();
    return (*mini)->key->key;
}


template<typename Type>
const Type FibHeap<Type>::extractMin() {
    Base::check();
    Type ret = (*mini)->key->key;
    size_--;
    roots += (*mini)->childrens;
    (*mini)->childrens.forget();
    delete(*mini);
    roots.erase(mini);
    consolidate();
    return ret;
}

template<typename Type>
void FibHeap<Type>::merge(FibHeap &otherHeap) {
    recalc(otherHeap.mini);
    roots += otherHeap.roots;
    size_ += otherHeap.size_;
    otherHeap.die();
}

template<typename Type>
FibHeap<Type> &FibHeap<Type>::operator+=(FibHeap &otherHeap) {
    merge(otherHeap);
    return *this;
}

template<typename Type>
FibTree<typename FibHeap<Type>::HData> *FibHeap<Type>::connect(
        FibTree<HData> *a, FibTree<HData> *b) {
    if (!a) return b;
    if (!b) return a;
    if (a->key->key < b->key->key) {
        return a->connect(b);
    } else {
        return b->connect(a);
    }
}

template<typename Type>
void FibHeap<Type>::die() {
    size_ = 0;
    roots.forget();
}

template<typename Type>
template<typename Iterator2>
FibHeap<Type>::FibHeap(Iterator2 begin, Iterator2 end) {
    for (; begin != end; ++begin) {
        insert(*begin);
    }
}

template<typename Type>
const typename FibHeap<Type>::Pointer FibHeap<Type>::insert(const Type &key) {
    return insert(new HData(key));
}

template<typename Type>
void FibHeap<Type>::erase(const FibHeap::Pointer &ptr) {
    static_cast<HData *>(&*ptr)->toDelete = true;
    Base::change(ptr, getMin());
    extractMin();
}

template<typename Type>
void FibHeap<Type>::recalc(Iterator it, bool force) {
    if (!mini || force) {
        mini = it;
        return;
    }
    if ((*it)->key->key < (*mini)->key->key || (*it)->key->toDelete) {
        mini = it;
    }
}

template<typename Type>
const typename FibHeap<Type>::Pointer FibHeap<Type>::insert(
        FibHeap::HData *dt) {
    auto *bt = new FibTree<HData>(dt);
    bt->key->fatherTree = bt;
    bt->fatherIter = roots.pushBack(bt);
    recalc(bt->fatherIter);
    ++size_;
    return Pointer(dt, this);
}

template<typename Type>
const unsigned FibHeap<Type>::size() const {
    return size_;
}

template<typename Type>
void FibHeap<Type>::change(const typename FibHeap<Type>::Pointer &ptr, const Type &key) {
    if (ptr->get() == key) return;
    auto *rem = static_cast<HData *>(&*ptr);
    if (key < ptr->get()) {
        rem->key = key;
        cut(rem->fatherTree);
        recalc(rem->fatherTree->fatherIter);
    } else {
        cut(rem->fatherTree);
        rem->fatherTree->key = new HData(key);
        recalc(rem->fatherTree->fatherIter, true);
        extractMin();
        rem->key = key;
        insert(rem);
    }
}

template<typename Type>
void FibHeap<Type>::cut(FibTree<FibHeap::HData> *node) {
    if (!node->father)
        return;
    node->mark = false;
    node->father->degree--;
    node->father->childrens.erase(node->fatherIter);
    node->fatherIter = roots.pushBack(node);
    if (node->father->mark) {
        cut(node->father);
    }
    node->father = nullptr;
}

template<typename Type>
void FibHeap<Type>::consolidate() {
    Array<FibTree<HData> *> res;
    List<FibTree<HData> *> q = roots;
    for (auto *tree : q) {
        if (tree->degree >= res.size()) {
            res.resize((unsigned)tree->degree + 1, nullptr);
        }
        if (res[tree->degree]) {
            int old = tree->degree;
            q += connect(res[old], tree);
            res[old] = nullptr;
        } else {
            res[tree->degree] = tree;
        }
    }
    q.forget();
    roots.forget();
    mini = roots.end();
    for (auto *tree : res) {
        if (tree) {
            tree->fatherIter = roots.pushBack(tree);
            recalc(tree->fatherIter);
            tree->father = nullptr;
        }
    }
}

template<typename Type>
FibHeap<Type>::~FibHeap() {
    for (auto *tree : roots) {
        delete tree;
    }
}

template<typename Type>
FibHeap<Type>::HData::HData(const Type &key, FibTree<HData> *father):
        Data(key), fatherTree(father) {}
