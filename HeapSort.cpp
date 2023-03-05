/*#ifndef TEMPLATE_H
#define TEMPLATE_H

int LeftChild(int idx) {
  return 2 * idx + 1;
}
// get Right child of idx
int RightChild(int idx) {
  return 2 * idx + 2;
}
// function to sift element
template <class T>
void SiftDown(T* begin, T* end, int sift_idx, int size) {
  int left = LeftChild(sift_idx);
  int right = RightChild(sift_idx);
  // we need to check if the heap is correct
  int largest = sift_idx;
  if (left < size && begin[largest] < begin[left]) {
    largest = left;
  }
  if (right < size && begin[largest] < begin[right]) {
    largest = right;
  }
  // if one of children is bigger we need to swap parent and this child
  if (largest != sift_idx && sift_idx < size) {
    auto tmp = begin[sift_idx];
    begin[sift_idx] = begin[largest];
    begin[largest] = tmp;
    // std::swap(begin[sift_idx], begin[largest]);
    SiftDown(begin, end, largest, size);
  }
}
// Build heap using sifting
template <class T>
void BuildHeap(T* begin, T* end) {
  int size = end - begin;
  for (int i = (size - 1) / 2; i >= 0; --i) {
    SiftDown(begin, end, i, size);
  }
}
template <class T>
void Sort(T* begin, T* end) {
  int size = end - begin;
  BuildHeap(begin, end);
  for (int i = size - 1; i >= 1; --i) {
    // drop out the biggest element and then sift the last one through size - 1 heap
    auto tmp = begin[0];
    begin[0] = begin[i];
    begin[i] = tmp;
    // std::swap(begin[0], begin[i]);
    --size;
    SiftDown(begin, end, static_cast<int>(0), size);
  }
}

#endif
*/
