#pragma once
#include <limits>

namespace gdamn::data {

template<typename T>
struct Node {
    T data          = {};
    Node<T>* next   = nullptr;
};

template<typename T>
class List {
public:
    List(T head = T());
    List(std::initializer_list<T> l);
    ~List();

    List<T>& operator=(List<T>&& other);
    List<T>& operator=(List<T>& other) = delete;

    class Iterator;

    void insert(T& key);
    void insert(T&& key);

    void remove(T& key);
    void remove(T&& key);
    void pop_front();

    size_t find(T& key);
    size_t find(T&& key);
    bool contains(T& key);
    bool contains(T&& key);

    T& front();

    Iterator begin()    { return Iterator(head); }
    Iterator end()      { return Iterator(nullptr); };
    size_t len()        { return node_count; };

private:
    size_t node_count   = 0;
    Node<T>* head       = nullptr;
};

template<typename T>
class List<T>::Iterator {
public:
    void operator++() {
        curr_node = curr_node->next;
    }

    void operator++(int) {
        curr_node = curr_node->next;
    }

    void operator+=(size_t n);

    bool operator==(Iterator other) {
        return this->curr_node == other.curr_node;
    }

    bool operator!=(Iterator other) {
        return !(*this == other);
    }
 
    T& operator*() {
        return curr_node->data;
    }

private:
    Iterator(Node<T>* itr) {
        this->curr_node = itr;
    }

    Node<T>* curr_node;
    friend List<T>;
};

template<typename T>
List<T>::List(T head_value) {
    this->head = new Node<T>();
    head->data = head_value;
    node_count++;
}

template<typename T>
List<T>::List(std::initializer_list<T> l) {
    this->head = new Node<T>();

    for(const auto& key : l) {
        insert(key);
    }
}

template<typename T>
List<T>::~List() {
    auto itr = this->head;
    Node<T>* des_node = nullptr;

    while(itr != nullptr) {
        des_node = itr;
        itr = itr->next;
        delete[] des_node;
    }
}

template<typename T>
List<T>& List<T>::operator=(List<T>&& other) {
    this->head = other.head;
    this->node_count = other.node_count;

    other.head = nullptr;
    return *this;
}

template<typename T>
T& List<T>::front() {
    return head->data;
}

template<typename T>
void List<T>::insert(T& key) {
    head->data = key;
    head->next = new Node<T>();
    head = head->next;
    node_count++;
}

template<typename T>
void List<T>::insert(T&& key) {
    head->next = new Node<T>();
    head = head->next;
    head->data = std::move(key);
    node_count++;
}

template<typename T>
size_t List<T>::find(T& key) {
    size_t index = 0;
    for(auto& i : *this) {
        if(i == key) return index;
        index++;
    }       
    return std::numeric_limits<size_t>::max();
}

template<typename T>
size_t List<T>::find(T&& key) {
    size_t index = 0;
    for(auto& i : *this) {
        if(i == key) return index;
        index++;
    }       
    return std::numeric_limits<size_t>::max();
}

template<typename T>
bool List<T>::contains(T& key) {
    if(find(key) == std::numeric_limits<std::size_t>::max())    return false;
    else                                                        return true;
}

template<typename T>
bool List<T>::contains(T&& key) {
    if(find(key) == std::numeric_limits<std::size_t>::max())    return false;
    else                                                        return true;
}

template<typename T>
void List<T>::remove(T& key) {
    Node<T>* itr = head;
    Node<T>* prev;
    while(itr != nullptr) {
        if(itr->data == key) break;
        prev = itr;
        itr = itr->next;
    }
    if(itr == head) head = head->next;
    prev->next = itr->next;
    delete[] itr;
}

template<typename T>
void List<T>::remove(T&& key) {
    Node<T>* itr = head;
    Node<T>* prev;
    while(itr != nullptr) {
        if(itr->data == key) break;
        prev = itr;
        itr = itr->next;
    }
    if(itr == head) head = head->next;
    prev->next = itr->next;
    delete[] itr;
}

template<typename T>
void List<T>::pop_front() {
    if(head->next == nullptr) head->next = new Node<T>();
    auto del = head;
    head = head->next;
    delete[] del;
}

}