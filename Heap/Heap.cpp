#ifndef HEAP_DONE
#define HEAP_DONE

#include <cmath>

#ifndef INCLUDED_HEAP

#include "Heap.h"

#endif

template<typename Type>
void Heap<Type>::siftUp(Pointer ptr) {
    for (int ind = static_cast<HData *>(&*ptr)->index;
    ind > BEGIN && heap[ind]->key <= heap[parent(ind)]->key; ind = parent(ind)) {
        swap(ind, parent(ind));
    }
}

template<typename Type>
void Heap<Type>::siftDown(Pointer ptr) {
    for (int ind = static_cast<HData *>(&*ptr)->index;
            child(ind, 0) < heap.size();) {
        unsigned mini = 0;
        for (unsigned i = 0; i < K; ++i) {
            if (child(ind, i) < heap.size() &&
                heap[child(ind, i)]->key < heap[child(ind, mini)]->key) {
                mini = i;
            }
        }
        if (heap[ind]->key <= heap[child(ind, mini)]->key) {
            break;
        }
        swap(child(ind, mini), ind);
        ind = child(ind, mini);
    }
}

template<typename Type>
void Heap<Type>::swap(int ind1, int ind2) {
    std::swap(heap[ind1], heap[ind2]);
    std::swap(static_cast<HData *>(heap[ind1])->index,
              static_cast<HData *>(heap[ind2])->index);
}

template<typename Type>
const unsigned Heap<Type>::parent(int index) const {
    if (index < 0) index += heap.size();
    return index / K;
}

template<typename Type>
const unsigned Heap<Type>::child(int index, int nchild) const {
    if (index < 0) index += heap.size();
    if (nchild < 0) nchild += K;
    return index * K + nchild;
}

template<typename Type>
Heap<Type>::Heap(): heap(1, nullptr) {}

template<typename Type>
template<typename Iterator>
Heap<Type>::Heap(Iterator begin, Iterator end): Heap() {
    for (; begin != end; ++begin) {
        heap.push(new HData(*this, *begin, heap.size()));
    }
    heapify();
}

template<typename Type>
const Type Heap<Type>::getMin() const {
    Base::check();
    return heap[BEGIN]->key;
}

template<typename Type>
const Type Heap<Type>::extractMin() {
    if (last) {
        --last;
        if (wasInsert) {
            K = 2;
            heapify();
        }
        wasInsert = false;
    }
    Base::check();
    Type ret = heap[BEGIN]->key;
    swap(BEGIN, -1);
    heap.pop();
    if (!empty()) siftDown(Pointer(heap[BEGIN]));
    return ret;
}

template<typename Type>
const typename Heap<Type>::Pointer Heap<Type>::insert(const Type &key) {
    auto dt = new HData(*this, key, heap.size());
    heap.push(dt);
    Pointer ptr(dt, this);
    if (last) {
        if (--last) {
            wasInsert = true;
        } else {
            if (!wasInsert) {
                K = 2;
                heapify();
            }
        }
    } else {
        siftUp(ptr);
    }
    return ptr;
}

template<typename Type>
void Heap<Type>::erase(const typename Heap<Type>::Pointer &ptr) {
    Base::change(ptr, getMin());
    extractMin();
}

template<typename Type>
void Heap<Type>::heapify() {
    for (int i = (int)heap.size() - 1; i > 0; --i) {
        siftDown(Pointer(heap[i]));
    }
}

template<typename Type>
const bool Heap<Type>::empty() const {
    return heap.size() <= BEGIN;
}

template<typename Type>
const unsigned Heap<Type>::size() const {
    return heap.size() - BEGIN;
}

template<typename Type>
void Heap<Type>::optimize(unsigned insertCount, unsigned extractCount) {
    unsigned n = insertCount + extractCount;
    if (10 * sqrt(n) > extractCount) {
        K = (unsigned) sqrt(n);
    }
}

template<typename Type>
Heap<Type>::HData::HData(Heap &father, const Type &key, int index):
            Data(father, key), index(index) { }

#endif
