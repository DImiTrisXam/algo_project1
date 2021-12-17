#include "test.hpp"

void testHashTable() {
  std::vector<float> vec1, vec2, vec3, vec4, vec5;
  std::vector<int> tVec;
  int size = 5;
  HashTable table(14, 2, 5, 10);

  std::default_random_engine generator(730);

  std::uniform_real_distribution<float> distribution(0, 100);

  for (auto i = 0; i < size; i++) {
    vec1.push_back(distribution(generator));
    vec2.push_back(distribution(generator));
    vec3.push_back(distribution(generator));
    vec4.push_back(distribution(generator));
    vec5.push_back(distribution(generator));
    tVec.push_back(i + 1);
  }

  auto c1 = new Curve(vec1, tVec, "id1");
  auto c2 = new Curve(vec2, tVec, "id2");
  auto c3 = new Curve(vec3, tVec, "id3");
  auto c4 = new Curve(vec4, tVec, "id4");
  auto c5 = new Curve(vec4, tVec, "id5");

  table.add(c1->vec, c1->tVec, c1->key, c1->id);
  table.add(c2->vec, c2->tVec, c2->key, c2->id);
  table.add(c3->vec, c3->tVec, c3->key, c3->id);
  table.add(c4->vec, c4->tVec, c4->key, c4->id);
  table.add(c5->vec, c5->tVec, c5->key, c5->id);

  CU_ASSERT(table.getCurrentSize() == 5);

  return;
}
