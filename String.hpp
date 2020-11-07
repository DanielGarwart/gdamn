#pragma once
#include <cstring>
#include <limits>
#include <memory>

namespace gdamn::data {

template<size_t n = 16>
class BasicString {
public:
    /* Big 5 */
    BasicString();
    BasicString(const char*);
    BasicString(const BasicString&);
    BasicString(BasicString&&);
    ~BasicString();

    BasicString& operator=(const BasicString& other) {
        if(buffer != nullptr) std::memset(buffer, 0, sizeof(buffer));

        if(other.n_chars <= (n_chars + n_reserve) && this->buffer != nullptr) { // Enough space left for the required cBasicString    
            std::strcpy(buffer, other.buffer);
            this->n_reserve -= other.n_chars;
        } else {
            delete[] buffer;
            this->buffer    = new char[other.n_chars + other.n_reserve];
            this->n_chars   = other.n_chars;
            this->n_reserve = other.n_reserve;
            std::strcpy(buffer, other.buffer);
        }

        return *this;
    }

    BasicString& operator=(BasicString&& other) {
        delete[] buffer;
        this->n_chars = other.n_chars;
        this->n_reserve = other.n_reserve;
        this->buffer = other.buffer;
        other.buffer = nullptr;
        other.n_chars = 0;
        other.n_reserve = 0;
        return *this;
    }

    BasicString& operator=(const char* str)  {
        if(buffer != nullptr) std::memset(buffer, 0, sizeof(buffer));
        size_t str_n = strlen(str);

        if(str_n <= (n_chars + n_reserve)) {
            strcpy(buffer, str);
            n_reserve = (n_chars + n_reserve) - str_n;
            n_chars = str_n;
        } else {
            delete[] buffer;
            buffer = new char[str_n + n];
            strcpy(buffer, str);
            n_chars = str_n;
            n_reserve = n;
        }

        return *this;
    }

    void reserve(size_t num);

    bool contains(const char character) const {
        return strchr(buffer, (char)character) != nullptr;
    }

    bool contains(const char* str) const {
        return strstr(buffer, str) != nullptr;
    }

    bool contains(const BasicString& str) const {
        return this->contains(str.c_str());
    }

    bool contains(const BasicString&& str) const {
        return this->contains(str.c_str());
    }

    size_t find(char character) {
        char* where = strchr(buffer, (char)character); 
        return where == nullptr ? (size_t)(where - buffer) : std::numeric_limits<size_t>::max();    
    }

    size_t find(const char* str) {
        char* where = strstr(buffer, str); 
        return where == nullptr ? (size_t)(where - buffer) : std::numeric_limits<size_t>::max();
    }

    size_t find(BasicString& str) {
        return this->find(str.buffer);
    }

    size_t find(BasicString&& str) {
        return this->find(str.buffer);
    }

    BasicString from(BasicString&);
    BasicString from(BasicString&&);
    BasicString from(const char*);

    char& operator[](size_t i) { return buffer[i]; }
    
    bool operator==(const BasicString& other) const {
        return strcmp(buffer, other.buffer) == 0;
    }

    bool operator==(const char* other) const {
        return strcmp(buffer, other) == 0;
    }

    bool operator!=(const BasicString& other) const {
        return !(*this == other);
    }

    bool operator!=(const char* other) const {
        return !(*this == other);
    }

    bool operator<(const BasicString& other) const {
        return strcmp(buffer, other.buffer) < 0;
    }

    bool operator<(const char* other) const {
        return strcmp(buffer, other) < 0;
    }

    bool operator>(const BasicString& other) const {
        return !(*this < other);
    }

    bool operator>(const char* other) const {
        return !(*this < other);
    }

    bool operator<=(const BasicString& other) const {
        return strcmp(buffer, other.buffer) <= 0;
    }

    bool operator<=(const char* other) const {
        return strcmp(buffer, other) <= 0;
    }

