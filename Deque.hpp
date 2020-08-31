#pragma once
#include <cstdlib>
#include <memory>
#include <limits>

template<typename T, size_t n>
class Deque;

template<typename T, size_t n = 12>
struct Chunk {
    Chunk() { n_left = n; data = new T[n]; }
    ~Chunk() { delete[] data; }
private:
    friend Deque<T, n>;
    size_t n_left = n;
    T* data = nullptr;
};

/* Constant insert_front && insert_back not implemented yet; Iterator too */
template<typename T, size_t n = 12>
class Deque {
public:
    Deque();
    ~Deque();

    /* Implement */
    class Iterator {

    };

    void insert(T& val);
    void insert(T&& val);
    void insert_front(T& val);
    void insert_front(T&& val);
    void insert_back(T& val);
    void insert_back(T&& val);

    bool contains(T& key);
    bool contains(T&& key);
    size_t find(T& key);
    size_t find(T&& key);

    T& operator[](size_t i) {
        return chunks[i % n_chunks]->data[i % n];
    }

    T& first() { return chunks[0][0]; }
    T& last();
    size_t len() { return n_nodes; }
    Iterator begin();
    Iterator end();

    void realign_chunks(size_t num);

private:
    size_t          n_chunks = 0;
    size_t          n_nodes = 0;
    size_t          curr_chunk = 0;
    Chunk<T, n>**   chunks = nullptr;
};

template<typename T, size_t n>
Deque<T, n>::Deque() {
    chunks = new Chunk<T, n>*();
    chunks[0] = new Chunk<T, n>();
    std::memset(chunks[0]->data, 0, sizeof(chunks[0]->data));
    n_chunks++;
}

template<typename T, size_t n>
Deque<T, n>::~Deque() {
    for(size_t i = 0; i < n_chunks; i++) {
        delete[] chunks[i];
    }
}

template<typename T, size_t n>
void Deque<T, n>::insert(T& val) {
    // Make sure that enough reserve is left
    if(chunks[curr_chunk]->n_left == 0) realign_chunks(1);
    size_t index = n - chunks[curr_chunk]->n_left;
    chunks[curr_chunk]->n_left--;
    chunks[curr_chunk]->data[index] = val;
    n_nodes++;
}

template<typename T, size_t n>
void Deque<T, n>::insert(T&& val) {
    if(chunks[curr_chunk]->n_left == 0) realign_chunks(1);
    size_t index = n - chunks[curr_chunk]->n_left;
    chunks[curr_chunk]->n_left--;
    chunks[curr_chunk]->data[index] = std::move(val);
    n_nodes++;
}

template<typename T, size_t n>
void Deque<T, n>::insert_back(T& val) {
    
}

template<typename T, size_t n>
void Deque<T, n>::insert_back(T&& val) {
    
}

template<typename T, size_t n>
void Deque<T, n>::insert_front(T& val) {

}

template<typename T, size_t n>
void Deque<T, n>::insert_front(T&& val) {

}

template<typename T, size_t n>
bool Deque<T, n>::contains(T& key) {
    return find(key) != std::numeric_limits<size_t>::max();
}

template<typename T, size_t n>
bool Deque<T, n>::contains(T&& key) {
    return find(key) != std::numeric_limits<size_t>::max();
}

template<typename T, size_t n>
size_t Deque<T, n>::find(T& key) {
    for(size_t i = 0; i < len(); i++)
        if((*this)[i] == key) return i;
    return std::numeric_limits<size_t>::max();
}

template<typename T, size_t n>
size_t Deque<T, n>::find(T&& key) {
    for(size_t i = 0; i < len(); i++)
        if((*this)[i] == key) return i;
    return std::numeric_limits<size_t>::max();
}

template<typename T, size_t n>
void Deque<T, n>::realign_chunks(size_t num) {
    auto new_chunks = new Chunk<T, n>*[n_chunks + num];
    std::memcpy(new_chunks, chunks, sizeof(chunks));
    chunks = new_chunks;
}