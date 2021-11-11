#include "hash.hpp"
#include <chrono>
#include <climits>
#include <cmath>
#include <iostream>
#include <limits>
#include <random>

const unsigned int M = UINT_MAX - 4;
const double inf = std::numeric_limits<double>::infinity();

float dotProduct(const std::vector<float> &x, const std::vector<float> &y) {
  float product = 0;

  for (size_t i = 0; i < x.size(); i++)
    product += x[i] * y[i];

  return product;
}

Data::Data(const std::vector<float> &vec, const std::string &id) : vec(vec), id(id), cluster(-1), minDist(inf) {}

/* DEBUG ONLY */
void Data::PRINT() const {
  std::cerr << "Id: '" << id << "', ";
  for (const auto value : vec)
    std::cerr << value << " ";
  std::cerr << "\n";
}

/*
* @throws: "const std::string" if out of memory.
*/
HashTable::HashTable(int k, int w, int pSize, unsigned int tableSize) : size(tableSize) {
  table = new std::list<Data *>[size];
  if (!table) // out of heap
    throw "Unable to create hashtable with size = " + std::to_string(size) + ". Out of heap memory.";

  generateHashFunctions(k, w, pSize);
  initr(pSize);
}

HashTable::~HashTable() {
  eraseAll();
  delete[] table;
}

/*
* @throws: "const std::string" if out of memory.
*/
void HashTable::add(const std::vector<float> &vec, const std::string &id) {
  auto newData = new Data(vec, id);
  if (!newData) // out of heap
    throw "Unable to insert element with id: '" + id + "' in hashtable. Out of heap memory.";
  auto index = gHash(vec);
  table[index].push_front(newData);
  containedItems++;
}

/*
* @return: true only if the hashtable is empty.
*/
bool HashTable::isEmpty() const {
  return containedItems == 0;
}

/*
* @return: the number of items contained in the hashtable.
*/
unsigned int HashTable::getCurrentSize() const {
  return containedItems;
}

void HashTable::eraseAll() {
  for (size_t i = 0; i < size; i++) {
    for (const auto *data : table[i])
      delete data;
  }
}

/*
* DEBUG ONLY
*/
void HashTable::PRINT() const {
  for (size_t i = 0; i < size; i++) {
    std::cerr << "Bucket " << i << " -------------------\n";
    for (const Data *data : table[i]) {
      if (!table[i].empty())
        data->PRINT();
    }
  }
}

/*
* Generates the hash functions.
*/
void HashTable::generateHashFunctions(int k, int w, int pSize) {
  for (size_t i = 0; i < k; i++) {
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);

    std::normal_distribution<float> distN(0.0, 1.0);            // standard normal distribution
    std::uniform_real_distribution<float> distU(0.0, (float)w); // uniform real distribution

    // pick v's coordinates from normal distribution
    std::vector<float> v;
    for (size_t i = 1; i <= pSize; i++)
      v.push_back(distN(generator));
    int t = distU(generator); // pick t from uniform distribution

    hashFunctions.push_front([=](const std::vector<float> &vec) { // lambda captures "whatever needed" by value
      return (size_t)floor((dotProduct(vec, v) + t) / (float)w);
    });
  }
}

/*
* Calls all 'hashFunctions' and combines the results.
*/
int HashTable::ID(const std::vector<float> &p) const {
  int sum = 0;
  int i = 0;

  for (const auto &hashFunction : hashFunctions) {
    sum += r[i++] * hashFunction(p);
  }

  return abs(sum % M);
}

void HashTable::initr(int pSize) {
  unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);

  std::uniform_int_distribution<int> distU(INT_MIN, INT_MAX); // uniform int distribution

  // pick v's coordinates from normal distribution
  for (size_t i = 1; i <= pSize; i++)
    this->r.push_back(distU(generator));
}

size_t HashTable::gHash(const std::vector<float> &p) const {
  return ID(p) % size;
}

/*
* Returns a list of items that are candidates for closest neighbor.
*/
std::list<Data *> &HashTable::getNeighborCandidates(Data &query) {
  size_t index = gHash(query.vec);

  return table[index];
}

int HashTable::getTableSize() {
  return size;
}

std::list<Data *> *HashTable::getTable() {
  return table;
}
