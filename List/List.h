#pragma once

template<typename Type>
class List {
private:
    class Node {
        friend class List<Type>;

    private:
        Node *next;
        Node *prev;

    public:

        Type key;

        explicit Node(const Type &);

        void connect(Node *);

        void cutFront();

        void cutBack();

    };

    Node *head;
    Node *tail;

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

        explicit operator bool() const;

        bool operator!() const;
    };

    List();

    explicit List(const Type &);

    List(List &);

    Iterator begin() const;

    Iterator end() const;

    template<typename Iterator2>
    List(const Iterator2 &, const Iterator2 &);

    ~List();

    bool empty() const;

    void check() const;

    List<Type> *connect(List &);

    Iterator pushFront(const Type &);

    Iterator pushBack(const Type &);

    void erase(Iterator);

    const Type popFront();

    const Type popBack();

    List &operator+=(List &);

    List &operator+=(const Type &);

    void forget();
};

#define INCLUDED_LIST

#include "List.cpp"