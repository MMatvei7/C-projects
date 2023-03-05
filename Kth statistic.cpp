/*#ifndef TEMPLATE_H
#define TEMPLATE_H

template <class T>
int Partition(T* begin, int p, T* end) {
  int size = end - begin - 1;
  int i = p - 1;
  for (int j = p; j < size; ++j) {
    if (!(begin[size] < begin[j])) {
      ++i;
      auto tmp = begin[i];
      begin[i] = begin[j];
      begin[j] = tmp;
    }
  }
  auto tmp = begin[i + 1];
  begin[i + 1] = begin[size];
  begin[size] = tmp;
  return i + 1;
}

template <class T>
void KthElement(T* begin, T* k, T* end) {
  int size = end - begin;
  if (size < 2) {
    return;
  }
  // we need to divide array on 2 parts:
  // to the left all elements that are smaller then the chosen one
  // to the right all elements that are greater then the chosen one
  // for this purpose we need to store inversions: how many elements smaller than chosen
  // Partition(begin, 0, end);
  // we need to divide the other of this 2 parts which contains k
  int chosen_one = Partition(begin, 0, end);
  if (k - begin == chosen_one) {
    return;
  } else if (k - begin < chosen_one) {
    KthElement(begin, k, begin + chosen_one);
  } else {
    KthElement(begin + chosen_one + 1, k, end);
  }
}

#endif
*/
