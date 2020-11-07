#include <concepts>
#include <algorithm>
#include "Vector.hpp"
#include "String.hpp"

namespace gdamn::algorithm {

template<typename T>
concept IsSortable = requires(T a, T b) { a < b; } || requires(T a, T b) { a > b; };

template<typename T>
concept CanBeIndexed = requires(T a, size_t i) {
    a.operator[](i);
};

template<typename T> // Can't find a better name lol
concept SelfSufficient = requires(T a) { a.len(); } && CanBeIndexed<T>;

template<gdamn::algorithm::CanBeIndexed container, typename lambda>
size_t partition(container& A, size_t low, size_t high, lambda comp_func) {
    size_t i = low - 1;
    for (size_t j = low; j < high; j++)
        if (comp_func(A[j], A[high])) {
            i++;
            std::swap(A[i], A[j]);
        }

    std::swap(A[i + 1], A[high]);
    return (i + 1);
}
 
template<gdamn::algorithm::CanBeIndexed Container>
void qsort(Container& container, size_t low, size_t high) {
    if (low < high) {
        size_t pivot = partition(container, low, high, []<typename T>(T& left, T& right) -> bool { return left <= right; });
        gdamn::algorithm::qsort(container, low, pivot - 1);
        gdamn::algorithm::qsort(container, pivot + 1, high);
    }
}

template<gdamn::algorithm::CanBeIndexed Container, typename lambda>
void qsort(Container& container, size_t low, size_t high, lambda cmp_func) {
    if (low < high) {
        size_t pivot = partition(container, low, high, cmp_func);
        gdamn::algorithm::qsort(container, low, pivot - 1);
        gdamn::algorithm::qsort(container, pivot + 1, high);
    }
}

template<gdamn::algorithm::SelfSufficient Container>
inline void qsort(Container& container) {
    gdamn::algorithm::qsort(container, 0, container.len() - 1);
}

template<gdamn::algorithm::SelfSufficient Container, typename lambda>
inline void qsort(Container& container, lambda cmp_func) {
    gdamn::algorithm::qsort(container, 0, container.len() - 1, cmp_func);
}

}