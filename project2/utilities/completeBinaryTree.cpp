#include "completeBinaryTree.hpp"
#include "metrics.hpp"
#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <set>
#include <string>

//////////////////////// Mean curve functions ////////////////////////

std::list<std::pair<size_t, size_t>> optimalTraversal(const Data &a, const Data &b) {
  auto x = (const Curve_ &)a;
  auto y = (const Curve_ &)b;
  auto m1 = x.vec.size();
  auto m2 = y.vec.size();
  double *distArray;

  try {
    distArray = new double[m1 * m2]; // array for dynamic programming
  } catch (const std::bad_alloc &e) {
    std::cout << "Allocation failed in optimalTraversal: " << e.what() << '\n';
  }

  // if (!distArray) // out of heap
  //   throw "Unable to allocate array in optimalTraversal. Out of heap memory.";

  for (auto i = 0; i < m1; ++i) { // compute distance matrix
    for (auto j = 0; j < m2; ++j) {
      // std::cout << "i: " << i << ",j: " << j << '\n';

      std::vector<float> temp1{x.tVec[i], x.vec[i]}; // i-th element of Curve_ x
      std::vector<float> temp2{y.tVec[j], y.vec[j]}; // j-th element of Curve_ y

      Data x_i(temp1, "temp1");
      Data y_j(temp2, "temp2");

      if (i == 0 && j == 0) // if first cell
        distArray[0] = euclidianDist(x_i, y_j);
      else if (i == 0 && j > 0) { // if first row
        // distArray[j] = distArray[0][j]
        distArray[j] = std::max(distArray[j - 1], euclidianDist(x_i, y_j));
      } else if (i > 0 && j == 0) { // if first column
        // distArray[i* m2] = distArray[i][0]
        distArray[i * m2] = std::max(distArray[(i - 1) * m2], euclidianDist(x_i, y_j));
      } else if (i > 0 && j > 0) { // anything else
        // distArray[(i - 1) * m2 + j] = distArray[i-1][j]
        auto d1 = distArray[(i - 1) * m2 + j];
        // distArray[(i - 1) * m2 + j - 1] = distArray[i-1][j-1]
        auto d2 = distArray[(i - 1) * m2 + j - 1];
        // distArray[i * m2 + j - 1] = distArray[i][j-1]
        auto d3 = distArray[i * m2 + j - 1];

        double min = std::min(d1, std::min(d2, d3));

        // distArray[i * m2 + j] = distArray[i][j]
        distArray[i * m2 + j] = std::max(min, euclidianDist(x_i, y_j));
      }
    }
  }

  std::list<std::pair<size_t, size_t>> traversal;

  auto P_i = m1 - 1;
  auto Q_i = m2 - 1;

  std::pair<size_t, size_t> tuple(P_i, Q_i);

  traversal.push_front(tuple);

  while (P_i > 0 && Q_i > 0) {
    auto d1 = distArray[(P_i - 1) * m2 + Q_i];     // distArray[P_i - 1][Q_i]
    auto d2 = distArray[P_i * m2 + Q_i - 1];       // distArray[P_i][Q_i-1]
    auto d3 = distArray[(P_i - 1) * m2 + Q_i - 1]; // distArray[P_i-1][Q_i-1]
    auto min = std::min(d1, std::min(d2, d3));

    if (min == d1) {
      std::pair<size_t, size_t> tuple(--P_i, Q_i);
      traversal.push_front(tuple);
    } else if (min == d2) {
      std::pair<size_t, size_t> tuple(P_i, --Q_i);
      traversal.push_front(tuple);
    } else {
      std::pair<size_t, size_t> tuple(--P_i, --Q_i);
      traversal.push_front(tuple);
    }
  }

  delete[] distArray;

  return traversal;
}

Curve_ *meanDiscreteFrechetCurve(const Data &a, const Data &b) {
  auto traversal = optimalTraversal(a, b);

  auto x = (const Curve_ &)a;
  auto y = (const Curve_ &)b;
  std::vector<float> vec;
  std::vector<float> tVec;

  for (const auto tuple : traversal) {
    vec.push_back((x.vec[tuple.first] + y.vec[tuple.second]) / 2);
    tVec.push_back((x.tVec[tuple.first] + y.tVec[tuple.second]) / 2);
  }

  // std::cout << "mean curve size: " << vec.size() << "\n";

  return new Curve_(vec, tVec, "mean");
}

