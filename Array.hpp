#pragma once
#include <cstdlib>
#include <initializer_list>
#include <limits>
#include <cstring>

namespace gdamn::data {

template<typename T, size_t n> 
class Array {
public:

    Array() {}
    ~Array() {}
    Array(std::initializer_list<T> list);
    
    Array<T, n>& operator=(Array<T, n>& other);
    Array<T, n>& operator=(Array<T, n>&& other);

    bool contains(T& key);
    bool contains(T&& key);

    size_t find(T& key);
    size_t find(T&& key);

    class Iterator {
    public:
        void operator++()               { index++; }
        void operator++(int)            { index++; }
        void operator+=(size_t i)       { index += i; }
        void operator-=(size_t i)       { index -= i; }
        void operator--()               { index--; }
        void operator--(int)            { index--; }

        bool operator==(Iterator other) { 
            return (this->index == other.index && this->beg == other.beg);
        }

        bool operator!=(Iterator other) {
            return !(*this == other);
        }

        T& operator*() 
        {  
            return beg[index]; 
        }
    private:
        Iterator(T* beg) { this->beg = beg; }
        T* beg;
        size_t index = 0;
    };

    constexpr inline size_t len()       { return n; }
    inline T& operator[](size_t i)      { return data[i]; }
    inline Iterator begin()             { return Iterator(data); }
    inline Iterator end()               { return Iterator(nullptr); };
    inline T* first()                   { return data; }
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