/*#include <iostream>

static int64_t count_inversions = 0;

void Merge(int32_t* merge_array, int32_t* a, int32_t* b, int32_t size_a, int32_t size_b) {
  // we need to keep pointers on a and b elements to sort and count inversions
  int32_t pointer_a = 0;
  int32_t pointer_b = 0;
  int32_t merge_array_idx = 0;
  while (pointer_a < size_a && pointer_b < size_b) {
    if (a[pointer_a] <= b[pointer_b]) {
      merge_array[merge_array_idx] = a[pointer_a];
      ++pointer_a;
      count_inversions += pointer_b;
    } else {
      merge_array[merge_array_idx] = b[pointer_b];
      ++pointer_b;
    }
    ++merge_array_idx;
  }
  // we need to add all the rest elements from a or b
  while (pointer_a < size_a) {
    merge_array[merge_array_idx] = a[pointer_a];
    ++pointer_a;
    ++merge_array_idx;
    count_inversions += pointer_b;
  }
  while (pointer_b < size_b) {
    merge_array[merge_array_idx] = b[pointer_b];
    ++pointer_b;
    ++merge_array_idx;
  }
  for (int32_t i = 0; i < size_a; ++i) {
    a[i] = merge_array[i];
  }
  for (int32_t i = 0; i < size_b; ++i) {
    b[i] = merge_array[i + size_a];
  }
}
void MergeSort(int32_t* merge_array, int32_t* a, int32_t size) {
  // if we have 1 or 0 elements we then this array is already sorted
  if (size == 1 || size == 0) {
    return;
  }
  int32_t half_size_of_current_array = size / 2;
  // we need to divide array on 2 parts and then merge them after sort of each part
  // create left half of current array
  auto* left_half = new int[half_size_of_current_array];
  for (int32_t i = 0; i < half_size_of_current_array; ++i) {
    left_half[i] = a[i];
  }
  // create right half of current array
  auto* right_half = new int[half_size_of_current_array + 1];
  for (int32_t i = half_size_of_current_array; i < size; ++i) {
    right_half[i - half_size_of_current_array] = a[i];
  }
  // need to understand if size / 2 is odd or even (not need actually)
  MergeSort(merge_array, left_half, half_size_of_current_array);
  MergeSort(merge_array + half_size_of_current_array, right_half, size - half_size_of_current_array);
  Merge(merge_array, left_half, right_half, half_size_of_current_array, size - half_size_of_current_array);
  for (int32_t i = 0; i < size; ++i) {
    a[i] = merge_array[i];
  }
  delete[] left_half;
  delete[] right_half;
}
int main() {
  int32_t n;
  std::cin >> n;
  auto* a = new int32_t[n];
  auto* merge_array = new int32_t[n + 1];
  for (int32_t i = 0; i <= n; ++i) {
    merge_array[i] = 0;
  }
  for (int i = 0; i < n; ++i) {
    std::cin >> a[i];
  }
  MergeSort(merge_array, a, n);
  std::cout << count_inversions << "\n";
  delete[] a;
  delete[] merge_array;
  return 0;
}
*/
