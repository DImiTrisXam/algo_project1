#ifndef __HASH__
#define __HASH__

#include <functional>
#include <list>
#include <vector>

class Data {
public:
  const std::vector<float> vec; // dianisma
  const std::string id;

  Data(const std::vector<float> &, const std::string &);
  void PRINT() const; /* DEBUG ONLY */
};

class HashTable {
public:
  HashTable(int k, int w, int pSize, unsigned int tableSize = 509);
  ~HashTable();
  void add(const std::vector<float>, const std::string);
  bool isEmpty() const;
  unsigned int getCurrentSize() const;
  void eraseAll();
  void PRINT() const; /* DEBUG ONLY */
  std::list<Data *> &getNeighborCandidates(Data &);
  int getTableSize();
  std::list<Data *> *getTable();

private:
  const unsigned int size;
  std::list<Data *> *table; // array of lists
  unsigned int containedItems = 0;
  std::list<std::function<size_t(const std::vector<float> &)>> hashFunctions;
  std::vector<float> r; // used by gHash only

  int ID(const std::vector<float> &) const;
  void generateHashFunctions(int, int, int); // called once in constructor
  void initr(int);                           // called once in constructor
  size_t gHash(const std::vector<float> &) const;
};

#endif