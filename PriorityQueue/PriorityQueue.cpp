#ifndef INCLUDED_PQ

#include "PriorityQueue.h"

#endif

template<typename Type>
void PriorityQueue<Type>::check() const {
    if (empty()) {
        throw std::out_of_range("There is no minimum in heap because heap is empty!");
    }
}

template<typename Type>
void PriorityQueue<Type>::change(
        const typename PriorityQueue<Type>::Pointer &ptr, const Type &key) {
    if (key == ptr->key) return;
    if (key < ptr->key) {
        ptr->key = key;
        siftUp(ptr);
    } else {
        ptr->key = key;
        siftDown(ptr);
    }
}

template<typename Type>
typename PriorityQueue<Type>::Data &PriorityQueue<Type>::Pointer::operator*() const {
    object->father = father;
    return *object;
}

template<typename Type>
PriorityQueue<Type>::Pointer::Pointer(PriorityQueue::Data *a, PriorityQueue<Type> *f):
        object(a), father(f) {}

template<typename Type>
typename PriorityQueue<Type>::Data *PriorityQueue<Type>::Pointer::operator->() const {
    object->father = father;
    return object;
}

template<typename Type>
typename PriorityQueue<Type>::Data &PriorityQueue<Type>::Data::operator=(const Type &key) {
    father->change(Pointer(this), key);
    return *this;
}

template<typename Type>
PriorityQueue<Type>::Data::Data(PriorityQueue &father, const Type &key):
        father(&father), key(key) {}

template<typename Type>
PriorityQueue<Type>::Data::Data(const Type &key): key(key), father(nullptr) {}

template<typename Type>
Type PriorityQueue<Type>::Data::get() const {
    return key;
}

template<typename Type>
std::ostream &operator<<(std::ostream &out, const typename PriorityQueue<Type>::Data &a) {
    out << a.key;
    return out;
}