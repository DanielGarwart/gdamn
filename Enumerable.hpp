#pragma once
#include "LinkedList.hpp"
#include "Vector.hpp"

namespace gdamn::data {

/* Implementation relies on LinkedList<T> for now */
template<typename T>
class Enumerable {
public:
    Enumerable() {}
    ~Enumerable() {}

    class Iterator {
    public:
        Iterator() { }

        Iterator(Iterator& itr) {
            this->internal_itr = itr.internal_itr;
        }

        Iterator(Iterator&& itr) {
            this->internal_itr = std::move(itr.internal_itr);
        }

        Iterator& operator=(Iterator& other) {
            this->internal_itr = other.internal_itr;
            return *this;
        }

        Iterator& operator=(Iterator&& other) {
            this->internal_itr = std::move(other.internal_itr);
            return *this;
        }

        Iterator& operator++(int) {
            internal_itr++;
            return *this;
        }

        Iterator& operator++() {
            internal_itr++;
            return *this;
        }

        Iterator& operator--(int) {
            internal_itr--;
            return *this;
        }

        Iterator& operator--() {
            internal_itr--;
            return *this;
        }

        Iterator& operator+=(size_t n) {
            internal_itr += n;
        }

        Iterator& operator-=(size_t n) {
            internal_itr -= n;
        }

        T& operator*() {
            return *internal_itr;
        }

        bool operator==(const Iterator other) const {
            return this->internal_itr == other.internal_itr;
        }

        bool operator!=(const Iterator other) const {
            return !(this->internal_itr == other.internal_itr);
        }

    private:
        Iterator(typename LinkedList<T>::Iterator itr) {
            this->internal_itr = itr;
        }
        typename LinkedList<T>::Iterator internal_itr;
        friend Enumerable<T>;
    };

    LinkedList<T> to_list() {
        LinkedList<T> ll;
        for(auto& x : internal) ll.insert(x);
        return ll;
    }

    Vector<T> to_vector() {
        Vector<T> vector;
        vector.realign(internal.len());
        for(auto& x : internal) vector.insert(x);
        return vector;
    }

    Enumerable<T> where(std::function<bool(const T&)> match_func) {
        Enumerable<T> enumerable;
        for(auto& x : internal) {
            if(match_func(x)) enumerable.insert(x);
        }
        return enumerable;
    }

    T& first() {
        return internal.first();
    }

    T& last() {
        return internal.last();
    }

    void insert(T& x) {
        internal.insert(x);
    }

    void insert(T&& x) {
        internal.insert(x);
    }

    inline size_t len() const { return internal.len(); }
    
    inline Iterator begin() {
        return Iterator(internal.begin());
    }

    inline Enumerable<T>::Iterator end() {
        return Iterator(internal.end());
    }

private:
    LinkedList<T> internal;
};

}