#include "hypercube.hpp"
#include <iostream>
#include <random>

Hypercube::Hypercube(int k, int w, int pSize, unsigned int tableSize) : HashTable(k, w, pSize, tableSize) {
  std::unordered_map<size_t, short> map;
  for (const auto &hashFunction : hashFunctions) {
    hValues.push_front(map);
  }
}

short Hypercube::f(size_t index, std::unordered_map<size_t, short> &hMap) {
  auto iter = hMap.find(index);

  // value found
  if (iter != hMap.end())
    return iter->second;

  // value does not exist - assign new random value
  short randNum;
  std::default_random_engine generator;
  std::random_device rd; //Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd());
  std::uniform_int_distribution<short> distribution(0, 1);
  randNum = distribution(gen);
  hMap[index] = randNum;
  return randNum;
}

size_t Hypercube::hash(const std::vector<float> &vec) {
  size_t vertice = 0, bit;
  auto hValuesIter = hValues.begin();
  for (const auto &hashFunction : hashFunctions) {
    bit = f(hashFunction(vec), *hValuesIter++);
    vertice = (vertice << 1) + bit;
  }

  return vertice;
}

/*
* @throws: "const std::string" if out of memory.
*/
void Hypercube::add(const std::vector<float> &vec, const std::string &id) {
  auto newData = new Data(vec, id);
  if (!newData) // out of heap
    throw "Unable to insert element with id: '" + id + "' in hashtable. Out of heap memory.";

  auto index = hash(vec);
  table[index].push_front(newData);
  containedItems++;
}

/*
* Returns a list of items that are candidates for closest neighbor.
*/
std::list<Data *> &Hypercube::getNeighborCandidates(Data &query, size_t &index_) {
  size_t index = hash(query.vec);
  index_ = index;

  return table[index];
}
