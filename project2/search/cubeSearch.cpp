#include "cubeSearch.hpp"
#include "../utilities/PriorityQueue.hpp"
#include "../utilities/metrics.hpp"
#include <algorithm>
#include <iostream>
#include <limits>

std::vector<Neighbor> trueDistanceN(Data &query, int k, HashTable *cube, const std::function<double(const Data &, const Data &)> &metric) {
  std::vector<Neighbor> b;                              // k best neighbors
  std::vector<Neighbor> temp;                           // helper vector to reverse b
  PriorityQueue pq;                                     // priority queue size k
  auto bDist = std::numeric_limits<double>::infinity(); // k'th best distance, initialize to infinity
  Neighbor n;

  n.id = "id";
  n.dist = bDist;

  for (size_t i = 0; i < k; i++) // initialize priority queue
    pq.push(n);

  auto size = cube->getTableSize();
  auto table = cube->getTable();

  for (auto j = 0; j < size; j++) {
    for (const auto &p : table[j]) { // for each item in bucket
      // distance between candidate and query
      auto dist = metric(query, *p);

      if (dist < bDist) { // if better than k-th best distance
        n.id = p->id;
        n.dist = dist;

        if (!pq.find(n)) { // if you don't find duplicate
          pq.pop();
          pq.push(n);            // put neighbor in k
          bDist = pq.top().dist; // change k-th best distance
        }
      }
    }
  }
  while (!pq.empty()) { // put items of queue to helper vector (descending order)
    temp.push_back(pq.top());
    pq.pop();
  }
  // put items of queue to actual vector (ascending order)
  for (auto it = temp.rbegin(); it != temp.rend(); ++it)
    b.push_back(*it);

  return b;
}

std::vector<Neighbor> approximateKNN(Data &query, int k, HashTable *cube, int M, int probes, int d, const std::function<double(const Data &, const Data &)> &metric) {
  std::vector<Neighbor> b;                              // k best neighbors
  std::vector<Neighbor> temp;                           // helper vector to reverse b
  PriorityQueue pq;                                     // priority queue size k
  auto bDist = std::numeric_limits<double>::infinity(); // k'th best distance, initialize to infinity
  Neighbor n;

  n.id = "id";
  n.dist = bDist;

  for (size_t i = 0; i < k; i++) // initialize priority queue
    pq.push(n);

  size_t index;
  auto buck = ((Hypercube *)cube)->getNeighborCandidates(query, index);
  std::unordered_map<size_t, std::vector<size_t>> vertices;

  size_t cubeSize = cube->getTableSize();
  auto table = cube->getTable();

  for (auto i = 0; i < cubeSize; i++) {
    int dist = hammingDist(index, i);
    vertices[dist].push_back(i);
  }

  int count = 0, currentProbes = 0;
  bool flag = false;

  for (int dist = 0; dist < d; dist++) {
    if (flag)
      break;
    for (const auto &index : vertices[dist]) {
      if (flag)
        break;
      if (currentProbes > probes) {
        flag = true;
        break;
      }

      auto buck = table[index];

      for (const auto &p : buck) { // for each item in bucket

        if (count > M) {
          flag = true;
          break;
        }

        // distance between candidate and query
        auto dist = metric(query, *p);

        if (dist < bDist) { // if better than k-th best distance
          n.id = p->id;
          n.dist = dist;

          if (!pq.find(n)) { // if you don't find duplicate
            pq.pop();
            pq.push(n);            // put neighbor in k
            bDist = pq.top().dist; // change k-th best distance
          }
        }
        count++;
      }
      currentProbes++;
    }
  }

  while (!pq.empty()) { // put items of queue to helper vector (descending order)
    temp.push_back(pq.top());
    pq.pop();
  }

  if (k > M) {
    // put items of queue to actual vector (ascending order)
    for (auto it = temp.rbegin(); it != temp.rbegin() + M; ++it)
      b.push_back(*it);
  } else {
    // put items of queue to actual vector (ascending order)
    for (auto it = temp.rbegin(); it != temp.rend(); ++it)
      b.push_back(*it);
  }

  return b;
}

std::vector<std::string> approximateRangeSearch(Data &query, int r, HashTable *cube, int M, int probes, int d, const std::function<double(const Data &, const Data &)> &metric) {
  std::vector<std::string> rNeighbors;

  size_t index;
  auto buck = ((Hypercube *)cube)->getNeighborCandidates(query, index);
  std::unordered_map<size_t, std::vector<size_t>> vertices;

  size_t cubeSize = cube->getTableSize();
  auto table = cube->getTable();

  for (auto i = 0; i < cubeSize; i++) {
    int dist = hammingDist(index, i);
    vertices[dist].push_back(i);
  }

  int count = 0, currentProbes = 0;

  for (int dist = 0; dist < d; dist++) {
    for (const auto &index : vertices[dist]) {
      if (currentProbes > probes) {
        return rNeighbors;
      }

      auto buck = table[index];

      for (const auto &p : buck) { // for each item in bucket

        if (count > M) {
          return rNeighbors;
        }

        // distance between candidate and query
        auto dist = metric(query, *p);

        if (dist < r) { // if smaller than radius
          rNeighbors.push_back(p->id);
        }
        count++;
      }
      currentProbes++;
    }
  }

  return rNeighbors;
}
