#pragma once

#include <PriorityQueue.h>
#include <List.h>
#include <Array.h>

template<typename Type>
class FibHeap;

template<typename Type>
class FibTree {
    friend class FibHeap<typename Type::type>;

    int degree;

    List<FibTree<Type> *> childrens;
    FibTree<Type> *father;
    typename List<FibTree<Type> *>::Iterator fatherIter;
    Type *key;
    bool mark;

    explicit FibTree(Type * = nullptr);

    FibTree<Type> *connect(FibTree *);

    ~FibTree();
};

template<typename Type>
class FibHeap : public PriorityQueue<Type> {
private:
    typedef typename PriorityQueue<Type>::Data Data;

    class HData : public Data {
        friend class FibHeap<Type>;

        FibTree<HData> *fatherTree;
        bool toDelete = false;

    public:
        typedef Type type;

        explicit HData(const Type &, FibTree<HData> * = nullptr);
    };

    friend class HData;

    typedef PriorityQueue<Type> Base;

public:
    typedef typename PriorityQueue<Type>::Pointer Pointer;

    const bool empty() const override;

    const Pointer insert(const Type &) override;

    const Type getMin() const override;

    const Type extractMin() override;

    void erase(const Pointer &) override;

    void merge(FibHeap &);

    void change(const Pointer &, const Type &) override;

    const unsigned size() const override;

    FibHeap<Type> &operator+=(FibHeap &);

    FibHeap();

    template<typename Iterator2>
    FibHeap(Iterator2, Iterator2);

    ~FibHeap();

private:

    typedef typename List<FibTree<HData> *>::Iterator Iterator;

    Iterator mini;
    unsigned size_;

    List<FibTree<HData> *> roots;

    FibTree<HData> *connect(
            FibTree<HData> *a, FibTree<HData> *b);

    void siftUp(Pointer) override {};

    void siftDown(Pointer) override {};

    const Pointer insert(HData *);

    void die();

    void recalc(Iterator, bool = false);

    void cut(FibTree<HData> *);

    void consolidate();
};

#define INCLUDE_FIBHEAP

#include "FibHeap.cpp"