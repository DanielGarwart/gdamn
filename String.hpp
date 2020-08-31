#include <cstring>
#include <memory>

template<typename alloc = std::allocator<char>>
class String {
public:
    String();
    String(const char* str);
    String(String& other);
    String(String&& other);
    String& operator=(String& other);
    String& operator=(String&& other);
    ~String();

    bool operator==(String& other);
    bool operator==(String&& other);
    bool operator!=(String& other);
    bool operator!=(String&& other);

    inline size_t len();
    inline const char* str();
private:
    size_t      n_buffer_left = 12; // in 12 Bytes
    size_t      n_used = 0;
    char*       c_string = nullptr;
    alloc       m_allocator;
};

template<typename alloc>
String<alloc>::String() {
    c_string = m_allocator.allocate(n_buffer_left); // Reserves 12 Bytes
}

template<typename alloc>
String<alloc>::String(const char* str) {
    c_string = m_allocator.allocate(strlen(str) + n_buffer_left);
    strcpy(c_string, str);
}

template<typename alloc>
String<alloc>::String(String& other) {
    c_string = m_allocator.allocate(strlen(str) + n_buffer_left);
    strcpy(c_string, other.c_string);
}

template<typename alloc>
String<alloc>::String(String&& other) {
    this->n_buffer_left = other.n_buffer_left;
    this->n_used = other.n_used;
    this->c_string = other.c_string;
    other.c_string = nullptr; other.n_buffer_left = 0; other.n_used = 0;
}

template<typename alloc>
typename String<alloc>& String<alloc>::operator=(String& other) {
    m_allocator.deallocate(c_string, n_used + n_buffer_left);
    c_string = m_allocator.allocate(other.n_used + n_buffer_left);
    strcpy(c_string, other.c_string);
}

template<typename alloc>
typename String<alloc>& String<alloc>::operator=(String&& other) {
    this->n_buffer_left = other.n_buffer_left;
    this->n_used = other.n_used;
    this->c_string = other.c_string;
    other.c_string = nullptr; other.n_buffer_left = 0; other.n_used = 0;
}

template<typename alloc>
String<alloc>::~String() {
    m_allocator.deallocate(c_string, n_used + n_buffer_left);
}

template<typename alloc>
size_t String<alloc>::len() {
    return n_used;
}

template<typename alloc>
const char* String<alloc>::str() {
    return c_string;
}