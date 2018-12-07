#pragma once

#include <iterator>

template<typename Type>
class List {
private:
    class Node {
        friend class List<Type>;

        Node *next;
        Node *prev;

        Node();

        Type *key;

        explicit Node(const Type &);

        void connect(Node *);

        ~Node();

    };

    Node *head;
    Node *tail;

    Node *endNode;

public:
    class Iterator : public std::iterator<std::input_iterator_tag, Type> {
        friend List;
    private:
        Node *father;
    public:
        Type &operator*() const;

        Type *operator->() const;

        explicit Iterator(Node *);

        bool operator==(const Iterator &) const;

        bool operator!=(const Iterator &) const;

        Iterator &operator++();

        const Iterator operator++(int);

        Iterator &operator--();

        const Iterator operator--(int);

        explicit operator bool() const;

        bool operator!() const;
    };

    List();

    explicit List(const Type &);

    List(List &);

    List &operator=(List &);            // ATTENTION!!! right List isn't rvalue, it'll be changed(cleared) for O(1) complexity

    Iterator begin() const;

    Iterator end() const;

    template<typename Iterator2>
    List(const Iterator2 &, const Iterator2 &,
         typename std::enable_if<std::is_constructible<
                                         Type,
                                         typename std::iterator_traits<Iterator2>::reference>::value>::type* = 0);

    ~List();

    bool empty() const;

    void clear();

    Iterator pushFront(const Type &);

    Iterator pushBack(const Type &);

    void erase(Iterator);

    const Type popFront();

    const Type popBack();

    List &operator+=(List &);

    List &operator+=(const Type &);


private:

    void forget();

    void check() const;

    List<Type> *connect(List &);
};

#define INCLUDED_LIST

#include "List.cpp"