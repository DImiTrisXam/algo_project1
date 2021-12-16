#include "completeBinaryTree.hpp"
#include "metrics.hpp"
#include <cmath>
#include <iostream>
#include <string>

//////////////////////// Mean curve functions ////////////////////////

std::list<std::pair<size_t, size_t>> optimalTraversal(const Data &a, const Data &b) {
  auto x = (const Curve &)a;
  auto y = (const Curve &)b;
  auto m1 = x.vec.size();
  auto m2 = y.vec.size();
  double *distArray;

  try {
    distArray = new double[m1 * m2]; // array for dynamic programming
  } catch (const std::bad_alloc &e) {
    std::cout << "Allocation failed in discreteFrechetDist: " << e.what() << '\n';
  }

  // if (!distArray) // out of heap
  //   throw "Unable to allocate array in discreteFrechetDist. Out of heap memory.";

  for (auto i = 0; i < m1; ++i) { // compute distance matrix
    for (auto j = 0; j < m2; ++j) {
      // std::cout << "i: " << i << ",j: " << j << '\n';

      std::vector<float> temp1{(float)x.tVec[i], x.vec[i]}; // i-th element of Curve x
      std::vector<float> temp2{(float)y.tVec[j], y.vec[j]}; // j-th element of Curve y

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

Curve *meanDiscreteFrechetCurve(const Data &a, const Data &b) {
  auto traversal = optimalTraversal(a, b);

  auto x = (const Curve &)a;
  auto y = (const Curve &)b;
  std::vector<float> vec;
  std::vector<int> tVec;
  int i = 0;

  for (const auto tuple : traversal) {
    vec.push_back((x.vec[tuple.first] + y.vec[tuple.second]) / 2);
    tVec.push_back(++i);
  }

  Curve *mean = new Curve(vec, tVec, "mean");

  return mean;
}

//////////////////////////////// Node ////////////////////////////////

bool CompleteBinaryTree::Node::isLeaf() {
  return left == nullptr && right == nullptr;
}

///////////////////////// CompleteBinaryTree /////////////////////////

CompleteBinaryTree::Node *CompleteBinaryTree::createNode() {
  Node *node = new Node;
  node->curve = nullptr;
  node->right = node->left = nullptr;

  return node;
}

CompleteBinaryTree::Node *CompleteBinaryTree::createNode(Data *curve) {
  Node *node = new Node;
  node->curve = (Curve *)curve;
  node->right = node->left = nullptr;

  return node;
}

int CompleteBinaryTree::countNumNodes(Node *node) {
  if (root == nullptr)
    return 0;

  return 1 + countNumNodes(node->left) + countNumNodes(node->right);
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
      rightCurve = computeMeanCurveRec(node->left);
    else
      return leftCurve;

    return meanDiscreteFrechetCurve(*leftCurve, *rightCurve);
  }
}

Data *CompleteBinaryTree::computeMeanCurve() {
  return computeMeanCurveRec(root);
}

void CompleteBinaryTree::createTreeFromVector(Node *node, Node *parent, double h, bool leftChild) {
  if (h == 0) // if you have reached a leaf
    return;

  if (currentIndex == leafs->size())
    return;

  if (node == nullptr) {
    node = createNode();

    if (leftChild)
      parent->left = node;
    else
      parent->right = node;
  }

  createTreeFromVector(node->left, node, h - 1, true);
  createTreeFromVector(node->right, node, h - 1, false);

  if (node->isLeaf()) {
    node->index = currentIndex;
    // std::cout << node->index << " ";
    currentIndex++;

    node->curve = (*leafs)[node->index];
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

CompleteBinaryTree::CompleteBinaryTree(std::vector<Data *> &leafs_) : leafs(&leafs_), currentIndex(0), numOfNodes(1) {
  root = createNode();
  auto maxHeight = ceil(leafs->size());
  createTreeFromVector(root, nullptr, maxHeight, false);

  std::cout << "CompleteBinaryTree created\n";
}

CompleteBinaryTree::~CompleteBinaryTree() {
  if (root != nullptr) {
    eraseAll(root);
    delete root;
  }

  std::cout << "CompleteBinaryTree destroyed\n";
}