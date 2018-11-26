#pragma once

#include <PriorityQueue.h>
#include <Array.h>

template<typename Type>
class BinomialHeap;

template<typename Type>
class BinomialTree {
    friend class BinomialHeap<typename Type::type>;
    Array<BinomialTree<Type> *> childrens;

    BinomialTree<Type> *father;

    Type *key;

    explicit BinomialTree(Type * = nullptr);

    BinomialTree<Type> *connect(BinomialTree *);

    ~BinomialTree();
};

template<typename Type>
class BinomialHeap: public PriorityQueue<Type> {
private:
    typedef typename PriorityQueue<Type>::Data Data;
    class HData: public Data {
        friend class BinomialHeap<Type>;

        BinomialTree<HData> * fatherTree;
        bool toDelete = false;

    public:
        typedef Type type;

        explicit HData(const Type &, BinomialTree<HData> * = nullptr);
    };

    friend class HData;

public:
    typedef PriorityQueue<Type> Base;
    typedef typename PriorityQueue<Type>::Pointer Pointer;

    const bool empty() const override;

    const Pointer insert(const Type &) override;

    const Type getMin() const override;

    const Type extractMin() override;

    void erase(const Pointer &) override;

    void merge(BinomialHeap &);

    const unsigned size() const override;

    BinomialHeap<Type> &operator+=(BinomialHeap &);

    BinomialHeap();

    explicit BinomialHeap(const Array<BinomialTree<HData> *> &);

    template<typename Iterator>
    BinomialHeap(Iterator, Iterator);

    ~BinomialHeap();

private:

    int mini;

    unsigned size_;
    Array<BinomialTree<HData> *> roots;

    BinomialTree<HData> *connect(
            BinomialTree<HData> *a, BinomialTree<HData> *b);

    void siftUp(Pointer) override;

    void siftDown(Pointer) override;

    const Pointer insert(HData *);

    void die();

    void recalc();

    void norm();

    void swap(BinomialTree<HData> *, BinomialTree<HData> *);
};

#define INCLUDE_BINHEAP

#include "BinomialHeap.cpp"