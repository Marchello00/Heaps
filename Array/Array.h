#pragma once

#include <iterator>

template<typename Type>
class Array {
public:
    typedef Type type;

    explicit Array(unsigned = 0);

    Array(unsigned, const Type &);

    template<typename Iterator2>
    Array(Iterator2, Iterator2, typename std::enable_if<std::__is_input_iterator  <Iterator2>::value &&
            std::is_constructible<
                    Type,
                    typename std::iterator_traits<Iterator2>::reference>::value>::type* = 0);

    Type &operator[](int index);

    const Type operator[](int index) const;

    const bool empty() const;                   // check array is empty or not

    const unsigned size() const;                // get size_ of array

    const unsigned capacity() const;            // get number of allocated places for elements

    void resize(unsigned);                      // change size_ of array
    void resize(unsigned, const Type &);        // change size_ of array & initialize new elements as given

    void clear();

    Array(const Array<Type> &);

    Array<Type> &operator=(const Array<Type> &);

    ~Array();

    class Iterator : public std::iterator<std::input_iterator_tag, Type> {
    public:
        Type &operator*();

        Type *operator->();

        bool operator==(const Iterator &) const;

        bool operator!=(const Iterator &) const;

        Iterator &operator++();

        Iterator &operator--();

        const Iterator operator++(int);

        const Iterator operator--(int);

        Iterator &operator+=(int);

        Iterator &operator-=(int);

        Iterator operator+(int) const;

        Iterator operator-(int) const;

        Iterator(Array<Type> *father, unsigned index);

    private:
        int index;
        Array<Type> *father;
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
    int constructed;

    Type *arr;                                  // container

    void reallocate(unsigned);
};

#define INCLUDED_ARRAY

#include "Array.cpp"