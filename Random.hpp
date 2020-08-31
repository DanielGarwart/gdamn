#include <ctime>
#include <cstdint>

namespace gdamn::system {

template<typename T, size_t a, size_t c, size_t m>
class LCG {
public:

    LCG(size_t seed) { 
        value = seed; 
    }
    
    LCG() {
        value = (size_t)std::time(nullptr);
    }

    T next() {
        value = (a * value + c) % m;
        return (T)value;
    };

    void seed(size_t seed) {
        value = seed;
    }

private:
    size_t value;
};

// 214013 2531011 ( mod 2 31 )
typedef LCG<uint64_t, 214013, 2531011, 0xFFFFFFFFFFFFFFFF>  LCGU64;
typedef LCG<int64_t, 214013, 2531011, 0xFFFFFFFFFFFFFFFF>   LCG64;
typedef LCG<uint32_t, 214013, 2531011, 0xFFFFFFFF>          LCGU32;
typedef LCG<uint32_t, 214013, 2531011, 0xFFFFFFFF>          LCG32;

}