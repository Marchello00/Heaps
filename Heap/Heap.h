#pragma once

#include <Array.h>
#include <PriorityQueue.h>

template<typename Type>
class Heap: public PriorityQueue<Type> {
private:
    typedef PriorityQueue<Type> Base;
    typedef typename PriorityQueue<Type>::Data Data;

    class HData: public Data {
    public:
        int index;
        HData(Heap &, const Type &, int);
    };

public:
    typedef typename PriorityQueue<Type>::Pointer Pointer;

    const bool empty() const override;

    const Type getMin() const override;

    const Type extractMin() override;

    const Pointer insert(const Type &) override;

    void erase(const Pointer &) override;

    const unsigned size() const override;

    void optimize(unsigned, unsigned);

    Heap();

    template<typename Iterator>
    Heap(Iterator, Iterator);

private:
    const unsigned K = 2;
    const int BEGIN = 1;

    Array<Data *> heap;

    const unsigned parent(int) const;
    const unsigned child(int, int) const;

    void siftUp(Pointer) override;

    void siftDown(Pointer) override;

    void heapify();

    void swap(int, int);
};

#define INCLUDED_HEAP

#include "Heap.cpp"