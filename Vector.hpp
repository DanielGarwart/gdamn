#pragma once
#include <initializer_list>
#include <cstring>
#include <limits>
#include <memory>
#include <functional>

namespace gdamn::data {

template<typename T, typename alloc = std::allocator<T>>
class Vector {
public:

    class Iterator {
    public:
        Iterator(T* ptr) {
            start = ptr;
        }

        void operator++(int) {
            advance++;
        }

        void operator++() {
            advance++;
        }

        void operator--(int) {
            if(advance == 0) return;
            advance--;
        }

        void operator--() {
            if(advance == 0) return;
            advance--;
        }

        void operator+=(size_t n) {
            advance += n;
        }

        void operator-=(size_t n) {
            advance += n;
        }

        bool operator!=(const Iterator other) const {
            return !(
                start + advance
                ==
                other.start + other.advance
            );
        }

        T& operator*() {
            return *(start + advance);
        }
    private:

        T* start;
        size_t advance = 0;
        friend Vector;
    };

    Vector();
    explicit Vector(size_t n); /* Make sure all integral types besides size_t are not accepted into this constructor */
    Vector(T item);
    Vector(std::initializer_list<T> items);
    ~Vector();

    void realign(size_t n);
    void for_each(std::function<void(T&)> call_back);

    void insert(T& item); // Return iterator
    void insert(T&& item);
    
    size_t find(T& key); // Return iterator
    size_t find(T&& key);

    inline bool contains(T& key); // Return iterator
    inline bool contains(T&& key);
    
    void remove(T& item);
    void remove(T&& item);
    void remove(Iterator& pos);

    inline size_t len() const;
    inline size_t available_reserve() const;
    auto begin();
    auto end();

