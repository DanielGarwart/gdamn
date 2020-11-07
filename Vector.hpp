#pragma once
#include <initializer_list>
#include <cstring>
#include <limits>
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

        Iterator& operator++(int) {
            advance++;
            return *this;
        }

        Iterator& operator++() {
            advance++;
            return *this;
        }

        Iterator& operator--(int) {
            advance -= (advance > 0);
            return *this;
        }

        Iterator& operator--() {
            advance -= (advance > 0);
            return *this;
        }

        Iterator& operator+=(size_t n) {
            advance += n;
            return *this;
        }

        Iterator& operator-=(size_t n) {
            advance -= advance >= n ? n : advance;
            return *this;
        }

        bool operator==(const Iterator other) const {
            return (
                start + advance
                ==
                other.start + other.advance
            );
        }

        bool operator!=(const Iterator other) const {
            return !(*this == other);
        }

        T& operator*() {
            return *(start + advance);
        }
    private:
        T* start;
        size_t advance = 0;
        friend Vector<T>;
    };

    Vector();
    explicit Vector(size_t n); /* Make sure all integral types besides size_t are not accepted into this constructor */
    Vector(T item);
    Vector(std::initializer_list<T> items);
    Vector(Vector&& other);
    Vector(Vector&);
    ~Vector();

    void realign(size_t n);
    void for_each(std::function<void(T&)> call_back);
    auto where(std::function<bool(T&)> match_func);

    void insert(T& item); // Return iterator
    void insert(T&& item);
    
    size_t find(T& key); // Return iterator
    size_t find(T&& key);

    inline bool contains(T& key);
    inline bool contains(T&& key);
    
    void remove(T& item);
    void remove(T&& item);
    void remove(Vector<T, alloc>::Iterator& pos);

    inline const size_t len() const;
    inline size_t available_reserve() const;
    auto begin();
    auto end();
    T& first() { return sub_data[0]; }
    T& last() { return sub_data[length - 1]; }

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
Vector<T, alloc>::Vector(Vector&& other) {
    allocator.deallocate(sub_data, length + n_reserve);
    this->sub_data = other.sub_data;
    this->length = other.length;
    this->n_reserve = other.n_reserve;
    other.sub_data = nullptr;
    other.length = 0;
    other.n_reserve = 0;
}

template<typename T, typename alloc>
Vector<T, alloc>::~Vector() {
    for(size_t i = 0; i < length; i++)
        sub_data[i].~T();
    allocator.deallocate(sub_data, length + n_reserve);
}

template<typename T, typename alloc>
void Vector<T, alloc>::realign(size_t n) {
    T* new_buffer = allocator.allocate(length + n_reserve + n);
    std::memcpy(new_buffer, sub_data, sizeof(sub_data));
    allocator.deallocate(sub_data, length + n_reserve);
    sub_data = new_buffer;
    n_reserve += n;
}

template<typename T, typename alloc>
void Vector<T, alloc>::for_each(std::function<void(T&)> call_back) {
    for(auto& val : *this) call_back(val);
}

template<typename T, typename alloc>
auto Vector<T, alloc>::where(std::function<bool(T&)> match_func) {
    // Enumerable<T> enumerable;
    // for(auto& x : *this) if(match_func(x)) enumerable.insert(x);
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

    T* new_buffer = allocator.allocate(length + n_reserve - 1);
    std::memcpy(new_buffer, sub_data, sizeof(T) * i);
    std::memcpy(new_buffer + i, sub_data + i + 1, sizeof(T) * (length - i - 1));
    sub_data[i].~T();
    allocator.deallocate(sub_data, length + n_reserve);

    length--;
    sub_data = new_buffer;
}

template<typename T, typename alloc>
void Vector<T, alloc>::remove(T&& item) {
    size_t i = find(item);
    if(i == std::numeric_limits<size_t>::max()) return;

    T* new_buffer = allocator.allocate(length + n_reserve - 1);
    std::memcpy(new_buffer, sub_data, sizeof(T) * i);
    std::memcpy(new_buffer + i, sub_data + i + 1, sizeof(T) * (length - i - 1));
    sub_data[i].~T();
    allocator.deallocate(sub_data, length + n_reserve);

    length--;
    sub_data = new_buffer;
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
const size_t Vector<T, alloc>::len() const {
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