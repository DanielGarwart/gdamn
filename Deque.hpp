#pragma once
#include <cstdlib>
#include <cstdint>
#include <memory>
#include <limits>
#include <functional>

namespace gdamn::data {

template<typename T, size_t n>
class Deque;

namespace {

template<typename T, size_t n = 12>
struct Chunk {
    Chunk()  { n_left = n; }
    ~Chunk() { delete[] data; }
private:
    friend Deque<T, n>;
    size_t n_left = n;
    T* data = nullptr;
};

}

template<typename T, size_t n = 12>
class Deque {
public:
    Deque();
    ~Deque();

    /* Make use of chunk caching friendliness*/
    class Iterator { 
    public:
        void operator++() {
            index++;
        }

        void operator++(int) {
            index++;
        }

        void operator--() {
            index--;
        }

        void operator--(int) {
            index--;
        }

        bool operator==(Iterator other) {
            return this->ref == other.ref && this->index == other.index;
        }

        bool operator!=(Iterator other) {
            return !(*this == other);
        }

        T& operator*() {
            return (*ref)[index];
        }

    private:
        Iterator(Deque<T, n>* ref, size_t index = 0) { 
            this->index = index;
            this->ref = ref; 
        }
        Deque<T, n>* ref = nullptr;
        size_t index = 0;
        friend Deque<T, n>;
    };

    void insert(T& val);
    void insert(T&& val);
    void insert_front(T& val);
    void insert_front(T&& val);
    void insert_back(T& val);
    void insert_back(T&& val);

    /* Costly operation !!! */
    void remove(T& val);
    void remove(T&& val);
    void remove_all(T& val);
    void remove_all(T&& val);
    void remove(Iterator where);

    void for_each(std::function<void(T&)> call_back);

    bool contains(T& key);
    bool contains(T&& key);
    size_t find(T& key);
    size_t find(T&& key);

    /* Doesn't account for front chunk yet */
    T& operator[](size_t i) { /* equation needs improvement */
        i += chunks[0]->n_left;
        return chunks[i / n]->data[i % n];
    }

    T& first();
    T& last();
    const size_t len()  { return n_nodes; }
    Iterator begin()    { return Iterator(this); }
    Iterator end()      { return Iterator(this, len()); }

