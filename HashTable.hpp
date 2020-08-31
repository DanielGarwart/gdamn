#pragma once
#include "LinkedList.hpp"
#include "Array.hpp"

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

    U& operator[](T& key) {
        auto& ll = buckets[hash(key)];

        for(auto& i : ll)
        {
            auto& [k, v] = i;
            if(k == key) return v;
        }

        ll.insert({ key, U() });
        return ll.last().second;
    }

    U& operator[](T&& key) {
        auto& ll = buckets[hash(key)];

        for(auto& i : ll)
        {
            auto& [k, v] = i;
            if(k == key) return v;
        }

        ll.insert({ key, U() });
        return ll.last().second;
    }

    const size_t len() { return nodes_num; }

private:

    size_t hash(T& key) {
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
    buckets[hash(key.first)].insert(key);
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

}