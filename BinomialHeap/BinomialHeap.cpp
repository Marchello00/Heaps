#ifndef BINHEAP_DONE
#define BINHEAP_DONE

#ifndef INCLUDE_BINHEAP

#include "BinomialHeap.h"

#endif

template<typename Type>
BinomialTree<Type> *BinomialTree<Type>::connect(BinomialTree *nchild) {
    nchild->father = this;
    childrens.push(nchild);
    return this;
}

template<typename Type>
BinomialTree<Type>::BinomialTree(Type *key):
        key(key), father(nullptr) {}

template<typename Type>
BinomialTree<Type>::~BinomialTree() {
    if (key) delete key;
    for (auto *tree : childrens) {
        delete tree;
    }
}

template<typename Type>
const bool BinomialHeap<Type>::empty() const {
    return !size_;
}

template<typename Type>
BinomialHeap<Type>::BinomialHeap(): size_(0), mini(-1) {}

template<typename Type>
const Type BinomialHeap<Type>::getMin() const {
    Base::check();
    return roots[mini]->key->key;
}

template<typename Type>
const Type BinomialHeap<Type>::extractMin() {
    Base::check();
    Type ret = roots[mini]->key->key;
    BinomialTree<HData> *mem = roots[mini];
    roots[mini] = nullptr;
    size_ -= (1U << mini);
    BinomialHeap<Type> newHeap(mem->childrens);
    merge(newHeap);
    mem->childrens.clear();
    norm();
    delete mem;
    return ret;
}

template<typename Type>
BinomialHeap<Type>::BinomialHeap(const Array<BinomialTree<HData> *> &arr):
        roots(arr), mini(-1), size_((1U << arr.size()) - 1) {
    recalc();
}

template<typename Type>
void BinomialHeap<Type>::merge(BinomialHeap &otherHeap) {
    BinomialTree<HData> *rem = nullptr;
    for (int i = 0; i < otherHeap.roots.size() || rem; ++i) {
        if (i >= roots.size()) {
            roots.push(nullptr);
        }
        if (i >= otherHeap.roots.size()) {
            otherHeap.roots.push(nullptr);
        }
        int num = bool(roots[i]) + bool(otherHeap.roots[i]) + bool(rem);
        if (num == 1) {
            roots[i] = connect(connect(roots[i], otherHeap.roots[i]), rem);
            rem = nullptr;
        } else if (num == 2) {
            rem = connect(connect(roots[i], otherHeap.roots[i]), rem);
            roots[i] = nullptr;
        } else {
            rem = connect(rem, otherHeap.roots[i]);
        }
    }
    size_ += otherHeap.size_;
    recalc();
    otherHeap.die();
}

template<typename Type>
BinomialHeap<Type> &BinomialHeap<Type>::operator+=(BinomialHeap &otherHeap) {
    merge(otherHeap);
    return *this;
}

template<typename Type>
BinomialTree<typename BinomialHeap<Type>::HData> *BinomialHeap<Type>::connect(
        BinomialTree<HData> *a, BinomialTree<HData> *b) {
    if (!a) return b;
    if (!b) return a;
    if (a->key->key < b->key->key) {
        return a->connect(b);
    } else {
        return b->connect(a);
    }
}

template<typename Type>
void BinomialHeap<Type>::die() {
    for (auto &c : roots) {
        c = nullptr;
    }
    size_ = 0;
    roots.clear();
}

template<typename Type>
template<typename Iterator>
BinomialHeap<Type>::BinomialHeap(Iterator begin, Iterator end): BinomialHeap() {
    for (; begin != end; ++begin) {
        insert(*begin);
    }
}

template<typename Type>
const typename BinomialHeap<Type>::Pointer BinomialHeap<Type>::insert(const Type &key) {
    return insert(new HData(key));
}

template<typename Type>
void BinomialHeap<Type>::erase(const BinomialHeap::Pointer &ptr) {
    static_cast<HData *>(&*ptr)->toDelete = true;
    Base::change(ptr, getMin());
    extractMin();
}

template<typename Type>
void BinomialHeap<Type>::recalc() {
    mini = -1;
    for (int i = 0; i < roots.size(); ++i) {
        if (!roots[i]) continue;
        roots[i]->father = nullptr;
        if (mini == -1 || roots[i]->key->key < roots[mini]->key->key ||
            roots[i]->key->toDelete) {
            mini = i;
        }
    }
}

template<typename Type>
void BinomialHeap<Type>::swap(BinomialTree<HData> *ftree, BinomialTree<HData> *stree) {
    std::swap(ftree->key, stree->key);
    std::swap(ftree->key->fatherTree, stree->key->fatherTree);
}

template<typename Type>
void BinomialHeap<Type>::siftUp(Pointer ptr) {
    for (auto *now = static_cast<HData *>(&*ptr)->fatherTree, *nt = now->father;
         nt && now->key->key <= nt->key->key; now = nt, nt = nt->father) {
        swap(now, nt);
    }
    recalc();
}

template<typename Type>
void BinomialHeap<Type>::siftDown(Pointer ptr) {
    auto rem = ptr->get();
    auto *save = static_cast<HData *>(&*ptr);
    static_cast<HData *>(&*ptr)->toDelete = true;
    Base::change(ptr, getMin());
    static_cast<HData *>(&*ptr)->fatherTree->key = new HData(rem);
    static_cast<HData *>(&*ptr)->toDelete = false;
    extractMin();
    save->key = rem;
    insert(save);
}

template<typename Type>
const typename BinomialHeap<Type>::Pointer BinomialHeap<Type>::insert(
        BinomialHeap::HData *dt) {
    auto *bt = new BinomialTree<HData>(dt);
    bt->key->fatherTree = bt;
    BinomialHeap<Type> tmp;
    tmp.roots.push(bt);
    tmp.size_++;
    *this += tmp;
    recalc();
    return Pointer(dt, this);
}

template<typename Type>
void BinomialHeap<Type>::norm() {
    for (; !roots.empty() && !roots[-1]; roots.pop());
}

template<typename Type>
const unsigned BinomialHeap<Type>::size() const {
    return size_;
}

template<typename Type>
BinomialHeap<Type>::~BinomialHeap() {
    for (auto *tree : roots) {
        if (tree) delete tree;
    }
}

template<typename Type>
BinomialHeap<Type>::HData::HData(const Type &key, BinomialTree<HData> *father):
        Data(key), fatherTree(father) { }
#endif
