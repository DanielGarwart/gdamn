#include "LinkedList.hpp"
#include "Vector.hpp"
#include "Array.hpp"
#include "Deque.hpp"
#include "List.hpp"
#include <string>
#include <iostream>

using namespace gdamn::data;

int main() {
    char l_l = false;
    std::cout << "CHOOSE LINKEDLIST (L), VECTOR(V) OR ARRAY(A): ";
    std::cin >> l_l;

    if(l_l == 'L') {
        LinkedList<int> ll;
        for(size_t i = 0; i < 100; i++)
            ll.insert(i * 10);
        for(auto i : ll)
            std::cout << i << std::endl;
        std::cout << ll.len() << std::endl;
        std::cout << ll.contains(100) << std::endl;

        ll.for_each([](int& key){
            key *= 10;
            std::cout << key << std::endl;
        });
    } else if(l_l == 'V') {
        Vector<int> vec;

        vec.realign(100); /* Optimize O(n) insertion to O(1) by preallocating */

        for(size_t i = 0; i < 100; i++)
            vec.insert(i * 10);
        
        for(size_t i = 0; i < 100; i++)
            std::cout << vec[i] << std::endl;
        std::cout << vec.len() << std::endl;
        std::cout << vec.contains(10) << std::endl;

        vec.for_each([](int& key){
            key *= 10;
            std::cout << key << std::endl;
        });
    } else if(l_l == 'A') {
        Array<int, 10> arr = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        
        arr.for_each([](int& val){
            std::cout << val << std::endl;
        });
        std::cout << "Contains 10: " << arr.contains(10) << std::endl;
        std::cout << "Index of 10: " << arr.find(10) << std::endl;
    }

    Deque<int> deque;
    
    for(int i = 0; i < 100; i++)
    {
        deque.insert(i);
    }

    deque.for_each([](int& a){ a *= 10; });
    std::cout << deque.find(990) << std::endl; /* should be at last index */
    std::cout << deque.len() << std::endl;
    std::cout << "Deque contains 10: " << deque.contains(10) << std::endl;
    deque.remove(10);
    std::cout << "After removal: " << deque.contains(10) << std::endl;
    /* ... more examples here */
}