Curve_ *reduceMeanCurveSize(Curve_ &c) {
  std::vector<float> temp1; // for y axis
  std::vector<float> temp2; // for x axis (time)

  temp1.push_back(c.vec[0]);
  temp2.push_back(c.tVec[0]);

  // get minima maxima
  for (int i = 1; i < c.vec.size() - 1; i++) {
    if (c.vec[i] >= std::min(c.vec[i - 1], c.vec[i + 1]) && c.vec[i] <= std::max(c.vec[i - 1], c.vec[i + 1])) {
      temp1.push_back(c.vec[i]);
      temp2.push_back(c.tVec[i]);
    }
  }

  temp1.push_back(c.vec[c.vec.size() - 1]);
  temp2.push_back(c.tVec[c.vec.size() - 1]);

  c.vec = temp1;
  c.tVec = temp2;

  // std::cout << "reduced mean curve size: " << c.vec.size() << "\n";

  return &c;
}

Curve_ *fitMeanCurveToSize(Curve_ &c, size_t size) {
  auto diff = c.vec.size() - size;
  std::set<size_t> indexes;
  unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);

  std::uniform_int_distribution<size_t> distribution(0, diff);

  while (indexes.size() < diff) { // choose #diff indexes
    indexes.insert(distribution(generator));
  }

  std::vector<float> temp1; // for y axis
  std::vector<float> temp2; // for x axis (time)

  // choose #size random points from curve
  for (auto i = 0; i < c.vec.size(); i++) {
    if (indexes.find(i) == indexes.end()) { // if you don't find index
      temp1.push_back(c.vec[i]);
      temp2.push_back(c.tVec[i]);
    }
  }

  c.vec = temp1;
  c.tVec = temp2;

  // std::cout << "fit mean curve size: " << c.vec.size() << "\n";

  return &c;
}

//////////////////////////////// Node ////////////////////////////////

bool CompleteBinaryTree::Node::isLeaf() {
  return left == nullptr && right == nullptr;
}

///////////////////////// CompleteBinaryTree /////////////////////////

CompleteBinaryTree::Node *CompleteBinaryTree::createNode() {
  Node *node = new Node;
  node->curve = nullptr;
  node->index = -1;
  node->right = nullptr;
  node->left = nullptr;

  return node;
}

CompleteBinaryTree::Node *CompleteBinaryTree::createNode(Data *curve) {
  Node *node = new Node;
  node->curve = (Curve_ *)curve;
  node->index = -1;
  node->right = nullptr;
  node->left = nullptr;

  return node;
}

size_t CompleteBinaryTree::numOfNodes() {
  return counter;
}

// Check if the tree is a complete binary tree
bool CompleteBinaryTree::checkComplete(Node *node, int index, int numberNodes) {
  // Check if the tree is empty
  if (root == nullptr)
    return true;

  if (index >= numberNodes)
    return false;

  return checkComplete(node->left, 2 * index + 1, numberNodes) && checkComplete(node->right, 2 * index + 2, numberNodes);
}

Data *CompleteBinaryTree::computeMeanCurveRec(Node *node) {
  if (node->isLeaf())
    return node->curve;
  else {
    auto leftCurve = computeMeanCurveRec(node->left);
    Data *rightCurve;

    if (node->right != nullptr)
      rightCurve = computeMeanCurveRec(node->right);
    else
      return leftCurve;

    auto mean = meanDiscreteFrechetCurve(*leftCurve, *rightCurve);
    auto dim = (*leafs)[0]->vec.size();

    // if (mean->vec.size() > 2 * dim) // if size of mean curve is too big
    //   return reduceMeanCurveSize(*mean);
    // else
    //   return mean;
    return fitMeanCurveToSize(*mean, dim);
  }
}

