#pragma once
#include <cstdlib>
#include <memory>
#include <limits>

template<typename T, size_t n = 12>
struct Chunk {
    Chunk() { n_left = n; data = new T[n]; }
    ~Chunks() { delete[] data; }

    size_t n_left;
    T* data = nullptr;
};

template<typename T>
class Deque {
public:
    Deque();
    ~Deque();

    class Iterator {

    };

    void insert(T& val);
    void insert(T&& val);
    void insert_front();
    void insert_back();

    bool contains(T& key);
    bool contains(T&& key);
    size_t find(T& key);
    size_t find(T& key);

    T& operator[](size_t i);
    T& first();
    T& last();
    size_t len() { return n_nodes; }
    Iterator begin();
    Iterator end();

    void realign_chunks(size_t n);

private:
    size_t          n_chunks = 0;
    size_t          n_nodes = 0;
    size_t          curr_chunk = 0;
    Chunk<T, 12>*   chunks = nullptr;
};

template<typename T>
Deque<T>::Deque() {
    chunks = new Chunk[1];
    std::memset(chunks[0].data, 0, sizeof(chunks[0].data));
}

template<typename T>
Deque<T>::~Deque() {
    delete[] chunks;
}

template<typename T>
void Deque<T>::insert(T& val) {
    // Make sure that enough reserve is left
    if(chunks[curr_chunk].n_left == 0) realign_chunks(1);
    size_t index = 12 - chunks[curr_chunk].n_left;
    chunks[curr_chunk].n_left--;
    chunks[curr_chunk].data[index] = val;
}

template<typename T>
void Deque<T>::insert(T&& val) {
    if(chunks[curr_chunk].n_left == 0) realign_chunks(1);
    size_t index = 12 - chunks[curr_chunk].n_left;
    chunks[curr_chunk].n_left--;
    chunks[curr_chunk].data[index] = std::move(val);
}

template<typename T>
size_t Deque<T>::find(T& key) {
    
}