#pragma once
#include "Enumerable.hpp"
#include "Array.hpp"
#include <utility>

namespace gdamn::data {

template<typename T, typename U, size_t bucket_count>
class HashTable {
public:
    HashTable() {}
    ~HashTable() {}

    HashTable(const HashTable&) = delete;
    HashTable(const HashTable&&) = delete;
    void insert(std::pair<T, U>& key_pair);
    void insert(std::pair<T, U>&& key_pair);

    bool contains(const std::pair<T, U>& key_pair);
    bool contains(const std::pair<T, U>&& key_pair);

    bool contains(const T& key);
    bool contains(const T&& key);

    void remove(const T& key);
    void remove(const T&& key);
    void remove_all(const T& key);
    void remove_all(const T&& key);
    void remove_all(const std::pair<T, U>& key_pair);
    void remove_all(const std::pair<T, U>&& key_pair);
    void remove_if(std::function<bool(const T&, const U&)>);

    void for_each(std::function<void(T&, U&)> call_back);
    
    /* Returns the keys that matched the provided function */
    Enumerable<T> where(std::function<bool(const T&, const U&)> match_func);
    /* Returns the values that matched the provided function */
    Enumerable<U> where_val(std::function<bool(const T&, const U&)> match_func);
    /* Returns pair of keys & values that matched the provided function  */
    Enumerable<std::pair<T, U>> where_pair(std::function<bool(const T&, const U&)> match_func);

    U& operator[](const T& key) {
        auto& ll = buckets[hash(key)];
        for(auto& i : ll)
            if(auto& [k, v] = i; k == key) return v;
        ll.insert({ key, std::move(U()) });
        nodes_num++;
        return ll.last().second;
    }

    U& operator[](T&& key) {
        auto& ll = buckets[hash(key)];
        for(auto& i : ll)
            if(auto& [k, v] = i; k == key) return v;
        ll.insert({ std::move(key), std::move(U()) });
        nodes_num++;
        return ll.last().second;
    }

    size_t len() const { return nodes_num; }

private:

