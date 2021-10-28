#include "HashTableRe.hpp"
#include <iostream>

/////////////////////////////////////////////

// Data functions

Data::Data(std::string k) : key(k) {}

Data::~Data() {
  std::cout << "Data destroyed\n";
}

/////////////////////////////////////////////

// hash function

unsigned long djb2(const char *str, size_t tsize) {
  unsigned long hash = 5381;
  int c;
  while (c = *str++)
    hash = ((hash << 5) + hash) + c; // hash * 33 + c
  return hash % tsize;
}

/////////////////////////////////////////////

// Get index of key in table from hashing

size_t getIndex(const char *key, size_t tsize) {
  unsigned long hash = djb2(key, tsize);
  return hash % tsize;
}

/////////////////////////////////////////////

int HashTable::getsize() {
  return size;
}

void HashTable::insert(Data &input) {
  size_t index = getIndex(input.key.c_str(), this->size);

  auto buck = std::make_shared<Data>(input);

  table[index].push_back(buck); // insert at the last bucket of the chain
}

std::vector<float> *HashTable::search(std::string key) {
  size_t index = getIndex(key.c_str(), this->size);
  Bucket bc = table[index];

  for (const auto &temp : bc) {
    if (temp->key.compare(key) == 0) // if you find key
      return &temp->value;
  }

  return nullptr;
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
}

HashTable::HashTable(size_t s /* = 10000*/) : size(s) {
  table = new Bucket[this->size]; // allocate buckets

  std::cout << "HashTable initialized\n";
}

HashTable::~HashTable() {
  delete[] table;

  std::cout << "HashTable destroyed\n";
}
