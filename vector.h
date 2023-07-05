#ifndef VECTOR_H_
#define VECTOR_H_
#define VECTOR_MEMORY_IMPLEMENTED
#include <cstddef>
#include <type_traits>
#include <iterator>
#include <initializer_list>
#include <algorithm>
#include <memory>
#include <exception>

template <typename T>
class Vector {
 private:
  T* data_;
  size_t size_;
  size_t capacity_;

  T* Allocate(size_t);
  void Reallocate(size_t);
  size_t Recommend(size_t) const noexcept;

 public:
  Vector();
  explicit Vector(size_t);
  Vector(size_t, const T&);
  template <class Iterator, class = std::enable_if_t<std::is_base_of_v<
                                std::forward_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>>>
  Vector(Iterator first, Iterator last) {
    size_ = std::distance(first, last);
    if (size_ > 0) {
      data_ = Allocate(size_);
      try {
        std::uninitialized_copy(first, last, data_);
      } catch (...) {
        operator delete(data_);
        size_ = 0;
        throw;
      }
    } else {
      data_ = nullptr;
    }
    capacity_ = size_;
  }
  Vector(std::initializer_list<T>);
  Vector(const Vector&);
  Vector(Vector&&) noexcept;
  Vector& operator=(const Vector&);
  Vector& operator=(Vector&&) noexcept;
  size_t Size() const noexcept;
  size_t Capacity() const noexcept;
  bool Empty() const noexcept;
  T& operator[](size_t) noexcept;
  const T& operator[](size_t) const noexcept;
  T& At(size_t);
  const T& At(size_t) const;
  T& Front() noexcept;
  const T& Front() const noexcept;
  T& Back() noexcept;
  const T& Back() const noexcept;
  T* Data() noexcept;
  const T* Data() const noexcept;
  void Swap(Vector&);
  void Resize(size_t);
  void Resize(size_t, const T&);
  void Reserve(size_t);
  void ShrinkToFit();
  void Clear() noexcept;
  void PushBack(const T&);
  void PushBack(T&&);
  void PopBack();
  template <typename... Args>
  void EmplaceBack(Args&&...);
  bool operator<(const Vector&) const;
  using ValueType = T;
  using Pointer = T*;
  using ConstPointer = const T*;
  using Reference = T&;
  using ConstReference = const T&;
  using SizeType = size_t;
  using Iterator = T*;
  using ConstIterator = const T*;
  using ReverseIterator = std::reverse_iterator<Iterator>;
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;
  // NOLINTNEXTLINE
  Iterator begin() noexcept {
    return data_;
  }
  // NOLINTNEXTLINE
  ConstIterator begin() const noexcept {
    return data_;
  }
  // NOLINTNEXTLINE
  Iterator end() noexcept {
    return (data_ + size_);
  }
  // NOLINTNEXTLINE
  ConstIterator end() const noexcept {
    return (data_ + size_);
  }
  // NOLINTNEXTLINE
  ConstIterator cbegin() const noexcept {
    return data_;
  }
  // NOLINTNEXTLINE
  ConstIterator cend() noexcept {
    return (data_ + size_);
  }
  // NOLINTNEXTLINE
  ReverseIterator rbegin() noexcept {
    return std::reverse_iterator(data_ + size_);
  }
  // NOLINTNEXTLINE
  ReverseIterator rend() noexcept {
    return std::reverse_iterator(data_);
  }
  // NOLINTNEXTLINE
  ConstReverseIterator rbegin() const noexcept {
    return std::reverse_iterator(data_ + size_);
  }
  // NOLINTNEXTLINE
  ConstReverseIterator rend() const noexcept {
    return std::reverse_iterator(data_);
  }
  // NOLINTNEXTLINE
  ConstReverseIterator crbegin() const noexcept {
    return std::reverse_iterator(data_ + size_);
  }
  // NOLINTNEXTLINE
  ConstReverseIterator crend() const noexcept {
    return std::reverse_iterator(data_);
  }
  ~Vector() noexcept;
};

template <typename T>
T* Vector<T>::Allocate(size_t size) {
  return reinterpret_cast<T*>(operator new(size * sizeof(T)));
}

template <typename T>
void Vector<T>::Reallocate(size_t new_capacity) {
  T* new_data = Allocate(new_capacity);
  try {
    std::uninitialized_move(data_, data_ + size_, new_data);
  } catch (...) {
    operator delete(new_data);
    throw;
  }
  capacity_ = new_capacity;
  for (size_t i = 0; i < size_; ++i) {
    data_[i].~T();
  }
  operator delete(data_);
  data_ = new_data;
}

template <typename T>
size_t Vector<T>::Recommend(size_t new_size) const noexcept {
  return std::max(2 * capacity_, new_size);
}

template <typename T>
void Vector<T>::Swap(Vector& other) {
  std::swap(capacity_, other.capacity_);
  std::swap(size_, other.size_);
  std::swap(data_, other.data_);
}

