#include "HashTable.hpp"
#include <string>
#include <iostream>

using namespace gdamn::data;

int main() {
    HashTable<std::string, std::string, 2> ht;

    ht.insert({ "MISTAH", "NO" });
    ht["MISTAH"];
    ht.contains({ "MISTAH", "NO" });
    std::cout << "HI" << std::endl;
}