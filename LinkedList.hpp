#pragma once
#include <memory>
#include <limits>
#include <cstdio>

namespace gdamn::data {

template<typename T>
struct Leaf {
    Leaf<T>* next = nullptr;
    Leaf<T>* prev = nullptr;

    T data;
};

template<typename T, typename alloc = std::allocator<Leaf<T>>>
class LinkedList {
public:
    LinkedList();
    ~LinkedList();

    LinkedList(std::initializer_list<T> list);

    LinkedList(LinkedList&& other) {
        this->head = other.head;
        this->back = other.back;
        this->length = other.length;
        other.head = nullptr;
        other.back = nullptr;
    }

    LinkedList<T>& operator=(LinkedList<T>&& other) {
        this->head = other.head;
        this->back = other.back;
        this->length = other.length;
        other.head = nullptr;
        other.back = nullptr;
        return *this;
    }

    LinkedList<T>& operator=(LinkedList<T>&)    = delete;
    LinkedList(LinkedList<T>&)                  = delete;

    class Iterator {
    public:
        Iterator();

        void operator++(int) {
            if(curr_node->next == nullptr) return;
            curr_node = curr_node->next;
        }

        void operator++() {
            if(curr_node->next == nullptr) return;
            curr_node = curr_node->next;
        }

        void operator--(int) {
            if(curr_node->prev == nullptr) return;
            curr_node = curr_node->prev;
        }

        void operator--() {
            if(curr_node->prev == nullptr) return;
            curr_node = curr_node->prev;
        }

        void operator+=(size_t n) {
            for(size_t i = 0; i < n; i++)
                this->operator++();
        }

        void operator-=(size_t n) {
            for(size_t i = 0; i < n; i++)
                this->operator--();
        }

        bool operator==(Iterator other) {
            if(this->curr_node == other.curr_node) return true;
            else return false;
        }

        bool operator!=(Iterator other) {
            return !(*this == other);
        }

        T& operator*() {
            return curr_node->data;
        }
    private:
        Iterator(Leaf<T>* ptr){ this->curr_node = ptr; }

        Leaf<T>* curr_node;
        friend LinkedList;
    };

    void insert(T& key);
    void insert(T&& key);
    void insert_front(T& key);
    void insert_front(T&& key);
    void insert_back(T& key);
    void insert_back(T&& key);
    
    void remove(T& key);
    void remove(T&& key);
    void remove(Iterator itr);
    void pop_front();
    void pop_back();

    size_t find(T& key);
    size_t find(T&& key);
    inline bool contains(T& key);
    inline bool contains(T&& key);

