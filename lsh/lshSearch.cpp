#include "lshSearch.hpp"
#include "../utilities/metrics.hpp"
#include <iostream>
#include <limits>
#include <queue>

struct CompareDist {
  bool operator()(Neighbor const &n1, Neighbor const &n2) {
    return n1.dist < n2.dist;
  }
};

std::vector<double> trueDistanceN(Data &query, int k, HashTable **tables, int L) {
  std::vector<double> b;                                  // k best neighbors
  std::priority_queue<double> pq;                         // priority queue size k
  double bDist = std::numeric_limits<double>::infinity(); // k'th best distance, initialize to infinity

  for (size_t i = 0; i < k; i++) // initialize priority queue
    pq.push(bDist);

  for (size_t i = 0; i < L; i++) { // for every table
    int size = tables[i]->getTableSize();
    std::list<Data *> *table = tables[i]->getTable();

    for (size_t j = 0; j < size; j++) {
      for (const Data *p : table[i]) { // for each item in bucket
        // distance between candidate and query
        double dist = euclidianDist(query.vec, p->vec);

        if (dist < bDist) { // if better than k-th best distance
          pq.pop();
          pq.push(dist);    // put neighbor in k
          bDist = pq.top(); // change k-th best distance
        }
      }
    }
  }

  while (!pq.empty()) { // put items of queue to vector
    b.push_back(pq.top());
    pq.pop();
  }

  // std::cout << '\n';

  return b;
}

std::vector<Neighbor> approximateKNN(Data &query, int k, HashTable **tables, int L) {
  std::vector<Neighbor> b;                                              // k best neighbors
  std::priority_queue<Neighbor, std::vector<Neighbor>, CompareDist> pq; // priority queue size k
  double bDist = std::numeric_limits<double>::infinity();               // k'th best distance, initialize to infinity
  Neighbor n;

  n.id = "id";
  n.dist = bDist;

  for (size_t i = 0; i < k; i++) // initialize priority queue
    pq.push(n);

  for (size_t i = 0; i < L; i++) { // for every table
    std::list<Data *> buck = tables[i]->getNeighborCandidates(query);

    for (const Data *p : buck) { // for each item in bucket
      // distance between candidate and query
      double dist = euclidianDist(query.vec, p->vec);

      // std::cout << dist << ' ';

      if (dist < bDist) { // if better than k-th best distance
        n.id = p->id;
        n.dist = dist;

        pq.pop();
        pq.push(n);            // put neighbor in k
        bDist = pq.top().dist; // change k-th best distance
      }
    }
    // std::cout << '\n';
  }

  while (!pq.empty()) { // put items of queue to vector
    // std::cout << pq.top().dist << ' ';
    b.push_back(pq.top());
    pq.pop();
  }

  // std::cout << '\n';

  return b;
}

std::vector<std::string> approximateRangeSearch(Data &query, int r, HashTable **tables, int L) {
  std::vector<std::string> rNeighbors;

  for (size_t i = 0; i < L; i++) { // for every table
    std::list<Data *> buck = tables[i]->getNeighborCandidates(query);

    for (const Data *p : buck) { // for each item in bucket
      // distance between candidate and query
      double dist = euclidianDist(query.vec, p->vec);

      if (dist < r) { // if smaller than radius
        rNeighbors.push_back(p->id);
      }
    }
  }

  return rNeighbors;
}