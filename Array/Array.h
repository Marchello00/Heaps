#pragma once

template<typename Type>
class Array {
public:
    typedef Type type;

    explicit Array(unsigned = 0);

    Array(unsigned, const Type &);

    template<typename Iterator2>
    Array(Iterator2, Iterator2);

    Type &operator[](int index);
    const Type operator[](int index) const;

    const bool empty() const;                   // check array is empty or not

    const unsigned size() const;                // get size_ of array

    const unsigned capacity() const;            // get number of allocated places for elements

    void resize(unsigned);                    // change size_ of array

    void clear();

    Array(const Array<Type> &);

    Array<Type> &operator=(const Array<Type> &);

    ~Array();

    void forceModeOn();                         // turn force mode on
    void forceModeOff();                        // turn force mode off

    class Iterator {
    public:
        Type &operator*();

        bool operator==(const Iterator &) const;
        bool operator!=(const Iterator &) const;

        Iterator &operator++();

        Iterator &operator--();

        const Iterator operator++(int);

        const Iterator operator--(int);

        Iterator &operator+=(unsigned);

        Iterator &operator-=(unsigned);

        Iterator operator+(unsigned) const;

        Iterator operator-(unsigned) const;

        Iterator(Array<Type> *father, unsigned index);

    private:
        unsigned index;
        Array<Type> * father;
    };

    Iterator getIterator(unsigned);

    Iterator begin();

    Iterator end();

    Iterator push(const Type &element);

    void pop();

private:
    unsigned size_;
    unsigned capacity_;
    unsigned alpha = 2, beta = 4;               // Alpha, Beta - constants for reallocation

    bool force = false;                         // if in force mode the destructors of the elements will not be invoked

    Type *arr;                                  // container

    void reallocate(unsigned);
};

#define INCLUDED_ARRAY

#include "Array.cpp"