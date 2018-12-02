#ifndef LIST_DONE
#define LIST_DONE

#include <stdexcept>

#ifndef INCLUDED_LIS

#include "List.h"

#endif

template<typename Type>
List<Type>::Node::Node(const Type &key):
        key(new Type(key)), next(nullptr), prev(nullptr) {}

template<typename Type>
void List<Type>::Node::connect(List::Node *to) {
    this->next = to;
    to->prev = this;
}

template<typename Type>
List<Type>::Node::Node() :
    prev(nullptr), next(nullptr),
    key(nullptr) { }

template<typename Type>
List<Type>::Node::~Node() {
    if (key) delete(key);
}

template<typename Type>
List<Type>::List():
        head(nullptr), tail(nullptr),
        endNode(new Node()) {}

template<typename Type>
List<Type>::List(const Type &key) : List() {
    head = tail = new Node(key);
    tail->connect(endNode);
}

template<typename Type>
typename List<Type>::Iterator List<Type>::pushFront(const Type &key) {
    List nl(key);
    nl.connect(*this);
    std::swap(nl.head, head);
    std::swap(nl.tail, tail);
    std::swap(nl.endNode, endNode);
    nl.forget();
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
        tail->connect(endNode);
        to.forget();
    }
    return this;
}

template<typename Type>
List<Type>::~List() {
    for (auto now = begin(); now && now != end(); delete (now++).father) {}
    delete(endNode);
}

template<typename Type>
const Type List<Type>::popFront() {
    check();
    auto *old = head;
    head = head->next;
    auto ret = *(old->key);
    delete old;
    if (head != endNode) head->prev = nullptr;
    else {
        tail = head = nullptr;
        endNode->prev = nullptr;
    }
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
List<Type>::List(const Iterator2 &begin, const Iterator2 &end,
                 typename std::enable_if<std::__is_input_iterator<Iterator2>::value &&
                                         std::is_constructible<
                                                 Type,
                                                 typename std::iterator_traits<Iterator2>::reference>::value>::type *)
        : List() {
    for (auto it = begin; it != end; ++it) {
        pushBack(*it);
    }
}

template<typename Type>
const Type List<Type>::popBack() {
    check();
    auto *old = tail;
    auto ret = *(old->key);
    tail = tail->prev;
    delete old;
    if (tail) {
        tail->connect(endNode);
    } else {
        head = nullptr;
        endNode->prev = nullptr;
    }
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
    return List::Iterator(endNode);
}

template<typename Type>
void List<Type>::forget() {
    head = tail = nullptr;
    endNode->prev = nullptr;
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
    endNode = new Node();
    if (tail) tail->connect(endNode);
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
void List<Type>::clear() {
    for (auto now = begin(); now != end(); delete (now++).father) {}
    head = tail = nullptr;
    endNode->prev = nullptr;
}

template<typename Type>
List<Type> &List<Type>::operator=(List &old) {
    head = old.head;
    tail = old.tail;
    endNode = new Node();
    if (tail) tail->connect(endNode);
    old.forget();
    return *this;
}

template<typename Type>
Type &List<Type>::Iterator::operator*() const {
    return *(father->key);
}

template<typename Type>
Type *List<Type>::Iterator::operator->() const {
    return father->key;
}

template<typename Type>
typename List<Type>::Iterator &List<Type>::Iterator::operator++() {
    father = father->next;
    return *this;
}

template<typename Type>
List<Type>::Iterator::Iterator(typename List<Type>::Node *father): father(father) {}

template<typename Type>
typename List<Type>::Iterator &List<Type>::Iterator::operator--() {
    father = father->prev;
    return *this;
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

#endif