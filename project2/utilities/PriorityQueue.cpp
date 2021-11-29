#include "PriorityQueue.hpp"
#include <iostream>
#include <stdexcept>
#include <string>

void PriorityQueue::swap(Neighbor &a, Neighbor &b) {
  Neighbor temp;

  temp.dist = a.dist;
  temp.id = a.id;

  a.dist = b.dist;
  a.id = b.id;
  b.dist = temp.dist;
  b.id = temp.id;
}

int PriorityQueue::parent(int i) {
  return (i - 1) / 2;
}

int PriorityQueue::leftChild(int i) {
  return (2 * i + 1);
}

int PriorityQueue::rightChild(int i) {
  return (2 * i + 2);
}

// Recursive heapify-down algorithm.
// The node at index `i` and its two direct children
// violates the heap property
void PriorityQueue::heapify_down(int i) {
  int left = leftChild(i);
  int right = rightChild(i);

  int largest = i;

  // compare `heap[i]` with its left and right child
  // and find the largest value
  if (left < size() && heap[left].dist > heap[i].dist)
    largest = left;

  if (right < size() && heap[right].dist > heap[largest].dist)
    largest = right;

  // swap with a child having greater value and
  // call heapify-down on the child
  if (largest != i) {
    swap(heap[i], heap[largest]);
    heapify_down(largest);
  }
}

// Recursive heapify-up algorithm
void PriorityQueue::heapify_up(int i) {
  // check if the node at index `i` and its parent violate the heap property
  if (i && heap[parent(i)].dist < heap[i].dist) {
    // swap the two if heap property is violated
    std::swap(heap[i], heap[parent(i)]);

    // call heapify-up on the parent
    heapify_up(parent(i));
  }
}

unsigned int PriorityQueue::size() {
  return heap.size();
}

bool PriorityQueue::empty() {
  return size() == 0;
}

bool PriorityQueue::find(Neighbor n) {
  bool found = false;

  for (auto &i : heap) {
    if (i.id.compare(n.id) == 0 && i.dist == n.dist) {
      found = true;
    }
  }

  return found;
}

void PriorityQueue::push(Neighbor entry) {
  heap.push_back(entry);

  // get element index and call heapify-up procedure
  int index = size() - 1;
  heapify_up(index);
}

void PriorityQueue::pop() {
  try {
    if (size() == 0) { // if heap is empty
      throw std::out_of_range("Heap is empty");
    }

    // replace the root of the heap with the last element of the vector
    heap[0] = heap.back();
    heap.pop_back();

    // call heapify-down on the root node
    heapify_down(0);
  } catch (const std::out_of_range &oor) { // catch and print the exception
    std::cout << std::endl
              << oor.what();
  }
}

Neighbor PriorityQueue::top() {
  try {
    if (size() == 0) // if heap is empty
      throw std::out_of_range("Heap is empty");

    return heap[0];                        // otherwise, return the top (first) element
  } catch (const std::out_of_range &oor) { // catch and print the exception
    std::cout << std::endl
              << oor.what();
  }
}