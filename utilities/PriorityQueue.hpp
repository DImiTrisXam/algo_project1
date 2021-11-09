#ifndef __PRIORITYQUEUE__
#define __PRIORITYQUEUE__

#include <string>
#include <vector>
#include "utilities.hpp"


class PriorityQueue {
  std::vector<Neighbor> heap;

  void swap(Neighbor &, Neighbor &);
  int parent(int);
  int leftChild(int);
  int rightChild(int);
  void heapify_down(int);
  void heapify_up(int);

public:
  unsigned int size();
  bool empty();
  bool find(Neighbor);
  void push(Neighbor);
  void pop();
  Neighbor top();
};

#endif
