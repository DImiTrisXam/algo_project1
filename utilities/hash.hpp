#ifndef __HASH__
#define __HASH__

#include <functional>
#include <list>
#include <vector>

class Data {
public:
  std::vector<float> vec; // dianisma
  std::string id;
  int cluster;       // which cluster data point is in
  double minDist;    // distance to nearest cluster

  Data(std::vector<float>, std::string);
  void PRINT() const; /* DEBUG ONLY */
};

class HashTable {
public:
  HashTable(int k, int w, int pSize, unsigned int tableSize = 509);
  ~HashTable();
  virtual void add(std::vector<float> &, std::string &);
  bool isEmpty() const;
  unsigned int getCurrentSize() const;
  void eraseAll();
  void PRINT() const; /* DEBUG ONLY */
  std::list<Data *> &getNeighborCandidates(Data &);
  int getTableSize();
  std::list<Data *> *getTable();

protected:
  const unsigned int size;
  std::list<Data *> *table; // array of lists
  unsigned int containedItems = 0;
  std::list<std::function<int(std::vector<float> &)>> hashFunctions;

  void generateHashFunctions(int, int, int); // called once in constructor

private:
  std::vector<int> r; // used by gHash only

  int ID(std::vector<float> &) const;
  void initr(int); // called once in constructor
  size_t gHash(std::vector<float> &) const;
};

#endif