#include "test.hpp"

void testDiscFrechet() {
  std::vector<float> vec1, vec2;
  std::vector<int> tVec;
  int size = 5;

  std::default_random_engine generator(730);

  std::uniform_real_distribution<float> distribution(0, 100);

  for (auto i = 0; i < size; i++) {
    vec1.push_back(distribution(generator));
    vec2.push_back(distribution(generator));
    tVec.push_back(i + 1);
  }

  auto c1 = new Curve(vec1, tVec, "id1");
  auto c2 = new Curve(vec2, tVec, "id2");

  double dist = discreteFrechetDist(*c1, *c2);
  double res = 56.05622100830078;

  CU_ASSERT(dist == res);

  return;
}
