#include "lshSearch.hpp"
#include "../utilities/metrics.hpp"
#include "../utilities/utilities.hpp"
#include <iostream>
#include <limits>
#include <queue>

struct CompareDist {
  bool operator()(Neighbor const &n1, Neighbor const &n2) {
    return n1.dist < n2.dist;
  }
};

void trueDistance() {
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

    for (const auto &p : buck) { // for each item in bucket
      // distance between candidate and query
      double dist = euclidianDist(query.vec, p->vec);

      if (dist < bDist) { // if better than k-th best distance
        n.id = p->id;
        n.dist = dist;

        pq.pop();
        pq.push(n);            // put neighbour in k
        bDist = pq.top().dist; // change k-th best distance
      }
    }
  }

  while (!pq.empty()) { // put items of queue to vector
    b.push_back(pq.top());
    pq.pop();
  }

  return b;
}

void approximateRangeSearch() {
}