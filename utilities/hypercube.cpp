#include "hypercube.hpp"
#include <iostream>

Hypercube::Hypercube(int k, int w, int pSize, unsigned int tableSize) : HashTable(k, w, pSize, tableSize) {}


/*
* @throws: "const std::string" if out of memory.
*/
void Hypercube::add(const std::vector<float> vec, const std::string id) {
    auto newData = new Data(vec, id);
    if (!newData) // out of heap
        throw "Unable to insert element with id: '" + id + "' in hashtable. Out of heap memory.";

    for (const auto &hashFunction : hashFunctions)
        std::cout << "index: " << hashFunction(vec) << std::endl;

    //auto index = gHash(vec);
    //table[index].push_front(newData);
    containedItems++;
}
