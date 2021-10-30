#include "HashTable.hpp"
#include <chrono>
#include <climits>
#include <cmath>
#include <iostream>
#include <random>

/////////////////////////////////////////////

// h_HashFunction functions

float dotProduct(const std::vector<float> &x, const std::vector<float> &y) {
  float product = 0;

  for (size_t i = 0; i < x.size(); i++)
    product += x[i] * y[i];

  return product;
}

int h_HashFunction::hash(std::vector<float> &p) {
  return (int)floor((dotProduct(p, this->v) + this->t) / (float)this->w);
}

h_HashFunction::h_HashFunction(int w_, int pSize) : w(w_) {
  // Create random engine with the help of seed
  unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);

  std::normal_distribution<float> distN(0.0, 1.0);                  // standard normal distribution
  std::uniform_real_distribution<float> distU(0.0, (float)this->w); // uniform real distribution

  // pick v's coordinates from normal distribution
  for (size_t i = 1; i <= pSize; i++)
    this->v.push_back(distN(generator));

  this->t = distU(generator); // pick t from uniform distribution
}

h_HashFunction::~h_HashFunction() {
  // std::cout << "h_HashFunction destroyed\n";
}

/////////////////////////////////////////////

// g_HashFunction functions

int g_HashFunction::ID(std::vector<float> &p, HashTable &table) {
  int sum = 0;

  for (size_t i = 0; i < table.hfuncs.size(); i++)
    sum += this->r[i] * table.hfuncs[i]->hash(p);

  return abs(sum % M);
}

int g_HashFunction::hash(std::vector<float> &p, HashTable &table) {
  return ID(p, table) % table.size;
}

g_HashFunction::g_HashFunction(int pSize) {
  // Create random engine with the help of seed
  unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);

  std::uniform_int_distribution<int> distU(INT_MIN, INT_MAX); // uniform int distribution

  // pick v's coordinates from normal distribution
  for (size_t i = 1; i <= pSize; i++)
    this->r.push_back(distU(generator));

  this->M = UINT_MAX - 4; // 2^32 - 5
}

g_HashFunction::~g_HashFunction() {
  // std::cout << "g_HashFunction destroyed\n";
}

/////////////////////////////////////////////

// Data functions

Data::Data(std::string k) : key(k) {}

Data::~Data() {
  // std::cout << "Data destroyed\n";
}

/////////////////////////////////////////////

// HashTable functions

int HashTable::getsize() {
  return size;
}

void HashTable::insert(Data &input) {
  size_t index = this->gfunc->hash(input.value, *this);

  auto buck = std::make_shared<Data>(input);

  table[index].push_back(buck); // insert at the last bucket of the chain
}

void HashTable::display() {
  for (size_t i = 0; i < this->size; i++) {
    Bucket bc = table[i];

    if (!bc.empty()) { // if bucket is not empty
      std::cout << "\nBucket " << i << "\n\n";

      for (const auto &temp1 : bc) {
        std::cout << "Key: " << temp1->key << " Value: ";
        for (const auto &temp2 : temp1->value) {
          std::cout << temp2 << " ";
        }
        std::cout << "\n";
      }
    }
  }
  std::cout << "\n";
}

Bucket &HashTable::getNeighbourCandidates(Data &query) {
  size_t index = this->gfunc->hash(query.value, *this);

  return table[index];
}

HashTable::HashTable(int k, int w, int pSize, size_t s /* = 10000*/) : size(s) {
  table = new Bucket[this->size]; // allocate buckets

  for (size_t i = 0; i < k; i++) { // initialize h_i funcs
    auto h_i = std::make_shared<h_HashFunction>(w, pSize);
    hfuncs.push_back(h_i);
  }

  this->gfunc = new g_HashFunction(pSize); // initialize g func

  // std::cout << "HashTable initialized\n";
}

HashTable::~HashTable() {
  delete[] table;
  delete gfunc;

  // std::cout << "HashTable destroyed\n";
}
