#pragma once
#include <cstdlib>
#include <initializer_list>
#include <limits>
#include <cstring>
#include <functional>
#include "Enumerable.hpp"

namespace gdamn::data {

template<typename T, size_t n> 
class Array {
public:

    Array() {}
    ~Array() {}
    Array(std::initializer_list<T> list);
    
    Array<T, n>& operator=(Array<T, n>& other);
    Array<T, n>& operator=(Array<T, n>&& other);

    void for_each(std::function<void(T&)> call_back);
    auto where(std::function<bool(const T&)> match_func);

    bool contains(T& key);
    bool contains(T&& key);

    size_t find(T& key);
    size_t find(T&& key);

    class Iterator {
    public:
        Iterator() {}
        
        Iterator(Iterator& itr) {
            this->beg = itr.beg;
            this->index = itr.index;
        }

        Iterator(Iterator&& itr) {
            this->beg = itr.beg;
            this->index = itr.index;
        }

        Iterator& operator=(Iterator& other) {
            this->beg = other.beg;
            this->index = other.index;
            return *this;
        }

        Iterator& operator=(Iterator&& other) {
            this->beg = other.beg;
            this->index = other.index;
            other.beg = nullptr;
            other.index = 0;
            return *this;
        }

        Iterator& operator++() {
            index += index < n;
            return *this; 
        }
        
        Iterator& operator++(int) { 
            index += index < n;
            return *this; 
        }

        Iterator& operator+=(size_t i) { 
            index += i * (index + i < n);
            return *this; 
        }
        Iterator& operator-=(size_t i) { 
            index -= i * (index - i < 0);
            return *this; 
        }

        Iterator& operator--() { 
            index -= index > 0;
            return *this; 
        }

        Iterator& operator--(int) { 
            index -= index > 0;
            return *this; 
        }

        bool operator==(const Iterator other) const { 
            return (this->index == other.index && this->beg == other.beg);
        }

        bool operator!=(const Iterator other) const {
            return !(this->index == other.index && this->beg == other.beg);
        }

        T& operator*() {
            return beg[index]; 
        }
    private:
        Iterator(T* beg, size_t i) { this->beg = beg; index = i; }
        friend Array<T, n>;
        T* beg = nullptr;
        size_t index = 0;
    };

    constexpr size_t len()     { return n; }
    T& operator[](size_t i)    { return data[i]; }
    Iterator begin()           { return Iterator(data, 0); }
    Iterator end()             { return Iterator(data, len()); };
    T* first()                 { return data; }
private:
    T data[n];
};

template<typename T, size_t n>
Array<T, n>::Array(std::initializer_list<T> list) {
    size_t i = 0;
    for(auto j : list) {
        data[i] = j;
        i++;
    }
}


template<typename T, size_t n>
Array<T, n>& Array<T, n>::operator=(Array<T, n>& other) {
    std::memcpy(data, other.data, sizeof(T) * n);
    return *this;
}

template<typename T, size_t n>
Array<T, n>& Array<T, n>::operator=(Array<T, n>&& other) {
    this->data = other.data;
    other.data = nullptr;
    return *this;
}

template<typename T, size_t n>
void Array<T, n>::for_each(std::function<void(T&)> call_back) {
    for(auto& val : *this)
        call_back(val);
}

template<typename T, size_t n>
auto Array<T, n>::where(std::function<bool(const T&)> match_func) {
    Enumerable<T> enumerable;
    for(size_t i = 0; i < len(); i++)
        if(match_func(data[i])) enumerable.insert(data[i]);
    return enumerable;
}

template<typename T, size_t n>
bool Array<T, n>::contains(T& key) {
    return (find(key) != std::numeric_limits<size_t>::max());  
}

template<typename T, size_t n>
bool Array<T, n>::contains(T&& key) {
    return (find(key) != std::numeric_limits<size_t>::max());  
}

template<typename T, size_t n>
size_t Array<T, n>::find(T& key) {
    for(size_t i = 0; i < n; i++) if(data[i] == key) return i;
    return std::numeric_limits<size_t>::max();
}

template<typename T, size_t n>
size_t Array<T, n>::find(T&& key) {
    for(size_t i = 0; i < n; i++) if(data[i] == key) return i;
    return std::numeric_limits<size_t>::max();
}

}