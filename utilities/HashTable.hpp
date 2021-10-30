#ifndef __HASHTABLE__
#define __HASHTABLE__

#include <memory>
#include <string>
#include <vector>

class HashTable;

class h_HashFunction {
  std::vector<float> v;
  int t;
  int w;

public:
  int hash(std::vector<float> &);
  h_HashFunction(int, int);
  ~h_HashFunction();
};

class g_HashFunction {
  std::vector<float> r;
  unsigned int M;
  int ID(std::vector<float> &, HashTable &);

public:
  int hash(std::vector<float> &, HashTable &);
  g_HashFunction(int);
  ~g_HashFunction();
};

struct Data {
  std::string key;
  std::vector<float> value;

  Data(std::string);
  ~Data();
};

typedef std::vector<std::shared_ptr<Data>> Bucket;
typedef std::vector<std::shared_ptr<h_HashFunction>> HashArray;

class HashTable {
  Bucket *table;
  size_t size;
  HashArray hfuncs; // array of h_i funcs, size k
  g_HashFunction *gfunc;

  friend class g_HashFunction;

public:
  int getsize();
  void insert(Data &);
  void display();
  Bucket &getNeighbourCandidates(Data &);
  HashTable(int, int, int, size_t s = 10000);
  ~HashTable();
};

#endif