template <typename T>
Vector<T>::Vector() : data_(nullptr), size_(0), capacity_(0) {
}

template <typename T>
Vector<T>::Vector(size_t size) {
  if (size > 0) {
    data_ = Allocate(size);
    try {
      std::uninitialized_default_construct(data_, data_ + size);
    } catch (...) {
      operator delete(data_);
      throw;
    }
    capacity_ = size_ = size;
  } else {
    data_ = nullptr;
    size_ = capacity_ = 0;
  }
}

template <typename T>
Vector<T>::Vector(size_t size, const T& value) {
  if (size > 0) {
    data_ = Allocate(size);
    try {
      std::uninitialized_fill(data_, data_ + size, value);
    } catch (...) {
      operator delete(data_);
      throw;
    }
    capacity_ = size_ = size;
  } else {
    data_ = nullptr;
    size_ = capacity_ = 0;
  }
}

template <typename T>
Vector<T>::Vector(std::initializer_list<T> list) : Vector(list.begin(), list.end()) {
}

template <typename T>
Vector<T>::Vector(const Vector& other) {
  if (other.size_ > 0) {
    data_ = Allocate(other.size_);
    try {
      std::uninitialized_copy(other.data_, other.data_ + other.size_, data_);
    } catch (...) {
      operator delete(data_);
      throw;
    }
  } else {
    data_ = nullptr;
  }
  size_ = capacity_ = other.size_;
}

template <typename T>
Vector<T>::Vector(Vector&& other) noexcept : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
  other.data_ = nullptr;
  other.size_ = other.capacity_ = 0;
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
  if (this == &other) {
    return *this;
  }

  Vector tmp(other);
  Swap(tmp);
  return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  Vector tmp(std::move(other));
  Swap(tmp);
  return *this;
}

template <typename T>
size_t Vector<T>::Size() const noexcept {
  return size_;
}

template <typename T>
size_t Vector<T>::Capacity() const noexcept {
  return capacity_;
}

template <typename T>
bool Vector<T>::Empty() const noexcept {
  return size_ == 0;
}

template <typename T>
T& Vector<T>::operator[](size_t index) noexcept {
  return *(data_ + index);
}

template <typename T>
const T& Vector<T>::operator[](size_t index) const noexcept {
  return *(data_ + index);
}

template <typename T>
T& Vector<T>::At(size_t index) {
  if (index >= size_) {
    throw std::out_of_range("Index out of range");
  }
  return *(data_ + index);
}

template <typename T>
const T& Vector<T>::At(size_t index) const {
  if (index >= size_) {
    throw std::out_of_range("Index out of range");
  }
  return *(data_ + index);
}

template <typename T>
T& Vector<T>::Front() noexcept {
  return *data_;
}

template <typename T>
const T& Vector<T>::Front() const noexcept {
  return *data_;
}

template <typename T>
T& Vector<T>::Back() noexcept {
  return *(data_ + size_ - 1);
}

template <typename T>
const T& Vector<T>::Back() const noexcept {
  return *(data_ + size_ - 1);
}

template <typename T>
T* Vector<T>::Data() noexcept {
  return data_;
}

template <typename T>
const T* Vector<T>::Data() const noexcept {
  return data_;
}

template <typename T>
void Vector<T>::Reserve(size_t new_capacity) {
  if (new_capacity > capacity_) {
    Reallocate(new_capacity);
  }
}

template <typename T>
void Vector<T>::Resize(size_t new_size) {
  if (new_size <= size_) {
    std::destroy(data_ + new_size, data_ + size_);
  } else if (new_size <= capacity_) {
    try {
      std::uninitialized_default_construct(data_ + size_, data_ + new_size);
    } catch (...) {
      throw;
    }
  } else {
    T* new_data = Allocate(new_size);

    try {
      std::uninitialized_move(data_, data_ + size_, new_data);
    } catch (...) {
      operator delete(new_data);
      throw;
    }

    try {
      std::uninitialized_default_construct(new_data + size_, new_data + new_size);
    } catch (...) {
      for (size_t i = 0; i < size_; ++i) {
        (new_data + i)->~T();
      }
      operator delete(new_data);
      throw;
    }

    for (size_t i = 0; i < size_; ++i) {
      data_[i].~T();
    }
    operator delete(data_);
    data_ = new_data;
    capacity_ = new_size;
  }
  size_ = new_size;
}

