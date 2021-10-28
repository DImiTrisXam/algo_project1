#ifndef __HASHTABLE__
#define __HASHTABLE__

#include <memory>
#include <string>
#include <vector>

struct Data {
  std::string key;
  std::vector<float> value;

  Data(std::string);
  ~Data();
};

typedef std::vector<std::shared_ptr<Data>> Bucket;

class HashTable {
  Bucket *table;
  size_t size;

public:
  int getsize();
  void insert(Data &);
  std::vector<float> *search(std::string);
  void display();
  HashTable(size_t s = 10000);
  ~HashTable();
};

#endif
