#include <exception>
#include <iostream>
#include <stdexcept>

#ifndef INCLUDED_ARRAY

#include "Array.h"

#endif

template<typename Type>
Array<Type>::Array(unsigned int size): size_(size), capacity_(std::max(size, 1U)) {
    arr = new Type[size];
}

template<typename Type>
Type &Array<Type>::operator[](int index) {
    if (index < 0) {
        index = size_ + index;
    }
    if (!(0 <= index && index < size_)) {
        throw std::out_of_range("Array size_ is " + std::to_string(size_) + ", so there is no "
                                                                           "element with index " +
                                std::to_string(index) + ".");
    }
    return arr[index];
}

template<typename Type>
const Type Array<Type>::operator[](int index) const {
    if (index < 0) {
        index = size_ + index;
    }
    if (!(0 <= index && index < size_)) {
        throw std::out_of_range("Array size_ is " + std::to_string(size_) + ", so there is no "
                                                                           "element with index " +
                                std::to_string(index) + ".");
    }
    return arr[index];
}

template<typename Type>
Array<Type>::Array(const Array<Type> &a): size_(a.size_), capacity_(a.capacity_) {
    arr = new Type[capacity_];
    for (unsigned i = 0; i < size_; ++i) {
        arr[i] = a.arr[i];
    }
}

template<typename Type>
Array<Type> &Array<Type>::operator=(const Array<Type> &a) {
    delete[] arr;
    size_ = a.size_, capacity_ = a.capacity_;
    arr = new Type[capacity_];
    for (unsigned i = 0; i < size_; ++i) {
        arr[i] = a.arr[i];
    }
    return *this;
}

template<typename Type>
Array<Type>::~Array() {
    delete[] arr;
}

template<typename Type>
const bool Array<Type>::empty() const {
    return size_ == 0;
}

template<typename Type>
const unsigned Array<Type>::size() const {
    return size_;
}

template<typename Type>
const unsigned Array<Type>::capacity() const {
    return capacity_;
}

template<typename Type>
void Array<Type>::resize(unsigned newSize) {
    if (size_ <= newSize && newSize <= capacity_) {
        size_ = newSize;
        return;
    }
    reallocate(newSize);
    size_ = newSize;
}

template<typename Type>
void Array<Type>::resize(unsigned newSize, const Type &init) {
    int old = size_;
    resize(newSize);
    for (; old < newSize; ++old) {
        arr[old] = init;
    }
}

template<typename Type>
void Array<Type>::reallocate(unsigned newSize) {
    auto *newArr = new Type[newSize];
    capacity_ = std::max(newSize, 1U);
    for (unsigned i = 0; i < std::min(newSize, size_); ++i) {
        newArr[i] = arr[i];
    }
    std::swap(newArr, arr);
    delete[] newArr;
}

template<typename Type>
typename Array<Type>::Iterator Array<Type>::push(const Type &element) {
    if (size_ == capacity_) {
        reallocate(alpha * capacity_);
    }
    arr[size_++] = element;
    return getIterator(size_ - 1);
}

template<typename Type>
typename Array<Type>::Iterator Array<Type>::getIterator(const unsigned index) {
    return Iterator(this, index);
}

template<typename Type>
typename Array<Type>::Iterator Array<Type>::begin() {
    return getIterator(0);
}

template<typename Type>
typename Array<Type>::Iterator Array<Type>::end() {
    return getIterator(size_);
}

template<typename Type>
void Array<Type>::pop() {
    if (empty()) {
        throw std::out_of_range("Array is empty, you can't use pop()"
                                " from empty array");
    }
    --size_;
    if (size_ * beta <= capacity_) {
        reallocate(size_ * alpha);
    }
}

template<typename Type>
Array<Type>::Array(unsigned size, const Type &fill): size_(size), capacity_(size) {
    arr = new Type[size_];
    for (unsigned i = 0; i < size_; ++i) {
        arr[i] = fill;
    }
}

template<typename Type>
template<typename Iterator2>
Array<Type>::Array(Iterator2 begin, Iterator2 end): Array() {
    for (; begin != end; ++begin) {
        push(*begin);
    }
}

template<typename Type>
void Array<Type>::clear() {
    resize(0);
}

template<typename Type>
typename Array<Type>::Iterator &Array<Type>::Iterator::operator++() {
    if (index < father->size_) ++index;
    return *this;
}

template<typename Type>
const typename Array<Type>::Iterator Array<Type>::Iterator::operator++(int) {
    Array<Type>::Iterator old = *this;
    this->operator++();
    return old;
}

template<typename Type>
typename Array<Type>::Iterator &Array<Type>::Iterator::operator--() {
    if (index > 0) --index;
    return *this;
}

template<typename Type>
const typename Array<Type>::Iterator Array<Type>::Iterator::operator--(int) {
    Array<Type>::Iterator old = *this;
    this->operator--();
    return old;
}

template<typename Type>
typename Array<Type>::Iterator &Array<Type>::Iterator::operator+=(unsigned k) {
    index = std::max(father->size(), index + k);
    return *this;
}

template<typename Type>
typename Array<Type>::Iterator &Array<Type>::Iterator::operator-=(unsigned k) {
    k = std::min(k, index);
    index -= k;
    return *this;
}

template<typename Type>
typename Array<Type>::Iterator Array<Type>::Iterator::operator+(unsigned k) const {
    Array<Type>::Iterator result = *this;
    result += k;
    return result;
}

template<typename Type>
typename Array<Type>::Iterator Array<Type>::Iterator::operator-(unsigned k) const {
    Array<Type>::Iterator result = *this;
    result -= k;
    return result;
}

template<typename Type>
Type &Array<Type>::Iterator::operator*() {
    return father->operator[](index);
}

template<typename Type>
Array<Type>::Iterator::Iterator(Array<Type> *const father, unsigned index):
        father(father), index(index) {}

template<typename Type>
bool Array<Type>::Iterator::operator==(const Array::Iterator &to) const {
    return this->father == to.father &&
           this->index == to.index;
}

template<typename Type>
bool Array<Type>::Iterator::operator!=(const Array::Iterator &to) const {
    return !(*this == to);
}
