#include "LinkedList.hpp"
#include "Vector.hpp"
#include "Array.hpp"
#include "Deque.hpp"
#include "List.hpp"
#include "HashTable.hpp"
#include <string>
#include <iostream>

using namespace gdamn::data;

int main() {
    char l_l = false;
    std::cout << "CHOOSE LINKEDLIST (L), VECTOR(V) OR ARRAY(A): ";
    std::cin >> l_l;
    Enumerable<int> enumerable;

    if(l_l == 'L') {
        LinkedList<int> ll;
        std::cout << ">>>>>>>>>>>>> LINKEDLIST <<<<<<<<<<<<<<" << std::endl;
        for(size_t i = 0; i < 100; i++)
            ll.insert(i * 10);
        for(auto i : ll)
            std::cout << i << std::endl;
        std::cout << "LinkedList length: "<< ll.len() << std::endl;
        std::cout << "Linked List contains 100: " << ll.contains(100) << std::endl;
        
        ll.for_each([](int& key){
            key *= 10;
            std::cout << key << std::endl;
        });
    } else if(l_l == 'V') {
        Vector<int> vec;

        vec.realign(100); /* Optimize O(n) insertion to O(1) by preallocating */
        std::cout << ">>>>>>>>>>>>> VECTOR <<<<<<<<<<<<<<" << std::endl;
        for(size_t i = 0; i < 100; i++)
            vec.insert(i * 10);
        
        for(size_t i = 0; i < 100; i++)
            std::cout << vec[i] << std::endl;
        std::cout << "Vector Length: " << vec.len() << std::endl;
        std::cout << "Vector contains 10: " << vec.contains(10) << std::endl;

        vec.for_each([](int& key){
            key *= 10;
            std::cout << key << std::endl;
        });
    } else if(l_l == 'A') {
        std::cout << ">>>>>>>>>>>>> ARRAY <<<<<<<<<<<<<<" << std::endl;
        Array<int, 10> arr = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        
        arr.for_each([](int& val){
            std::cout << val << std::endl;
        });
        std::cout << "Contains 10: " << arr.contains(10) << std::endl;
        std::cout << "Index of 10: " << arr.find(10) << std::endl;
    }

    Deque<int> deque;
    
    for(int i = 0; i < 100; i++) deque.insert(i);

    std::cout << ">>>>>>>>>>>>> DEQUE <<<<<<<<<<<<<<" << std::endl;
    deque.for_each([](int& a){ a *= 10; });
    std::cout << deque.find(990) << std::endl; /* should be at last index */
    std::cout << deque.len() << std::endl;
    std::cout << "Deque contains 10: " << deque.contains(10) << std::endl;
    deque.remove(10);
    std::cout << "After removal: " << deque.contains(10) << std::endl;
    /* ... more examples here */

    std::cout << ">>>>>>>>>>>>> HASHTABLE <<<<<<<<<<<<<<" << std::endl;
    HashTable<std::string, std::string, 256> ht;
    ht.insert({ "HELLO WORLD", "HELLO BACK!" });
    std::string str = "HELLO WORLD";
    ht["ARE YOU THERE?"] = "YES";
    std::cout << "ARE YOU THERE?: " << ht["ARE YOU THERE?"] << std::endl;
    std::cout << "HELLO WORLD: " << ht[str] << std::endl;
    std::cout << "Items: " << ht.len() << std::endl;
    std::cout << "Contains 'HELLO WORLD': " << ht.contains("HELLO WORLD") << std::endl;
}