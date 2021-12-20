#ifndef __TREE__
#define __TREE__

#include "curve_.hpp"

class CompleteBinaryTree {
  struct Node {
    Data *curve;
    struct Node *left, *right;
    int index; // index of CompleteBinaryTree vector (index only used by leafs)

    bool isLeaf();
  };

  Node *root;
  std::vector<Data *> *leafs;
  int currentIndex;
  size_t counter;

  Node *createNode();
  Node *createNode(Data *);
  Data *computeMeanCurveRec(Node *);
  void changeIndexes();
  void changeIndexesRec(Node *);
  void eraseAll(Node *);
  void printRec(Node *);
  void print2DUtil(Node *root, int space);
  
public:
  size_t numOfNodes();
  bool checkComplete(Node *, int, int);
  Data *computeMeanCurve();
  void createTreeFromVectorEven(Node *, Node *, double, bool);
  void createTreeFromVectorOdd(double);
  void createTreeFromVectorOddRec(Node *, Node *, int, bool);
  void PRINT();
  void print2D();

  CompleteBinaryTree(std::vector<Data *> &);
  ~CompleteBinaryTree();
};

Curve_ *meanDiscreteFrechetCurve(const Data &, const Data &);
std::list<std::pair<size_t, size_t>> optimalTraversal(const Data &, const Data &);

#endif