    size_t hash(const T& key) const {
        return std::hash<T>{}(key) % bucket_count;
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
bool HashTable<T, U, bucket_count>::contains(const std::pair<T, U>& key) {
    return buckets[hash(key.first)].contains(key);
}

template<typename T, typename U, size_t bucket_count>
bool HashTable<T, U, bucket_count>::contains(const std::pair<T, U>&& key) {
    return buckets[hash(key.first)].contains(key);
}

template<typename T, typename U, size_t bucket_count>
bool HashTable<T, U, bucket_count>::contains(const T& key) {
    for(const auto& i : buckets[hash(key)])
        if(const auto& [k, v] = i; k == key) return true;
    return false;
}

template<typename T, typename U, size_t bucket_count>
bool HashTable<T, U, bucket_count>::contains(const T&& key) {
    for(const auto& i : buckets[hash(key)]) 
        if(const auto& [k, v] = i; k == std::move(key)) return true;
    return false;
}

template<typename T, typename U, size_t bucket_count>
void HashTable<T, U, bucket_count>::remove(const T& key) {
    auto& ll = buckets[hash(key)];
    bool was_found = false;
    auto i = ll.begin();
    for(; i != ll.end() && !was_found; i++) {
        const auto& [k, v] = *i;
        was_found = k == key;
    }
        
    if(was_found) {
        nodes_num--;
        ll.remove(i);
    }
}

template<typename T, typename U, size_t bucket_count>
void HashTable<T, U, bucket_count>::remove(const T&& key) {
    LinkedList<std::pair<T, U>>& ll = buckets[hash(key)];
    bool was_found = false;
    auto i = ll.begin();
    for(; i != ll.end() && !was_found; i++) {
        const auto& [k, v] = *i;
        was_found = k == std::move(key);
    }
        
    if(was_found) {
        nodes_num--;
        ll.remove(i);
    }
}

template<typename T, typename U, size_t bucket_count>
void HashTable<T, U, bucket_count>::remove_all(const T& key) {
    LinkedList<std::pair<T, U>>& ll = buckets[hash(key)];
    auto i = ll.begin();
    for(auto j = ll.begin(); i != ll.end();) {
        if(const auto& [k, v] = *i; k == key) {
            j = i;
            j++;
            auto where = i;
            ll.remove(where);
            i = j;
            nodes_num--;
        } else {
            i++;
        }
    }
}

template<typename T, typename U, size_t bucket_count>
void HashTable<T, U, bucket_count>::remove_all(const T&& key) {
    LinkedList<std::pair<T, U>>& ll = buckets[hash(key)];
    auto i = ll.begin();
    
    for(auto j = ll.begin(); i != ll.end();) {
        if(const auto& [k, v] = *i; k == std::move(key)) {
            j = i;
            j++;
            auto where = i;
            ll.remove(where);
            i = j;
            nodes_num--;
        } else {
            i++;
        }
    }
}

template<typename T, typename U, size_t bucket_count>
void HashTable<T, U, bucket_count>::remove_all(const std::pair<T, U>& key) {
    LinkedList<std::pair<T, U>>& ll = buckets[hash(key)];
    auto i = ll.begin();

    for(auto j = ll.begin(); i != ll.end();) {
        if(*i == key) {
            j = i;
            j++;
            auto where = i;
            ll.remove(where);
            i = j;
            nodes_num--;
        } else {
            i++;
        }
    }
}

template<typename T, typename U, size_t bucket_count>
void HashTable<T, U, bucket_count>::remove_all(const std::pair<T, U>&& key) {
    LinkedList<std::pair<T, U>>& ll = buckets[hash(key)];
    auto i = ll.begin();

    for(auto j = ll.begin(); i != ll.end();) {
        if(*i == std::move(key)) {
            j = i;
            j++;
            auto where = i;
            ll.remove(where);
            i = j;
            nodes_num--;
        } else {
            i++;
        }
    }
}

template<typename T, typename U, size_t bucket_count>
void HashTable<T, U, bucket_count>::for_each(std::function<void(T&, U&)> call_back) {
    for(auto& bucket_list : buckets) {
        for(auto& pair : bucket_list) {
            auto& [k, v] = pair;
            call_back(k, v);
        }
    }
}

template<typename T, typename U, size_t bucket_count>
Enumerable<T> HashTable<T, U, bucket_count>::where(std::function<bool(const T&, const U&)> match_func) {
    Enumerable<T> enumerable;
    for(auto& bucket_list : buckets) {
        for(auto& [k, v] : bucket_list) {
            if(match_func(k, v)) enumerable.insert(k); 
        }
    }
    return enumerable;
}

template<typename T, typename U, size_t bucket_count>
Enumerable<U> HashTable<T, U, bucket_count>::where_val(std::function<bool(const T&, const U&)> match_func) {
    Enumerable<U> enumerable;
    for(auto& bucket_list : buckets) {
        for(auto& [k, v] : bucket_list) {
            if(match_func(k, v)) enumerable.insert(v);
        }
    }
    return enumerable;
}

template<typename T, typename U, size_t bucket_count>
Enumerable<std::pair<T, U>> HashTable<T, U, bucket_count>::where_pair(std::function<bool(const T&, const U&)> match_func) {
    Enumerable<std::pair<T, U>> enumerable;
    for(auto& bucket_list : buckets) {
        for(auto& pair : bucket_list) {
            auto& [k, v] = pair;
            if(match_func(k, v)) enumerable.insert(pair);
        }
    }
    return enumerable;
}

template<typename T, typename U, size_t bucket_count>
void HashTable<T, U, bucket_count>::remove_if(std::function<bool(const T&, const U&)> call_back) {    
    for(auto& bucket_list : buckets) {
        auto i = bucket_list.begin();
        for(auto j = bucket_list.begin(); i != bucket_list.end();) {
            const auto& [k, v] = *i;
            if(call_back(k, v)) {
                j = i;
                j++;
                auto where = i;
                bucket_list.remove(where);
                i = j;
                nodes_num--;
            } else {
                i++;
            }
        }
    }
}

}