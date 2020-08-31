#include <algorithm>
#include <iterator>
#include <utility>

namespace gdamn::algorithm {

template<typename container_iter, typename compare>
auto realign(container_iter begin, container_iter end, compare comp_func) {
    container_iter lower_bound = begin;
    std::advance(end, -1);

    for(auto itr = begin; itr < end; std::advance(itr, 1))
        if(comp_func(*itr, *end)) {
            std::swap(*lower_bound,*itr);
            std::advance(lower_bound, 1);
        }

    std::swap(*lower_bound,*end);
    return lower_bound;
}

template<typename iter, typename compare>
void qsort(iter begin, iter end, compare comp_func) {
    if (begin < end) {
        auto upper_bound = realign(begin, end, comp_func);
        qsort(begin, upper_bound, comp_func);
        qsort(upper_bound + 1, end, comp_func);
    }
}

template<typename container, typename compare>
void qsort(container& c, compare comp_func) {
    qsort(c.begin(), c.end(), comp_func);
}

}