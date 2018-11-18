#pragma once

template<typename Type>
class List {
private:
    class Node {
    private:
        Node *next;
        Node *prev;

    public:

        Type key;

        Node *getNext() const;

        Node *getPrev() const;

        explicit Node(const Type &);

        void connect(Node *);

        void cutFront();

        void cutBack();

    };

    Node *head;
    Node *tail;

    void forget();

public:
    class Iterator {
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
    };

    List();

    explicit List(const Type &);

    Iterator begin() const;
    Iterator end() const;

    template<typename Iterator2>
    List(const Iterator2 &, const Iterator2 &);

    ~List();

    bool empty() const;

    void check() const;

    List<Type> *connect(List &);

    List<Type> *pushFront(const Type &);

    List<Type> *pushBack(const Type &);

    const Type popFront();

    const Type popBack();

    List &operator+=(List &);

    List &operator+=(const Type &);
};

#define INCLUDED_LIST

#include "List.cpp"