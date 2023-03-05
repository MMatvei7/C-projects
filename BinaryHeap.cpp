/*#include <iostream>

class BinaryHeap {
 private:
  int64_t max_size_ = 0;
  int64_t heap_size_ = 0;
  int64_t* arr_;

 public:
  int64_t Parent(int64_t);
  int64_t LeftChild(int64_t);
  int64_t RightChild(int64_t);
  int64_t SiftDown(int64_t*, int64_t, int64_t);
  int64_t SiftUp(int64_t*, int64_t, int64_t);
  int64_t MaxHeapInsert(int64_t);
  int64_t MaxHeapExtractMax();
  int64_t MaxHeapDelete(int64_t i);
  void SetMaxSize(int64_t size) {
    max_size_ = size;
  }
  void SetArray(int64_t* arr) {
    arr_ = arr;
  }
  void SetHeapSize(int64_t size) {
    heap_size_ = size;
  }
  int64_t GetHeapSize() {
    return heap_size_;
  }
  int64_t* GetArray() {
    return arr_;
  }
  int64_t GetMaxSize() {
    return max_size_;
  }
};

// get parent
int64_t BinaryHeap::Parent(int64_t idx) {
  return (idx - 1) / 2;
}

// get Left child of idx
int64_t BinaryHeap::LeftChild(int64_t idx) {
  return 2 * idx + 1;
}
// get Right child of idx
int64_t BinaryHeap::RightChild(int64_t idx) {
  return 2 * idx + 2;
}
// function to sift down element
int64_t BinaryHeap::SiftDown(int64_t* a, int64_t sift_idx, int64_t size) {
  int64_t left = LeftChild(sift_idx);
  int64_t right = RightChild(sift_idx);
  // we need to check if the heap is correct
  int64_t largest = sift_idx;
  if (left < size && a[largest] < a[left]) {
    largest = left;
  }
  if (right < size && a[largest] < a[right]) {
    largest = right;
  }
  // if one of children is bigger we need to swap parent and this child
  if (largest != sift_idx && sift_idx < size) {
    std::swap(a[sift_idx], a[largest]);
    return SiftDown(a, largest, size);
  }
  return sift_idx;
}
// sift up element
int64_t BinaryHeap::SiftUp(int64_t* a, int64_t sift_idx, int64_t size) {
  int64_t parent = Parent(sift_idx);
  if (a[parent] < a[sift_idx]) {
    std::swap(a[parent], a[sift_idx]);
    return SiftUp(a, parent, size);
  }
  return sift_idx;
}

// insert element
int64_t BinaryHeap::MaxHeapInsert(int64_t x) {
  ++heap_size_;
  arr_[heap_size_ - 1] = x;
  return SiftUp(arr_, heap_size_ - 1, heap_size_);
}

// extract max element
int64_t BinaryHeap::MaxHeapExtractMax() {
  std::swap(arr_[0], arr_[heap_size_ - 1]);
  --heap_size_;
  return SiftDown(arr_, 0, heap_size_);
}

// delete elem by index i
int64_t BinaryHeap::MaxHeapDelete(int64_t i) {
  auto del_el = arr_[i];
  std::swap(arr_[heap_size_ - 1], arr_[i]);
  --heap_size_;
  SiftUp(arr_, i, heap_size_);
  SiftDown(arr_, i, heap_size_);
  return del_el;
}

int main() {
  BinaryHeap heap;
  int64_t n;
  int64_t m;
  std::cin >> n >> m;
  heap.SetMaxSize(n);
  auto arr = new int64_t[n];
  heap.SetArray(arr);
  int64_t type;
  int64_t value;
  for (int64_t i = 0; i < m; ++i) {
    std::cin >> type;
    if (type == 1) {
      if (heap.GetHeapSize() > 1) {
        std::cout << heap.MaxHeapExtractMax() + 1 << " " << heap.GetArray()[heap.GetHeapSize()] << '\n';
      } else if (heap.GetHeapSize() == 1) {
        std::cout << 0 << " " << heap.GetArray()[0] << '\n';
        heap.SetHeapSize(heap.GetHeapSize() - 1);
      } else {
        std::cout << -1 << '\n';
      }
    }
    if (type == 2) {
      std::cin >> value;
      if (heap.GetHeapSize() >= heap.GetMaxSize()) {
        std::cout << -1 << '\n';
      } else {
        std::cout << heap.MaxHeapInsert(value) + 1 << '\n';
      }
    }
    if (type == 3) {
      std::cin >> value;
      if (heap.GetHeapSize() <= value - 1 || heap.GetHeapSize() < 1) {
        std::cout << -1 << '\n';
      } else if (value < 1) {
        std::cout << -1 << '\n';
      } else {
        std::cout << heap.MaxHeapDelete(value - 1) << '\n';
      }
    }
  }
  for (int64_t i = 0; i < heap.GetHeapSize(); ++i) {
    std::cout << heap.GetArray()[i] << " ";
  }
  std::cout << '\n';
  delete[] arr;
  return 0;
}
*/
