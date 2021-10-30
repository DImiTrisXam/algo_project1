#include "lshSearch.hpp"
#include "../utilities/HashTable.hpp"
#include "../utilities/metrics.hpp"
#include "../utilities/utilities.hpp"
#include <iostream>
#include <limits>
#include <queue>

void trueDistance() {
}

std::vector<double> approximateKNN(Data &query, int k, HashTable **tables, int L) {
  std::vector<double> b;                                  // k best neighbors
  std::priority_queue<double> pq;                         // priority queue size k
  double bDist = std::numeric_limits<double>::infinity(); // k'th best distance, initialize to infinity

  for (size_t i = 0; i < k; i++) // initialize priority queue
    pq.push(bDist);

  for (size_t i = 0; i < L; i++) { // for every table
    Bucket buck = tables[i]->getNeighbourCandidates(query);
    // Neighbor n;

    for (const auto &p : buck) { // for each item in bucket
      // distance between candidate and query
      double dist = euclidianDist(query.value, p->value);

      if (dist < bDist) { // if better than k-th best distance
        // n.id = p->key;
        // n.dist = dist;

        pq.pop();
        pq.push(dist);    // put neighbour in k
        bDist = pq.top(); // change k-th best distance
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