    bool operator>=(const BasicString& other) const {
        return !(*this >= other);
    }

    bool operator>=(const char* other) const {
        return !(*this <= other);
    }

    void operator+=(const BasicString& str) {
        (*this) += str.buffer;
    }

    void operator+=(const BasicString&& str) {
        (*this) += str.buffer;
    }

    void operator+=(const char* str) {
        size_t n_str = strlen(str);

        if(n_str <= n_reserve) {
            strcpy(buffer + n_chars, str);
        } else {
            char* temp = new char[n_chars + n_str + n]; // Provide new reserves
            std::memset(temp, 0, sizeof(temp));
            strcpy(temp, buffer);
            strcpy(temp + n_chars, str);
            delete[] buffer;
            buffer = temp;
        }
    }
    
    const char* c_str() const { return buffer; }
    size_t len() const { return n_chars; }
    size_t available_reserve() const { return n_reserve; }

    template<size_t m>
    friend std::ostream& operator<< (std::ostream& os, const BasicString<m>& p_t);

private:
    char* buffer = nullptr;
    size_t n_chars = 0;
    size_t n_reserve = 0;
};

template<size_t n>
std::ostream& operator<<(std::ostream& os, const BasicString<n>& str) {
    os << str.c_str();
    return os;
}

template<size_t n>
BasicString<n>::BasicString() {
    buffer = new char[n];
    std::memset(buffer, 0, sizeof(buffer));
    n_reserve = n;
}

template<size_t n>
BasicString<n>::BasicString(const char* str) {
    if(str != nullptr) {
        buffer = new char[std::strlen(str) + n];
        std::strcpy(buffer, str);
        n_chars = std::strlen(str);
        n_reserve = n;
    } else {
        buffer = new char[n];
        std::memset(buffer, 0, sizeof(buffer));
        n_reserve = n;
    }
}

template<size_t n>
BasicString<n>::BasicString(const BasicString& other) {
    this->n_chars = other.n_chars;
    this->n_reserve = other.n_reserve;
    buffer = new char[n_chars + n_reserve];
    std::strcpy(buffer, other.c_str());
}

template<size_t n>
BasicString<n>::BasicString(BasicString&& other) {
    delete[] buffer;
    this->buffer = other.buffer;
    this->n_chars = other.n_chars;
    this->n_reserve = other.n_reserve;
    other.buffer = nullptr;
}

template<size_t n>
BasicString<n>::~BasicString() {
    if(buffer != nullptr) std::memset(buffer, 0, sizeof(buffer));
    delete[] buffer;
}

template<size_t n>
BasicString<n> BasicString<n>::from(BasicString& str) {
    return std::move(BasicString<n>(strstr(buffer, str.buffer)));
}

template<size_t n>
BasicString<n> BasicString<n>::from(BasicString<n>&& str) {
    return std::move(BasicString<n>(strstr(buffer, str.buffer)));
}

template<size_t n>
BasicString<n> BasicString<n>::from(const char* str) {
    return std::move(BasicString<n>(strstr(buffer, str)));
}

template<size_t n>
void BasicString<n>::reserve(size_t num) {
    char* temp = new char[n_chars + n_reserve + num];
    strcpy(temp, buffer);
    delete[] buffer;
    buffer = temp;
    n_reserve += num;
}

template<typename T, typename U, size_t bucket_count>
class HashTable;

/* Uses BasicString with 16 Byte Reservation divided into 256 buckets */
using Dictionary = HashTable<BasicString<16>, BasicString<16>, 256>;
using String = BasicString<16>;

}

namespace std {
    template <size_t n>
    struct hash<gdamn::data::BasicString<n>> {
        size_t operator()(const gdamn::data::BasicString<n>& str) {
            size_t hash = 0;
            for(size_t i = 0; i < str.len(); i++)
                hash += str.c_str()[i];
            return hash;
        }
    };
}