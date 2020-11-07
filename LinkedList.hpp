#pragma once
#include <memory>
#include <limits>
#include <cstdio>
#include <functional>
namespace gdamn::data {

template<typename T>
class LinkedList {
private:
    template<typename U>
    struct DoubleNode {
        DoubleNode<U>* next = nullptr;
        DoubleNode<U>* prev = nullptr;
        U data;
    };

public:
    LinkedList();
    ~LinkedList();

    LinkedList(std::initializer_list<T> list);

    LinkedList(LinkedList<T>&& other) {
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

    LinkedList(LinkedList<T>&);

    class Iterator {
    public:
        Iterator() {}

        Iterator(Iterator& itr) {
            this->curr_node = itr.curr_node;
        }

        Iterator(Iterator&& itr) {
            this->curr_node = itr.curr_node;
            itr.curr_node = nullptr;
        }

        Iterator& operator=(Iterator& other) {
            this->curr_node = other.curr_node;
            return *this;
        }

        Iterator& operator=(Iterator&& other) {
            this->curr_node = other.curr_node;
            other.curr_node = nullptr;
            return *this;
        }

        Iterator& operator++() {
            if(curr_node->next == nullptr) return *this;
            curr_node = curr_node->next;
            return *this;
        }

        Iterator& operator++(int) {
            if(curr_node->next == nullptr) return *this;
            curr_node = curr_node->next;
            return *this;
        }

        Iterator& operator--() {
            if(curr_node->prev == nullptr) return *this;
            curr_node = curr_node->prev;
            return *this;
        }

        Iterator& operator--(int) {
            if(curr_node->prev == nullptr) return *this;
            curr_node = curr_node->prev;
            return *this;
        }

        Iterator& operator+=(size_t n) {
            for(size_t i = 0; i < n; i++)
                this->operator++();
            return *this;
        }

        Iterator& operator-=(size_t n) {
            for(size_t i = 0; i < n; i++)
                this->operator--();
            return *this;
        }

        bool operator==(const Iterator other) const {
            return this->curr_node == other.curr_node;
        }

        bool operator!=(const Iterator other) const {
            return !(this->curr_node == other.curr_node);
        }

        T& operator*() {
            return curr_node->data;
        }
    private:
        Iterator(DoubleNode<T>* ptr){ this->curr_node = ptr; }

        friend LinkedList<T>;
        DoubleNode<T>* curr_node = nullptr;
    };

    void for_each(std::function<void(T&)> call_back);

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
    inline bool contains(const T& key);
    inline bool contains(const T&& key);

    inline T& first();
    inline T& last();
    inline size_t len() const;
    inline Iterator begin();
    inline Iterator end();

private:
    DoubleNode<T>*        head    = nullptr;
    DoubleNode<T>*        back    = nullptr;
    size_t                length  = 0;
    friend                LinkedList<T>::Iterator;
};

template<typename T>
LinkedList<T>::LinkedList() {
    head = new DoubleNode<T>();
    back = new DoubleNode<T>();
    head->next = back;
    back->prev = nullptr;
    back->next = nullptr;
}

template<typename T>
LinkedList<T>::~LinkedList() {
    DoubleNode<T>* itr = head->next;

    while(itr->next != nullptr) {
        itr = itr->next;
        delete itr->prev;
    }
    
    delete head;
}

template<typename T>
LinkedList<T>::LinkedList(std::initializer_list<T> list) {
    for(auto& i : list) insert(i);
}

template<typename T>
void LinkedList<T>::for_each(std::function<void(T&)> call_back) {
    for(auto& val : *this) call_back(val);
}

template<typename T>
void LinkedList<T>::insert(T& key) {
    back->data = key;
    back->next = new DoubleNode<T>();
    back->next->prev = back;
    back = back->next;
    length++;
}

template<typename T>
void LinkedList<T>::insert(T&& key) {
    back->data = std::move(key);
    back->next = new DoubleNode<T>();
    back->next->prev = back;
    back = back->next;
    length++;
}

template<typename T>
void LinkedList<T>::insert_front(T& key) {
    DoubleNode<T>* new_head = new DoubleNode<T>();
    new_head->data = key;
    new_head->next = head->next;
    head->next = new_head;
    length++;
}

template<typename T>
void LinkedList<T>::insert_front(T&& key) {
    DoubleNode<T>* new_head = new DoubleNode<T>();
    new_head->data = std::move(key);
    new_head->next = head->next;
    head->next = new_head;
    length++;
}

template<typename T>
void LinkedList<T>::insert_back(T& key) {
    insert(key);
}

template<typename T>
void LinkedList<T>::insert_back(T&& key) {
    insert(key);
}

template<typename T>
void LinkedList<T>::remove(T& key) {
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

    DoubleNode<T>* itr = head->next;
    for(auto x = 0; x < i; x++) itr = itr->next;
    // We have reached the node
    itr->prev->next = itr->next;
    itr->next->prev = itr->prev;
    delete itr;
    length--;
}

template<typename T>
void LinkedList<T>::remove(T&& key) {
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

    DoubleNode<T>* itr = head->next;
    for(auto x = 0; x < i; x++) itr = itr->next;
    // We have reached the node
    itr->prev->next = itr->next;
    itr->next->prev = itr->prev;

    delete itr;
    length--;
}

template<typename T>
void LinkedList<T>::remove(Iterator itr) {
    if(itr.curr_node == head->next) 
    { pop_front(); return; }
    
    if(itr.curr_node == back->prev) 
    { pop_back(); return; };
    
    itr.curr_node->prev->next = itr.curr_node->next;
    itr.curr_node->next->prev = itr.curr_node->prev;
    delete itr.curr_node;
    length--;
}

template<typename T>
void LinkedList<T>::pop_front() {
    auto del_node = head->next;
    head->next = del_node->next;
    del_node->next->prev = nullptr;
    delete del_node;
    length--;
}

template<typename T>
void LinkedList<T>::pop_back() {
    auto del_node = back->prev;
    del_node->prev->next = back;
    back->prev = del_node->prev;
    delete del_node;
    length--;
}

template<typename T>
size_t LinkedList<T>::find(T& key) {
    size_t index = 0;
    for(auto& i : *this) {
        if(i == key) return index;
        index++;
    }        
    return std::numeric_limits<size_t>::max();
}

template<typename T>
size_t LinkedList<T>::find(T&& key) {
    size_t index = 0;
    for(auto& i : *this) {
        if(i == key) return index;
        index++;
    }       
    return std::numeric_limits<size_t>::max();
}

template<typename T>
bool LinkedList<T>::contains(const T& key) {
    for(const auto& i : *this) if(i == key) return true;
    return false;
}

template<typename T>
bool LinkedList<T>::contains(const T&& key) {
    for(const auto& i : *this) if(i == key) return true;
    return false;
}

template<typename T>
T& LinkedList<T>::first() {
    return head->next->data;
}
template<typename T>
T& LinkedList<T>::last() {
    return back->prev->data;
}

template<typename T>
size_t LinkedList<T>::len() const {
    return length;
}

template<typename T>
typename LinkedList<T>::Iterator LinkedList<T>::begin() {
    return Iterator(head->next);
}

template<typename T>
typename LinkedList<T>::Iterator LinkedList<T>::end() {
    return Iterator(back);
}

}