#include "cubeSearch.hpp"
#include "../utilities/metrics.hpp"
#include "../utilities/PriorityQueue.hpp"
#include <iostream>
#include <limits>

std::vector<Neighbor> trueDistanceN(Data &query, int k, HashTable *cube) {
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
        auto dist = euclidianDist(query.vec, p->vec);

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
}

std::vector<Neighbor> approximateKNN(Data &query, int k, HashTable *cube) {
  
}

std::vector<std::string> approximateRangeSearch(Data &query, int r, HashTable *cube) {
}