template <typename T>
void Vector<T>::Resize(size_t new_size, const T& value) {
  if (new_size <= size_) {
    std::destroy(data_ + new_size, data_ + size_);
  } else if (new_size <= capacity_) {
    try {
      std::uninitialized_fill(data_ + size_, data_ + new_size, value);
    } catch (...) {
      throw;
    }
  } else {
    T* new_data = Allocate(new_size);
    try {
      std::uninitialized_move(data_, data_ + size_, new_data);
    } catch (...) {
      operator delete(new_data);
      throw;
    }

    try {
      std::uninitialized_fill(new_data + size_, new_data + new_size, value);
    } catch (...) {
      for (size_t i = 0; i < size_; ++i) {
        (new_data + i)->~T();
      }
      operator delete(new_data);
      throw;
    }

    for (size_t i = 0; i < size_; ++i) {
      data_[i].~T();
    }
    operator delete(data_);
    data_ = new_data;
    capacity_ = new_size;
  }
  size_ = new_size;
}

template <typename T>
void Vector<T>::ShrinkToFit() {
  if (capacity_ > 0) {
    Reallocate(size_);
  }
  if (capacity_ == 0) {
    operator delete(data_);
    data_ = nullptr;
  }
}

template <typename T>
void Vector<T>::Clear() noexcept {
  std::destroy(data_, data_ + size_);
  size_ = 0;
}

template <typename T>
void Vector<T>::PushBack(const T& value) {
  auto possible_capacity = Recommend(size_ + 1);
  if (size_ == capacity_) {
    T* new_data = Allocate(possible_capacity);

    try {
      std::uninitialized_move(data_, data_ + size_, new_data);
    } catch (...) {
      operator delete(new_data);
      throw;
    }

    try {
      std::uninitialized_fill(new_data + size_, new_data + size_ + 1, value);
    } catch (...) {
      for (size_t i = 0; i < size_; ++i) {
        (new_data + i)->~T();
      }
      operator delete(new_data);
      throw;
    }

    for (size_t i = 0; i < size_; ++i) {
      data_[i].~T();
    }
    operator delete(data_);
    data_ = new_data;
    capacity_ = possible_capacity;
  } else {
    try {
      new (data_ + size_) T(value);
    } catch (...) {
      (data_ + size_)->~T();
      throw;
    }
  }
  ++size_;
}

template <typename T>
void Vector<T>::PushBack(T&& value) {
  /*
  if (size_ == capacity_) {
    try {
      Reserve(Recommend(size_ + 1));
    } catch (...) {
      throw;
    }
  }
  new (data_ + size_) T(std::move(value));
  ++size_;
   */
  auto possible_capacity = Recommend(size_ + 1);
  if (size_ == capacity_) {
    T* new_data = Allocate(possible_capacity);

    try {
      std::uninitialized_move(data_, data_ + size_, new_data);
    } catch (...) {
      operator delete(new_data);
      throw;
    }

    try {
      // std::uninitialized_fill(new_data + size_, new_data + size_ + 1, std::move(value));
      new (new_data + size_) T(std::move(value));
    } catch (...) {
      for (size_t i = 0; i < size_; ++i) {
        (new_data + i)->~T();
      }
      operator delete(new_data);
      throw;
    }

    for (size_t i = 0; i < size_; ++i) {
      data_[i].~T();
    }
    operator delete(data_);
    data_ = new_data;
    capacity_ = possible_capacity;
  } else {
    try {
      new (data_ + size_) T(std::move(value));
    } catch (...) {
      (data_ + size_)->~T();
      throw;
    }
  }
  ++size_;
}

template <typename T>
void Vector<T>::PopBack() {
  if (!Empty()) {
    (data_ + size_ - 1)->~T();
    --size_;
  }
}

template <typename T>
template <typename... Args>
void Vector<T>::EmplaceBack(Args&&... args) {
  if (size_ == capacity_) {
    Reserve(Recommend(size_ + 1));
  }
  try {
    new (data_ + size_) T(std::forward<Args>(args)...);
    ++size_;
  } catch (...) {
    // (data_ + size_)->~T();
    throw;
  }
}

template <typename T>
bool Vector<T>::operator<(const Vector& other) const {
  if (other.data_ == nullptr || data_ == nullptr) {
    return size_ < other.size_;
  }

  auto short_len = std::min(size_, other.size_);
  size_t i = 0;
  while (i < short_len) {
    if (data_[i] == other.data_[i]) {
      ++i;
    } else if (data_[i] < other.data_[i]) {
      return true;
    } else {
      return false;
    }
  }
  return size_ < other.size_;
}

template <typename T>
bool operator==(const Vector<T>& first, const Vector<T>& second) {
  return !(first < second) && !(second < first);
}

template <typename T>
bool operator!=(const Vector<T>& first, const Vector<T>& second) {
  return !(first == second);
}

template <typename T>
bool operator>(const Vector<T>& first, const Vector<T>& second) {
  return !(first < second) && !(first == second);
}

template <typename T>
bool operator>=(const Vector<T>& first, const Vector<T>& second) {
  return !(first < second);
}

template <typename T>
bool operator<=(const Vector<T>& first, const Vector<T>& second) {
  return !(second < first);
}

template <typename T>
Vector<T>::~Vector() noexcept {
  std::destroy(data_, data_ + size_);
  operator delete(data_);
}

#endif