#ifndef TEMPLATE_ARRAY
#define TEMPLATE_ARRAY
#define ARRAY_TRAITS_IMPLEMENTED
#include <stdexcept>

class ArrayOutOfRange : public std::out_of_range {
 public:
  ArrayOutOfRange() : std::out_of_range("ArrayOutOfRange") {
  }
};

template <class T, size_t N>
class Array {
 public:
  T array[N];
  const T& operator[](size_t) const;
  T& operator[](size_t);
  T& At(size_t);
  const T& At(size_t) const;
  T& Front();
  const T& Front() const;
  T& Back();
  const T& Back() const;
  T* Data();
  const T* Data() const;
  size_t Size() const;
  bool Empty() const;
  void Fill(const T&);
  void Swap(Array<T, N>&);
};

template <class T, size_t N>
const T& Array<T, N>::operator[](size_t i) const {
  return *(array + i);
}

template <class T, size_t N>
T& Array<T, N>::operator[](size_t i) {
  return *(array + i);
}

template <class T, size_t N>
T& Array<T, N>::At(size_t idx) {
  if (idx >= N) {
    throw ArrayOutOfRange{};
  }
  return array[idx];
}

template <class T, size_t N>
const T& Array<T, N>::At(size_t idx) const {
  if (idx >= N) {
    throw ArrayOutOfRange{};
  }
  return array[idx];
}

template <class T, size_t N>
T& Array<T, N>::Front() {
  return array[0];
}

template <class T, size_t N>
const T& Array<T, N>::Front() const {
  return array[0];
}

template <class T, size_t N>
T& Array<T, N>::Back() {
  return array[N - 1];
}

template <class T, size_t N>
const T& Array<T, N>::Back() const {
  return array[N - 1];
}

template <class T, size_t N>
T* Array<T, N>::Data() {
  return array;
}

template <class T, size_t N>
const T* Array<T, N>::Data() const {
  return array;
}

template <class T, size_t N>
size_t Array<T, N>::Size() const {
  return N;
}

template <class T, size_t N>
bool Array<T, N>::Empty() const {
  return N > 0 ? false : true;
}

template <class T, size_t N>
void Array<T, N>::Fill(const T& value) {
  for (size_t i = 0; i < N; ++i) {
    array[i] = value;
  }
}

template <class T, size_t N>
void Array<T, N>::Swap(Array<T, N>& other) {
  for (size_t i = 0; i < N; ++i) {
    std::swap(array[i], other[i]);
  }
}
// additional part
template <class T, size_t N>
size_t GetSize(T (&)[N]) {
  return N;
}

template <class T>
size_t GetSize(T) {
  return 0;
}

template <class T>
size_t GetRank(T) {
  return 0;
}

template <class T, size_t N>
size_t GetRank(T (&arr)[N]) {
  return GetRank(arr[0]) + 1;
}

template <class T>
size_t GetNumElements(T) {
  return 1;
}

template <class T, size_t N>
size_t GetNumElements(T (&arr)[N]) {
  return N * GetNumElements(arr[0]);
}

#endif
