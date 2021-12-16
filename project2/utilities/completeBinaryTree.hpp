#ifndef __TREE__
#define __TREE__

#include "curve.hpp"

class CompleteBinaryTree {
  struct Node {
    Data *curve;
    struct Node *left, *right;
    size_t index; // index of CompleteBinaryTree vector (index only used by leafs)

    bool isLeaf();
  };

  Node *root;
  std::vector<Data *> *leafs;
  size_t currentIndex;
  size_t numOfNodes;

  Node *createNode();
  Node *createNode(Data *);
  Data *computeMeanCurveRec(Node *);
  void eraseAll(Node *);
  void printRec(Node *);

public:
  int countNumNodes(Node *);
  bool checkComplete(Node *, int, int);
  Data *computeMeanCurve();
  void createTreeFromVector(Node *, Node *, double, bool);
  void PRINT();
  CompleteBinaryTree(std::vector<Data *> &);
  ~CompleteBinaryTree();
};

Curve *meanDiscreteFrechetCurve(const Data &, const Data &);
std::list<std::pair<size_t, size_t>> optimalTraversal(const Data &, const Data &);

#endif
