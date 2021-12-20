#include "lshSearch.hpp"
#include "../Fred/include/curve.hpp"
#include "../Fred/include/frechet.hpp"
#include "../Fred/include/point.hpp"
#include "../utilities/metrics.hpp"
#include <cstring>
#include <iostream>
#include <limits>
#include <typeinfo>

void snapQueryToGrid(Curve_ &c, Grid **grids, int i, std::string &metric) {
  if (metric.compare("discrete") == 0) {
    grids[i]->snapTo2DGrid(c); // snap curve
    c.collapseGridToVector();
    c.padding();
  } else {
    float averageVariation = 0;

    for (auto i = 0; i < c.vec.size() - 1; i++)
      averageVariation += abs(c.vec[i + 1] - c.vec[i]);

    averageVariation = averageVariation / c.vec.size();
    float epsilon = (averageVariation * 160) / 100;

    c.filter(epsilon);
    grids[i]->snapTo1DGrid(c); // snap curve
    c.getMinimaMaxima();
    c.padding();
  }
}

std::vector<Neighbor> trueDistanceN(Data &query, int k, HashTable **tables, int L, const std::function<double(const Data &, const Data &)> &metric) {
  std::vector<Neighbor> b;                              // k best neighbors
  std::vector<Neighbor> temp;                           // helper vector to reverse b
  PriorityQueue pq;                                     // priority queue size k
  auto bDist = std::numeric_limits<double>::infinity(); // k'th best distance, initialize to infinity
  Neighbor n;

  n.id = "id";
  n.dist = bDist;

  for (size_t i = 0; i < k; i++) // initialize priority queue
    pq.push(n);

  for (auto i = 0; i < L; i++) { // for every table
    auto size = tables[i]->getTableSize();
    auto table = tables[i]->getTable();

    for (auto j = 0; j < size; j++) {
      for (const auto &p : table[j]) { // for each item in bucket
        // distance between candidate and query
        double dist;
        if (!metric) {
          Points qPoints(1);
          Points pPoints(1);

          for (auto q : query.vec) {
            Point p = Point(1);
            p.set(0, q);
            qPoints.add(p);
          }

          for (auto &_p : p->vec) {
            Point pp = Point(1);
            pp.set(0, _p);
            pPoints.add(pp);
          }

          Curve qCurve = Curve(qPoints);
          Curve pCurve = Curve(pPoints);
          dist = Frechet::Continuous::distance(qCurve, pCurve).value;

        } else
          dist = metric(query, *p);

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

  while (!pq.empty()) { // put items of queue to helper vector (descending order)
    temp.push_back(pq.top());
    pq.pop();
  }
  // put items of queue to actual vector (ascending order)
  for (auto it = temp.rbegin(); it != temp.rend(); ++it)
    b.push_back(*it);

  return b;
}

std::vector<Neighbor> approximateKNN(Data &query, int k, HashTable **tables, Grid **grids, int L, std::string &discrete, const std::function<double(const Data &, const Data &)> &metric) {
  std::vector<Neighbor> b;                              // k best neighbors
  std::vector<Neighbor> temp;                           // helper vector to reverse b
  PriorityQueue pq;                                     // priority queue size k
  auto bDist = std::numeric_limits<double>::infinity(); // k'th best distance, initialize to infinity
  Neighbor n;

  n.id = "id";
  n.dist = bDist;

  for (size_t i = 0; i < k; i++) // initialize priority queue
    pq.push(n);

  auto type1 = typeid(query).name();
  auto type2 = typeid(Curve_).name();

  for (auto i = 0; i < L; i++) {     // for every table
    if (strcmp(type1, type2) == 0) { // if curve
      auto c = (Curve_ &)query;
      snapQueryToGrid(c, grids, i, discrete);
    }

    auto buck = tables[i]->getNeighborCandidates(query);

    for (const auto &p : buck) { // for each item in bucket
                                 // distance between candidate and query
      double dist;
      if (discrete.compare("continuous") == 0) {
        Points qPoints(1);
        Points pPoints(1);
        for (auto q : query.vec) {
          Point p = Point(1);
          p.set(0, q);
          qPoints.add(p);
        }
        for (auto _p : p->vec) {
          Point pp = Point(1);
          pp.set(0, _p);
          pPoints.add(pp);
        }
        Curve qCurve = Curve(qPoints);
        Curve pCurve = Curve(pPoints);
        dist = Frechet::Continuous::distance(qCurve, pCurve).value;

      } else
        dist = metric(query, *p);

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

std::vector<std::string> approximateRangeSearch(Data &query, double r, HashTable **tables, Grid **grids, int L, const std::function<double(const Data &, const Data &)> &metric) {
  std::vector<std::string> rNeighbors;

  auto type1 = typeid(query).name();
  auto type2 = typeid(Curve_).name();
  std::string discrete = "discrete";

  for (size_t i = 0; i < L; i++) {   // for every table
    if (strcmp(type1, type2) == 0) { // if curve
      auto c = (Curve_ &)query;
      snapQueryToGrid(c, grids, i, discrete);
    }

    auto buck = tables[i]->getNeighborCandidates(query);

    for (const auto &p : buck) { // for each item in bucket
      // distance between candidate and query
      double dist = metric(query, *p);

      if (dist < r) { // if smaller than radius
        rNeighbors.push_back(p->id);
      }
    }
  }

  return rNeighbors;
}