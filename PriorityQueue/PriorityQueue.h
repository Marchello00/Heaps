#pragma once
#include <Array.h>

template<typename Type>
class PriorityQueue {
public:
    class Pointer;
protected:
    class Data {
        friend class Pointer;
        friend class PriorityQueue<Type>;
    protected:
        PriorityQueue *father;

        Type key;
    public:
        Type get() const;

        virtual Data &operator=(const Type &);

        Data(PriorityQueue &, const Type &);

        explicit Data(const Type &);

        virtual ~Data()= default;
    };

public:
    class Pointer {
    protected:
        Data * object;
        PriorityQueue<Type> *father;
    public:

        virtual Data &operator*() const;
        virtual Data *operator->() const;
        explicit Pointer(Data * = nullptr, PriorityQueue<Type> * = nullptr);
    };

    virtual const bool empty() const=0;

    virtual const Type getMin() const=0;

    virtual const Type extractMin()=0;

    virtual const Pointer insert(const Type &)=0;

    virtual void erase(const Pointer &)=0;

    virtual const unsigned size() const=0;

    virtual void change(const Pointer &, const Type &);

    virtual ~PriorityQueue()= default;

protected:
    virtual void siftUp(Pointer)=0;
    virtual void siftDown(Pointer)=0;
    virtual void check() const;

};

template<typename Type>
std::ostream &operator<<(std::ostream &, const typename PriorityQueue<Type>::Data &);

#define INCLUDED_PQ

#include "PriorityQueue.cpp"