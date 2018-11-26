#ifndef INCLUDED_LIST

#include <stdexcept>
#include "List.h"

#endif

template<typename Type>
List<Type>::Node::Node(const Type &key):
        key(key), next(nullptr), prev(nullptr) {}

template<typename Type>
void List<Type>::Node::connect(List::Node *to) {
    this->next = to;
    to->prev = this;
}

template<typename Type>
void List<Type>::Node::cutFront() {
    this->prev = nullptr;
}

template<typename Type>
void List<Type>::Node::cutBack() {
    this->next = nullptr;
}

template<typename Type>
List<Type>::List():
    head(nullptr), tail(nullptr) { }

template<typename Type>
List<Type>::List(const Type &key) {
    head = tail = new Node(key);
}

template<typename Type>
typename List<Type>::Iterator List<Type>::pushFront(const Type &key) {
    auto nl = List(key);
    nl.connect(*this);
    *this = nl;
    return begin();
}

template<typename Type>
typename List<Type>::Iterator List<Type>::pushBack(const Type &key) {
    List here(key);
    auto ret = here.begin();
    connect(here);
    return ret;
}

template<typename Type>
List<Type> *List<Type>::connect(List &to) {
    if (!to.head) {
        return this;
    }
    if (!tail) {
        *this = to;
    } else {
        tail->connect(to.head);
        tail = to.tail;
    }
    to.forget();
    return this;
}

template<typename Type>
List<Type>::~List() {
    for (auto now = begin(); now != end(); delete (now++).father) { }
}

template<typename Type>
const Type List<Type>::popFront() {
    check();
    auto old = head->getNext();
    auto ret = old->key;
    delete old;
    if (head) head->cutFront();
    return ret;
}

template<typename Type>
bool List<Type>::empty() const {
    return !head && !tail;
}

template<typename Type>
void List<Type>::check() const {
    if (empty()) {
        throw std::out_of_range("List is empty!");
    }
}

template<typename Type>
template<typename Iterator2>
List<Type>::List(const Iterator2 &begin, const Iterator2 &end): List() {
    for (auto it = begin; it != end; ++it) {
        pushBack(*it);
    }
}

template<typename Type>
const Type List<Type>::popBack() {
    auto old = tail;
    auto ret = old->key;
    tail = old->getPrev();
    delete old;
    if (tail) tail->cutBack();
    return ret;
}

template<typename Type>
List<Type> &List<Type>::operator+=(List &to) {
    return *connect(to);
}

template<typename Type>
typename List<Type>::Iterator List<Type>::begin() const {
    return List::Iterator(head);
}

template<typename Type>
typename List<Type>::Iterator List<Type>::end() const {
    return List::Iterator(nullptr);
}

template<typename Type>
void List<Type>::forget() {
    head = tail = nullptr;
}

template<typename Type>
List<Type> &List<Type>::operator+=(const Type &element) {
    pushBack(element);
    return *this;
}

template<typename Type>
List<Type>::List(List &old) {
    head = old.head;
    tail = old.tail;
    old.forget();
}

template<typename Type>
void List<Type>::erase(typename List<Type>::Iterator it) {
    auto *f = it.father;
    if (f->prev) {
        f->prev->next = f->next;
    }
    if (f->next) {
        f->next->prev = f->prev;
    }
    if (head == f) {
        head = f->next;
    }
    if (tail == f) {
        tail = f->prev;
    }
    delete f;
}

template<typename Type>
Type &List<Type>::Iterator::operator*() const {
    return father->key;
}

template<typename Type>
Type *List<Type>::Iterator::operator->() const {
    return father->key;
}

template<typename Type>
typename List<Type>::Iterator &List<Type>::Iterator::operator++() {
    *this = Iterator(father->next);
    return *this;
}

template<typename Type>
List<Type>::Iterator::Iterator(List::Node *father): father(father) { }

template<typename Type>
typename List<Type>::Iterator &List<Type>::Iterator::operator--() {
    father = father->getPrev();
    return this;
}

template<typename Type>
const typename List<Type>::Iterator List<Type>::Iterator::operator++(int) {
    auto ret = *this;
    this->operator++();
    return ret;
}

template<typename Type>
const typename List<Type>::Iterator List<Type>::Iterator::operator--(int) {
    auto ret = *this;
    this->operator--();
    return ret;
}

template<typename Type>
bool List<Type>::Iterator::operator==(const List::Iterator &a) const {
    return father == a.father;
}

template<typename Type>
bool List<Type>::Iterator::operator!=(const List::Iterator &a) const {
    return !(*this == a);
}

template<typename Type>
bool List<Type>::Iterator::operator!() const {
    return !father;
}

template<typename Type>
List<Type>::Iterator::operator bool() const {
    return father;
}