    T& operator[](size_t i);
private:
    T* sub_data;
    size_t length = 0;
    size_t n_reserve = 0;
    alloc allocator;
};

template<typename T, typename alloc>
Vector<T, alloc>::Vector() {
    sub_data = allocator.allocate(1);
    n_reserve = 1;
    length = 0;
}

template<typename T, typename alloc>
Vector<T, alloc>::Vector(size_t n) {
    sub_data = allocator.allocate(n);
    n_reserve = n;
}

template<typename T, typename alloc>
Vector<T, alloc>::Vector(T item) {
    sub_data = allocator.allocate(1);
    sub_data[0] = item;
    length++;
}

template<typename T, typename alloc>
Vector<T, alloc>::Vector(const std::initializer_list<T> items) {
    length = items.size();
    sub_data = allocator.allocate(length);
    size_t i = 0;

    for(const auto& item : items) {
        sub_data[i] = item;
        i++;
    }
}

template<typename T, typename alloc>
Vector<T, alloc>::~Vector() {
    allocator.deallocate(sub_data, length + n_reserve);
}

template<typename T, typename alloc>
void Vector<T, alloc>::realign(size_t n) {
    T* temp_buffer = allocator.allocate(length);
    std::memcpy(temp_buffer, sub_data, sizeof(T) * length);
    allocator.deallocate(sub_data, length + n_reserve);
    n_reserve += n;
    sub_data = allocator.allocate(length + n_reserve);
    std::memcpy(sub_data, temp_buffer, sizeof(T) * length);
    allocator.deallocate(temp_buffer, length);
}

template<typename T, typename alloc>
void Vector<T, alloc>::for_each(std::function<void(T&)> call_back) {
    for(auto& val : *this) call_back(val);
}

template<typename T, typename alloc>
void Vector<T, alloc>::insert(T& item) {
    if(n_reserve == 0) realign(1);
    sub_data[length] = item;
    length++;
    n_reserve--; // Consumed reserved storage
}

template<typename T, typename alloc>
void Vector<T, alloc>::insert(T&& item) {
    if(n_reserve == 0) realign(1);
    sub_data[length] = std::move(item);
    length++;
    n_reserve--; // Consumed reserved storage
}

template<typename T, typename alloc>
size_t Vector<T, alloc>::find(T& item) {
    for(size_t i = 0; i < length; ++i)
        if(sub_data[i] == item) return i;
    return std::numeric_limits<size_t>::max(); // Item has not been found, so max size_t is returned
}

template<typename T, typename alloc>
size_t Vector<T, alloc>::find(T&& item) {
    for(size_t i = 0; i < length; ++i)
        if(sub_data[i] == item) return i;
    return std::numeric_limits<size_t>::max(); // Item has not been found, so max size_t is returned
}

template<typename T, typename alloc>
bool Vector<T, alloc>::contains(T& item) {
    return find(item) != std::numeric_limits<size_t>::max();
}

template<typename T, typename alloc>
bool Vector<T, alloc>::contains(T&& item) {
    return find(item) != std::numeric_limits<size_t>::max();
}

template<typename T, typename alloc>
void Vector<T, alloc>::remove(T& item) {
#define nshift (length - i + 1)
    size_t i = find(item);
    if(i == std::numeric_limits<size_t>::max()) return;

    T* lower_bounds = allocator.allocate(i);
    T* upper_bounds = allocator.allocate(nshift);
    std::memcpy(lower_bounds, sub_data, sizeof(T) * i);
    std::memcpy(upper_bounds, sub_data + nshift, sizeof(T) * nshift);
    allocator.deallocate(sub_data, length + n_reserve);

    sub_data = allocator.allocate((length - 1) + n_reserve);
    std::memcpy(sub_data, lower_bounds, sizeof(T) * i);
    std::memcpy(sub_data + (nshift-1), upper_bounds, sizeof(T) * nshift);

    length--;
    allocator.deallocate(lower_bounds, i);
    allocator.deallocate(upper_bounds, nshift);
}

template<typename T, typename alloc>
void Vector<T, alloc>::remove(T&& item) {
    size_t i = find(item);
    if(i == std::numeric_limits<size_t>::max()) return;

    T* lower_bounds = allocator.allocate(i);
    T* upper_bounds = allocator.allocate(nshift);
    std::memcpy(lower_bounds, sub_data, sizeof(T) * i);
    std::memcpy(upper_bounds, sub_data + nshift, sizeof(T) * nshift);
    allocator.deallocate(sub_data, length + n_reserve);

    sub_data = allocator.allocate((length - 1) + n_reserve);
    std::memcpy(sub_data, lower_bounds, sizeof(T) * i);
    std::memcpy(sub_data + (nshift-1), upper_bounds, sizeof(T) * nshift);

    length--;
    allocator.deallocate(lower_bounds, i);
    allocator.deallocate(upper_bounds, nshift);
}

template<typename T, typename alloc>
void Vector<T, alloc>::remove(Vector<T, alloc>::Iterator& pos) {
    if(pos == end()) return;
    auto index_shift = length - pos.advance + 1;
    T* lower_bounds = allocator.allocate(pos.advance + 1);
    T* upper_bounds = allocator.allocate(index_shift);
    std::memcpy(lower_bounds, sub_data, sizeof(T) * (pos.advance + 1));
    std::memcpy(upper_bounds, sub_data + (pos.advance + 1), sizeof(T) * index_shift);
    allocator.deallocate(sub_data, length + n_reserve);

    sub_data = allocator.allocate((length - 1) + n_reserve);
    std::memcpy(sub_data, lower_bounds, sizeof(T) * (pos.advance + 1));
    std::memcpy(sub_data + pos.advance, upper_bounds, sizeof(T) * index_shift);

    length--;
    allocator.deallocate(lower_bounds, pos.advance + 1);
    allocator.deallocate(upper_bounds, index_shift);
}

template<typename T, typename alloc>
size_t Vector<T, alloc>::len() const {
    return length;
}

template<typename T, typename alloc>
size_t Vector<T, alloc>::available_reserve() const {
    return n_reserve;
}

template<typename T, typename alloc>
auto Vector<T, alloc>::begin() {
    return Iterator(sub_data);
}

template<typename T, typename alloc>
auto Vector<T, alloc>::end() {
    return Iterator(sub_data + length);
}

template<typename T, typename alloc>
T& Vector<T, alloc>::operator[](size_t i) {
    return sub_data[i];
}

}