Data *CompleteBinaryTree::computeMeanCurve() {
  auto mean = (Curve_ *)computeMeanCurveRec(root);

  auto dim = (*leafs)[0]->vec.size();

  if (mean->vec.size() > dim)
    return fitMeanCurveToSize(*mean, dim);
  else if (mean->vec.size() < dim) {
    std::cout << "SHIT\n";
    return mean;
  } else
    return mean;
}

void CompleteBinaryTree::createTreeFromVectorEven(Node *node, Node *parent, double h, bool leftChild) {
  if (h == -1) // if you have reached a leaf
    return;

  if (currentIndex == leafs->size())
    return;

  if (node == nullptr) {
    node = createNode();

    if (leftChild)
      parent->left = node;
    else
      parent->right = node;

    counter++;
  }

  createTreeFromVectorEven(node->left, node, h - 1, true);
  createTreeFromVectorEven(node->right, node, h - 1, false);

  if (node->isLeaf()) {
    node->index = currentIndex;
    // std::cout << node->index << " ";
    currentIndex++;

    node->curve = (*leafs)[node->index];
  }
}

void CompleteBinaryTree::changeIndexesRec(Node *node) {
  if (node == nullptr)
    return;

  changeIndexesRec(node->left);
  changeIndexesRec(node->right);

  if (node->isLeaf()) {
    node->index = currentIndex;
    // std::cout << node->index << " ";
    currentIndex++;

    node->curve = (*leafs)[node->index];
  }

  return;
}

void CompleteBinaryTree::changeIndexes() {
  changeIndexesRec(root);

  return;
}

void CompleteBinaryTree::createTreeFromVectorOdd(double height) {
  for (auto i = 0; i <= height; i++)
    createTreeFromVectorOddRec(root, nullptr, i, false);

  changeIndexes();
}

void CompleteBinaryTree::createTreeFromVectorOddRec(Node *node, Node *parent, int level, bool leftChild) {
  if (counter == 2 * leafs->size() - 1)
    return;

  if (node == nullptr) { // if leaf
    node = createNode();

    if (leftChild)
      parent->left = node;
    else
      parent->right = node;

    counter++;

    return;
  }

  if (level == 0)
    return;
  else if (level > 0) {
    createTreeFromVectorOddRec(node->left, node, level - 1, true);
    createTreeFromVectorOddRec(node->right, node, level - 1, false);
    return;
  }
}

void CompleteBinaryTree::eraseAll(Node *node) {
  if (node == nullptr)
    return;

  eraseAll(node->left);
  eraseAll(node->right);

  if (node->curve != nullptr)
    delete node->curve;

  return;
}

void CompleteBinaryTree::printRec(Node *node) {
  if (node == nullptr)
    return;

  printRec(node->left);
  printRec(node->right);

  if (node->isLeaf())
    std::cout << node->index << " ";

  return;
}

void CompleteBinaryTree::PRINT() {
  printRec(root);

  std::cout << "\n";

  return;
}

void CompleteBinaryTree::print2DUtil(Node *node, int space) {
  // Base case
  if (node == nullptr)
    return;

  // Increase distance between levels
  space += 10;

  // Process right child first
  print2DUtil(node->right, space);

  // Print current node after space
  // count
  // std::cout<<std::endl;
  for (int i = 10; i < space; i++)
    std::cout << " ";
  std::cout << node->index << "\n";

  // Process left child
  print2DUtil(node->left, space);
}

// Wrapper over print2DUtil()
void CompleteBinaryTree::print2D() {
  // Pass initial space count as 0
  print2DUtil(root, 0);
}

CompleteBinaryTree::CompleteBinaryTree(std::vector<Data *> &leafs_) : leafs(&leafs_), currentIndex(0), counter(1) {
  root = createNode();
  auto maxHeight = ceil(log2(leafs->size()));
  // std::cout << "height " << maxHeight << "\n";

  if (leafs->size() % 2 == 0) // if even curves
    createTreeFromVectorEven(root, nullptr, maxHeight, false);
  else // if odd curves
    createTreeFromVectorOdd(maxHeight);

  // std::cout << "CompleteBinaryTree created\n";
}

CompleteBinaryTree::~CompleteBinaryTree() {
  if (root != nullptr) {
    eraseAll(root);
    delete root;
  }

  // std::cout << "CompleteBinaryTree destroyed\n";
}