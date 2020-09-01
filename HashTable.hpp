#pragma once
#include "LinkedList.hpp"
#include "Array.hpp"
#include <utility>

namespace gdamn::data {

template<typename T, typename U, size_t bucket_count>
class HashTable {
public:
    HashTable() {}
    ~HashTable() {}

    void insert(std::pair<T, U>& key_pair);
    void insert(std::pair<T, U>&& key_pair);

    bool contains(std::pair<T, U>& key_pair);
    bool contains(std::pair<T, U>&& key_pair);

    bool contains(const T& key);
    bool contains(const T&& key);

    U& operator[](const T& key) { /* Could still cause problems? */
        auto& ll = buckets[hash(key)];
        for(auto& i : ll)
            if(auto& [k, v] = i; k == key) return v;
        ll.insert({ key, std::move(U()) });
        return ll.last().second;
    }

    U& operator[](T&& key) {
        auto& ll = buckets[hash(key)];
        for(auto& i : ll)
            if(auto& [k, v] = i; k == key) return v;
        ll.insert({ std::move(key), std::move(U()) });
        return ll.last().second;
    }

    size_t len() { return nodes_num; }

private:
    /* Provide better hash function */
    size_t hash(const T& key) {
        size_t s = 0;
        for(auto c : key) s += (size_t)c;
        return (s % bucket_count);
    }

    size_t nodes_num = 0;
    Array<LinkedList<std::pair<T, U>>, bucket_count> buckets;  
};

template<typename T, typename U, size_t bucket_count>
void HashTable<T, U, bucket_count>::insert(std::pair<T, U>& key) {
    if(buckets[hash(key.first)].contains(key)) return;
    buckets[hash(key.first)].insert(key);
    nodes_num++;
}

template<typename T, typename U, size_t bucket_count>
void HashTable<T, U, bucket_count>::insert(std::pair<T, U>&& key) {
    if(buckets[hash(key.first)].contains(key)) return;
    buckets[hash(key.first)].insert(std::move(key));
    nodes_num++;
}

template<typename T, typename U, size_t bucket_count>
bool HashTable<T, U, bucket_count>::contains(std::pair<T, U>& key) {
    if(buckets[hash(key.first)].contains(key)) return true;
    else return false;
}

template<typename T, typename U, size_t bucket_count>
bool HashTable<T, U, bucket_count>::contains(std::pair<T, U>&& key) {
    if(buckets[hash(key.first)].contains(key)) return true;
    else return false;
}

template<typename T, typename U, size_t bucket_count>
bool HashTable<T, U, bucket_count>::contains(const T& key) {
    auto& ll = buckets[hash(key)];
    for(auto& i : ll)
        if(auto& [k, v] = i; k == key) return true;
    return false;
}

template<typename T, typename U, size_t bucket_count>
bool HashTable<T, U, bucket_count>::contains(const T&& key) {
    auto& ll = buckets[hash(key)];
    for(const auto& i : ll) 
        if(const auto [k, v] = i; k == std::move(key)) return true;
    return false;
}

}