    void realign_chunks(const size_t num) {
        auto new_chunks = new Chunk<T, n>*[n_chunks + num]; /* Fix the memory leak it'll cause */
        std::memcpy(new_chunks, chunks, n_chunks * sizeof(Chunk<T, n>*)); /* Prev upper bound */
        for(size_t i = 0; i < num; i++) {
            new_chunks[n_chunks + i] = new Chunk<T, n>();
            new_chunks[n_chunks + i]->data = new T[n];
        }
        operator delete[](chunks);
        chunks = new_chunks;
        n_chunks += num;
    }

private:
    size_t          n_chunks = 0;
    size_t          n_nodes = 0;
    size_t          curr_chunk = 1;
    Chunk<T, n>**   chunks = nullptr;
};

template<typename T, size_t n>
Deque<T, n>::Deque() {
    chunks = new Chunk<T, n>*[2]; /* Front chunk and first back chunk */
    chunks[0] = new Chunk<T, n>();
    chunks[0]->data = new T[n];
    std::memset(chunks[0]->data, 0, sizeof(T) * n);
    chunks[1] = new Chunk<T, n>();
    chunks[1]->data = new T[n];
    std::memset(chunks[1]->data, 0, sizeof(T) * n);
    n_chunks += 2;
}

template<typename T, size_t n>
Deque<T, n>::~Deque() {
    for(size_t i = 0; i < n_chunks; i++) delete chunks[i];
    delete[] chunks;
}

template<typename T, size_t n>
void Deque<T, n>::insert(T& val) {
    // Make sure that enough reserve is left
    if(chunks[curr_chunk]->n_left == 0)
    {
        realign_chunks(1);
        curr_chunk++;
    }
    size_t index = n - chunks[curr_chunk]->n_left;
    chunks[curr_chunk]->n_left--;
    chunks[curr_chunk]->data[index] = val;
    n_nodes++;
}

template<typename T, size_t n>
void Deque<T, n>::insert(T&& val) {
    if(chunks[curr_chunk]->n_left == 0) 
    {
        realign_chunks(1);
        curr_chunk++;
    }
    size_t index = n - chunks[curr_chunk]->n_left;
    chunks[curr_chunk]->n_left--;
    chunks[curr_chunk]->data[index] = std::move(val);
    n_nodes++;
}

template<typename T, size_t n>
void Deque<T, n>::insert_back(T& val) {
    insert(val);
}

template<typename T, size_t n>
void Deque<T, n>::insert_back(T&& val) {
    insert(val);
}

template<typename T, size_t n>
void Deque<T, n>::insert_front(T& val) {
    if(chunks[0]->n_left == 0)
    {
        /* Allocate new front-chunk and make current part of actual chunk-chain */
        auto new_chunk = new Chunk<T, n>*[n_chunks + 1];
        std::memcpy(new_chunk + 1, chunks, sizeof(Chunk<T, n>*) * n_chunks);
        operator delete[](chunks);
        chunks = new_chunk;
        chunks[0] = new Chunk<T, n>();
        chunks[0]->data = new T[n];
        n_chunks++;
    }

    chunks[0]->n_left--;
    chunks[0]->data[chunks[0]->n_left] = val;
    n_nodes++;
}

template<typename T, size_t n>
void Deque<T, n>::insert_front(T&& val) {
    if(chunks[0]->n_left == 0)
    {
        /* Allocate new front-chunk and make current part of actual chunk-chain */
        auto new_chunk = new Chunk<T, n>*[n_chunks + 1];
        std::memcpy(new_chunk + 1, chunks, sizeof(Chunk<T, n>) * n_chunks);
        operator delete(chunks);
        chunks = new_chunk;
        chunks[0] = new Chunk<T, n>();
        chunks[0]->data = new T[n];
    }

    chunks[0]->n_left--;
    chunks[0]->data[chunks[0]->n_left] = std::move(val);
    n_nodes++;
}

template<typename T, size_t n>
void Deque<T, n>::remove(T& key) {
    size_t index = find(key);
    if(index == std::numeric_limits<size_t>::max()) return; /* Key doesn't exist in chunks */

    /* TODO: copy per chunk && std::memcpy */
    auto temp = new T[len() - (index + 1)]; /* Copy everything into temporary storage */
    size_t j = 0;
    for(size_t i = index + 1, j = 0; i < len(); i++, j++)
        temp[j] = (*this)[i];
    for(size_t i = index, size_t j = 0; j < (len() - (index + 1)); i++, j++)
        (*this)[i] = temp[j];
    n_nodes--;
    chunks[n_chunks - 1]->n_left++;
    operator delete[](temp);
}

template<typename T, size_t n>
void Deque<T, n>::remove(T&& key) {
    size_t index = find(key);
    if(index == std::numeric_limits<size_t>::max()) return; /* Key doesn't exist in chunks */

    /* TODO: copy per chunk && std::memcpy */
    auto temp = new T[len() - (index + 1)]; /* Copy everything into temporary storage */
    size_t j = 0;
    for(size_t i = index + 1; i < len(); i++, j++)
        temp[j] = (*this)[i];
    for(size_t i = index, j = 0; j < (len() - (index + 1)); i++, j++) {
        (*this)[i] = temp[j]; 
    }
    n_nodes--;
    chunks[n_chunks - 1]->n_left++; /* TODO: Consider by realign_chunk pre-allocated chunks */
    operator delete[](temp);
}

template<typename T, size_t n>
void Deque<T, n>::remove(Iterator key) {
    size_t index = key.index; /* Key doesn't exist in chunks */

    /* TODO: copy per chunk && std::memcpy */
    auto temp = new T[len() - (index + 1)]; /* Copy everything into temporary storage */
    for(size_t i = index + 1, size_t j = 0; i < len(); i++, j++)
        temp[j] = (*this)[i];
    for(size_t i = index, size_t j = 0; j < (len() - (index + 1)); i++, j++) {
        (*this)[i] = temp[j]; 
    }
    n_nodes--;
    chunks[n_chunks - 1]->n_left++; /* TODO: Consider by realign_chunk pre-allocated chunks */
    operator delete[](temp);
}

template<typename T, size_t n>
void Deque<T, n>::remove_all(T& key) {

}

template<typename T, size_t n>
void Deque<T, n>::remove_all(T&& key) {
    
}

template<typename T, size_t n>
void Deque<T, n>::for_each(std::function<void(T&)> call_back) {
    for(size_t i = 0; i < len(); i++)
        call_back((*this)[i]);
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
    return std::numeric_limits<size_t>::max(); /* key not found so max size_t value is returned as indicator */
}

template<typename T, size_t n>
size_t Deque<T, n>::find(T&& key) {
    for(size_t i = 0; i < len(); i++)
        if((*this)[i] == key) return i;
    return std::numeric_limits<size_t>::max();
}

template<typename T, size_t n>
T& Deque<T, n>::first() {
    return (*this)[0];
}

template<typename T, size_t n>
T& Deque<T, n>::last() {
    return (*this)[n_nodes - 1];
}

}