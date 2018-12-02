#ifndef ARRAY_DONE
#define ARRAY_DONE

#include <exception>
#include <iostream>
#include <stdexcept>

#ifndef INCLUDED_ARRAY

#include "Array.h"

#endif

template<typename Type>
Array<Type>::Array(unsigned int size): size_(size), capacity_(std::max(size, 1U)),
                                       constructed(size) {
    arr = static_cast<Type *>(::operator new(capacity_ * sizeof(Type)));
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
Array<Type>::Array(const Array<Type> &a): size_(a.size_),
                                          capacity_(a.capacity_), constructed(a.size()) {
    arr = static_cast<Type *>(::operator new(capacity_ * sizeof(Type)));
    for (unsigned i = 0; i < size_; ++i) {
        ::new(arr + i) Type(a.arr[i]);
    }
}

template<typename Type>
Array<Type> &Array<Type>::operator=(const Array<Type> &a) {
    for (unsigned i = 0; i < capacity_; ++i) {
        arr[i].~Type();
    }
    ::operator delete(arr);
    size_ = a.size_, capacity_ = a.capacity_;
    constructed = a.size_;
    arr = static_cast<Type *>(::operator new(capacity_ * sizeof(Type)));
    for (unsigned i = 0; i < size_; ++i) {
        ::new(arr + i) Type(a.arr[i]);
    }
    return *this;
}

template<typename Type>
Array<Type>::~Array() {
    ::operator delete(arr);
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
    int old = size_;
    if (!(size_ <= newSize && newSize <= capacity_)) {
        reallocate(newSize);
    }
    size_ = newSize;
    for (; old < size_; ++old) {
        if (constructed <= old) {
            ::new((void *) (arr + old)) Type();
            ++constructed;
        }
    }
}

template<typename Type>
void Array<Type>::resize(unsigned newSize, const Type &init) {
    int old = size_;
    if (!(size_ <= newSize && newSize <= capacity_)) {
        reallocate(newSize);
    }
    size_ = newSize;
    for (; old < size_; ++old) {
        if (constructed <= old) {
            ::new((void *) (arr + old)) Type(init);
            ++constructed;
        } else {
            arr[old] = init;
        }
    }
}

template<typename Type>
void Array<Type>::reallocate(unsigned newSize) {
    auto *newArr = static_cast<Type *>(::operator new(newSize * sizeof(Type)));
    for (unsigned i = 0; i < std::min(newSize, size_); ++i) {
        ::new((void *) (newArr + i)) Type(arr[i]);
    }
    for (unsigned i = 0; i < constructed; ++i) {
        arr[i].~Type();
    }
    constructed = std::min(newSize, size_);
    capacity_ = std::max(newSize, 1U);
    std::swap(newArr, arr);
    ::operator delete(newArr);
}

template<typename Type>
typename Array<Type>::Iterator Array<Type>::push(const Type &element) {
    if (size_ == capacity_) {
        reallocate(alpha * capacity_);
    }
    if (size_ < constructed) {
        arr[size_++] = element;
    } else {
        ++constructed;
        ::new(arr + size_++) Type(element);
    }
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
Array<Type>::Array(unsigned size, const Type &fill): size_(size),
                                                     capacity_(size), constructed(size_) {
    arr = static_cast<Type *>(::operator new(size_ * sizeof(Type)));
    for (unsigned i = 0; i < size_; ++i) {
        ::new((void *) (arr + i)) Type(fill);
    }
}

template<typename Type>
template<typename Iterator2>
Array<Type>::Array(Iterator2 begin, Iterator2 end,
                   typename std::enable_if<std::__is_input_iterator<Iterator2>::value &&
                                           std::is_constructible<
                                                   Type,
                                                   typename std::iterator_traits<Iterator2>::reference>::value>::type *)
        : Array() {
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
    Array<Type>::Iterator
            old = *this;
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
    Array<Type>::Iterator
            old = *this;
    this->operator--();
    return old;
}

template<typename Type>
typename Array<Type>::Iterator &Array<Type>::Iterator::operator+=(int k) {
    index = std::min((int)father->size(), index + k);
    if (index < 0) index = father->size();
    return *this;
}

template<typename Type>
typename Array<Type>::Iterator &Array<Type>::Iterator::operator-=(int k) {
    index = std::min((int)father->size(), index - k);
    if (index < 0) index = father->size();
    return *this;
}

template<typename Type>
typename Array<Type>::Iterator Array<Type>::Iterator::operator+(int k) const {
    Array<Type>::Iterator
            result = *this;
    result += k;
    return result;
}

template<typename Type>
typename Array<Type>::Iterator Array<Type>::Iterator::operator-(int k) const {
    Array<Type>::Iterator
            result = *this;
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

template<typename Type>
Type *Array<Type>::Iterator::operator->() {
    return &father->operator[](index);
}

#endif