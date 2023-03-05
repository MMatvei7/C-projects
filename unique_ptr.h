/*#ifndef unique_ptr_h
#define unique_ptr_h
#include <iostream>

template <class T>
class UniquePtr {
 private:
  T* ptr_;

 public:
  UniquePtr();
  explicit UniquePtr(T*);
  UniquePtr(const UniquePtr&) = delete;
  UniquePtr& operator=(const UniquePtr&) = delete;
  UniquePtr(UniquePtr&&) noexcept;
  UniquePtr& operator=(UniquePtr&&) noexcept;
  T* Release();
  T* Reset(T* object);
  T* Reset();
  void Swap(UniquePtr&);
  T* Get() const;
  T& operator*() const;
  T* operator->() const;
  explicit operator bool() const;
  ~UniquePtr();
};

template <class T>
UniquePtr<T>::UniquePtr() : ptr_(nullptr) {
}

template <class T>
UniquePtr<T>::UniquePtr(T* object) : ptr_(object) {
}

template <class T>
UniquePtr<T>::UniquePtr(UniquePtr<T>&& object) noexcept : ptr_(object.ptr_) {
  object.ptr_ = nullptr;
}

template <class T>
UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr<T>&& object) noexcept {
  if (this == &object) {
    return *this;
  }

  auto tmp = std::move(object);
  Swap(tmp);
  return *this;
}

template <class T>
T* UniquePtr<T>::Release() {
  auto tmp = ptr_;
  ptr_ = nullptr;
  return tmp;
}

template <class T>
T* UniquePtr<T>::Reset(T* object) {
  if (ptr_ == nullptr) {
    ptr_ = object;
    return ptr_;
  }
  auto tmp = ptr_;
  ptr_ = object;
  delete tmp;
  return ptr_;
}

template <class T>
T* UniquePtr<T>::Reset() {
  auto tmp = ptr_;
  ptr_ = nullptr;
  delete tmp;
  return ptr_;
}

template <class T>
void UniquePtr<T>::Swap(UniquePtr<T>& object) {
  std::swap(ptr_, object.ptr_);
}

template <class T>
T* UniquePtr<T>::Get() const {
  return ptr_;
}

template <class T>
T& UniquePtr<T>::operator*() const {
  return *ptr_;
}

template <class T>
UniquePtr<T>::operator bool() const {
  return ptr_ != nullptr;
}

template <class T>
T* UniquePtr<T>::operator->() const {
  return ptr_;
}

template <class T>
UniquePtr<T>::~UniquePtr() {
  delete ptr_;
}

#endif
*/