    inline T& first();
    inline T& last();
    inline size_t len();
    inline Iterator begin();
    inline Iterator end();

private:
    Leaf<T>*        head    = nullptr;
    Leaf<T>*        back    = nullptr;
    size_t     length  = 0;
    alloc           allocator;
};

template<typename T, typename alloc>
LinkedList<T, alloc>::LinkedList() {
    head = allocator.allocate(1);
    back = allocator.allocate(1);
    head->next = back;
}

template<typename T, typename alloc>
LinkedList<T, alloc>::~LinkedList() {
    Leaf<T>* itr = head->next;
    
    while(itr->next != nullptr) {
        allocator.deallocate(itr->prev, 1);
        itr = itr->next;
    }
}

template<typename T, typename alloc>
LinkedList<T, alloc>::LinkedList(std::initializer_list<T> list) {
    for(auto& i : list) insert(i);
}

template<typename T, typename alloc>
void LinkedList<T, alloc>::insert(T& key) {
    back->data = key;
    back->next = allocator.allocate(1);
    back->next->prev = back;
    back = back->next;
    length++;
}

template<typename T, typename alloc>
void LinkedList<T, alloc>::insert(T&& key) {
    back->data = std::move(key);
    back->next = allocator.allocate(1);
    back->next->prev = back;
    back = back->next;
    length++;
}

template<typename T, typename alloc>
void LinkedList<T, alloc>::insert_front(T& key) {
    Leaf<T>* new_head = allocator.allocate(1);
    new_head->data = key;
    new_head->next = head->next;
    head->next = new_head;
    length++;
}

template<typename T, typename alloc>
void LinkedList<T, alloc>::insert_front(T&& key) {
    Leaf<T>* new_head = allocator.allocate(1);
    new_head->data = std::move(key);
    new_head->next = head->next;
    head->next = new_head;
    length++;
}

template<typename T, typename alloc>
void LinkedList<T, alloc>::insert_back(T& key) {
    insert(key);
}

template<typename T, typename alloc>
void LinkedList<T, alloc>::insert_back(T&& key) {
    insert(key);
}

template<typename T, typename alloc>
void LinkedList<T, alloc>::remove(T& key) {
    size_t i = find(key);
    if(i == std::numeric_limits<size_t>::max()) return;
    
    if(i == 0) {
        pop_front();
        return;
    }

    if(i == length-1) {
        pop_back();
        return;
    }

    Leaf<T>* itr = head->next;
    for(auto x = 0; x < i; x++) itr = itr->next;
    // We have reached the node
    itr->prev->next = itr->next;
    itr->next->prev = itr->prev;
    allocator.deallocate(itr, 1);
    length--;
}

template<typename T, typename alloc>
void LinkedList<T, alloc>::remove(T&& key) {
    size_t i = find(key);
    if(i == std::numeric_limits<size_t>::max()) return;
    
    if(i == 0) {
        pop_front();
        return;
    }

    if(i == length-1) {
        pop_back();
        return;
    }

    Leaf<T>* itr = head->next;
    for(auto x = 0; x < i; x++) itr = itr->next;
    // We have reached the node
    itr->prev->next = itr->next;
    itr->next->prev = itr->prev;

    allocator.deallocate(itr, 1);
    length--;
}

template<typename T, typename alloc>
void LinkedList<T, alloc>::remove(Iterator itr) {
    if(itr.curr_node == head->next) 
    { pop_front(); return; }
    
    if(itr.curr_node == back->prev) 
    { pop_back(); return; };
    
    itr.curr_node->prev->next = itr.curr_node->next;
    itr.curr_node->next->prev = itr.curr_node->prev;
    allocator.deallocate(itr.curr_node, 1);
    length--;
}

template<typename T, typename alloc>
void LinkedList<T, alloc>::pop_front() {
    auto del_node = head->next;
    head->next = del_node->next;
    del_node->next->prev = nullptr;
    allocator.deallocate(del_node, 1);
    length--;
}

template<typename T, typename alloc>
void LinkedList<T, alloc>::pop_back() {
    auto del_node = back->prev;
    del_node->prev->next = back;
    back->prev = del_node->prev;
    allocator.deallocate(del_node, 1);
    length--;
}

template<typename T, typename alloc>
size_t LinkedList<T, alloc>::find(T& key) {
    size_t index = 0;
    for(auto& i : *this) {
        if(i == key) return index;
        index++;
    }        
    return std::numeric_limits<size_t>::max();
}

template<typename T, typename alloc>
size_t LinkedList<T, alloc>::find(T&& key) {
    size_t index = 0;
    for(auto& i : *this) {
        if(i == key) return index;
        index++;
    }       
    return std::numeric_limits<size_t>::max();
}

template<typename T, typename alloc>
bool LinkedList<T, alloc>::contains(T& key) {
    for(auto& i : *this) if(i == key) return true;
    return false;
}

template<typename T, typename alloc>
bool LinkedList<T, alloc>::contains(T&& key) {
    for(auto& i : *this) if(i == key) return true;
    return false;
}

template<typename T, typename alloc>
T& LinkedList<T, alloc>::first() {
    return head->next->data;
}
template<typename T, typename alloc>
T& LinkedList<T, alloc>::last() {
    return back->prev->data;
}

template<typename T, typename alloc>
size_t LinkedList<T, alloc>::len() {
    return length;
}

template<typename T, typename alloc>
typename LinkedList<T, alloc>::Iterator LinkedList<T, alloc>::begin() {
    return Iterator(head->next);
}

template<typename T, typename alloc>
typename LinkedList<T, alloc>::Iterator LinkedList<T, alloc>::end() {
    return Iterator(back);
}

}