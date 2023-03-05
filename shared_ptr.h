#ifndef shared_ptr_h
#define shared_ptr_h
#define WEAK_PTR_IMPLEMENTED
#include <iostream>
#include <stdexcept>

class BadWeakPtr : public std::runtime_error {
 public:
  BadWeakPtr() : std::runtime_error("BadWeakPtr") {
  }
};

template <class U>
class WeakPtr;

struct Counter {
  size_t strong_ptr = 0;
  size_t weak_ptr = 0;
};

template <class T>
class SharedPtr {
 private:
  T* ptr_;
  Counter* strong_counter_;
  void Deleter();

 public:
  SharedPtr();
  SharedPtr(T*);                 // NOLINT
  SharedPtr(const WeakPtr<T>&);  // NOLINT
  SharedPtr(const SharedPtr&);
  SharedPtr& operator=(const SharedPtr&);
  SharedPtr(SharedPtr&&) noexcept;
  SharedPtr& operator=(SharedPtr&&) noexcept;
  T* Reset(T* object);
  T* Reset();
  void Swap(SharedPtr&);
  T* Get() const;
  Counter* GetCounter() const;
  size_t UseCount() const;
  T& operator*() const;
  T* operator->() const;
  explicit operator bool() const;
  ~SharedPtr() noexcept;
};

template <class T>
void SharedPtr<T>::Deleter() {
  if (strong_counter_ == nullptr || ptr_ == nullptr) {
    return;
  }
  if (strong_counter_->strong_ptr <= 1) {
    auto tmp_ptr = ptr_;
    auto tmp_cnt = strong_counter_;
    ptr_ = nullptr;
    strong_counter_ = nullptr;
    delete tmp_ptr;
    delete tmp_cnt;
  }
}

template <class T>
SharedPtr<T>::SharedPtr() : ptr_(nullptr), strong_counter_(nullptr) {
}

template <class T>
SharedPtr<T>::SharedPtr(T* object) : ptr_(object) {
  if (object == nullptr) {
    strong_counter_ = nullptr;
  } else {
    strong_counter_ = new Counter;
    strong_counter_->strong_ptr = 1;
  }
}

template <class T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& other) : ptr_(other.ptr_), strong_counter_(other.strong_counter_) {
  if (strong_counter_ != nullptr) {
    ++strong_counter_->strong_ptr;
  }
}

template <class T>
SharedPtr<T>::SharedPtr(const WeakPtr<T>& other) {
  if (other.Expired()) {
    throw BadWeakPtr{};
  }
  ptr_ = other.GetWeak();
  strong_counter_ = other.GetCnt();
  if (strong_counter_ != nullptr) {
    ++strong_counter_->strong_ptr;
  } else {
    strong_counter_->strong_ptr = 1;
  }
}

template <class T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other) {
  if (this == &other) {
    return *this;
  }

  Deleter();
  if (ptr_ != nullptr) {
    ptr_ = other.ptr_;
    --strong_counter_->strong_ptr;
    strong_counter_ = other.strong_counter_;
    if (other.strong_counter_ != nullptr) {
      ++strong_counter_->strong_ptr;
    } else {
      strong_counter_->strong_ptr = 2;
    }
  } else {
    ptr_ = other.ptr_;
    strong_counter_ = other.strong_counter_;
    if (other.strong_counter_ != nullptr) {
      ++strong_counter_->strong_ptr;
    }
  }
  return *this;
}

template <class T>
SharedPtr<T>::SharedPtr(SharedPtr<T>&& other) noexcept : ptr_(other.ptr_) {
  strong_counter_ = other.strong_counter_;
  other.ptr_ = nullptr;
  other.strong_counter_ = nullptr;
}

template <class T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  auto tmp = std::move(other);
  Swap(tmp);
  return *this;
}

template <class T>
void SharedPtr<T>::Swap(SharedPtr<T>& other) {
  std::swap(ptr_, other.ptr_);
  std::swap(strong_counter_, other.strong_counter_);
}

template <class T>
T* SharedPtr<T>::Reset(T* object) {
  if (ptr_ == nullptr) {
    ptr_ = object;
    strong_counter_ = new Counter;
    strong_counter_->strong_ptr = 1;
    return ptr_;
  }
  if (strong_counter_->strong_ptr == 1) {
    auto tmp_ptr = ptr_;
    ptr_ = object;
    delete tmp_ptr;
  } else {
    ptr_ = object;
    --strong_counter_->strong_ptr;
    strong_counter_ = new Counter;
    strong_counter_->strong_ptr = 1;
  }
  return ptr_;
}

template <class T>
T* SharedPtr<T>::Reset() {
  if (strong_counter_ == nullptr) {
    return nullptr;
  }
  if (strong_counter_->strong_ptr <= 1) {
    auto tmp = ptr_;
    auto tmp_cnt = strong_counter_;
    ptr_ = nullptr;
    --strong_counter_->strong_ptr;
    strong_counter_ = nullptr;
    delete tmp;
    delete tmp_cnt;
  } else {
    ptr_ = nullptr;
    --strong_counter_->strong_ptr;
    strong_counter_ = nullptr;
  }
  return ptr_;
}

template <class T>
T* SharedPtr<T>::Get() const {
  return ptr_;
}

template <class T>
Counter* SharedPtr<T>::GetCounter() const {
  return strong_counter_;
}

template <class T>
size_t SharedPtr<T>::UseCount() const {
  if (strong_counter_ == nullptr) {
    return 0;
  }
  return strong_counter_->strong_ptr;
}

template <class T>
T& SharedPtr<T>::operator*() const {
  return *ptr_;
}

template <class T>
SharedPtr<T>::operator bool() const {
  return ptr_ != nullptr;
}

template <class T>
T* SharedPtr<T>::operator->() const {
  return ptr_;
}

template <class T>
SharedPtr<T>::~SharedPtr<T>() noexcept {
  if (strong_counter_ != nullptr) {
    --strong_counter_->strong_ptr;
    if (strong_counter_->strong_ptr == 0) {
      delete ptr_;
      if (strong_counter_->weak_ptr == 0) {
        delete strong_counter_;
      }
    }
  }
}

template <class U>
class WeakPtr {
 private:
  U* weak_;
  Counter* weak_counter_;

 public:
  WeakPtr();
  WeakPtr(U*);  // NOLINT
  WeakPtr(const WeakPtr&);
  WeakPtr& operator=(const WeakPtr&);
  WeakPtr(WeakPtr&&) noexcept;
  WeakPtr& operator=(WeakPtr&&) noexcept;
  WeakPtr(const SharedPtr<U>&);  // NOLINT
  void Swap(WeakPtr&);
  U* Reset();
  U* GetWeak() const;
  Counter* GetCnt() const;
  size_t UseCount() const;
  bool Expired() const;
  SharedPtr<U> Lock() const;
  ~WeakPtr() noexcept;
};

template <class U>
WeakPtr<U>::WeakPtr() : weak_(nullptr), weak_counter_(nullptr) {
}

template <class U>
WeakPtr<U>::WeakPtr(U* object) : weak_(object) {
  if (object == nullptr) {
    weak_counter_ = nullptr;
  } else {
    weak_counter_ = new Counter;
    weak_counter_->weak_ptr = 1;
  }
}

template <class U>
WeakPtr<U>::WeakPtr(const WeakPtr<U>& other) : weak_(other.weak_), weak_counter_(other.weak_counter_) {
  if (weak_counter_ != nullptr) {
    ++weak_counter_->weak_ptr;
  }
}

template <class U>
WeakPtr<U>& WeakPtr<U>::operator=(const WeakPtr<U>& other) {
  if (this == &other) {
    return *this;
  }

  if (weak_ != nullptr) {
    weak_ = other.weak_;
    --weak_counter_->weak_ptr;
    if (weak_counter_->strong_ptr + weak_counter_->weak_ptr == 0) {
      auto tmp = weak_counter_;
      weak_counter_ = nullptr;
      delete tmp;
    }
    weak_counter_ = other.weak_counter_;
    if (other.weak_counter_ != nullptr) {
      ++weak_counter_->weak_ptr;
    }
  } else {
    weak_ = other.weak_;
    weak_counter_ = other.weak_counter_;
    if (other.weak_counter_ != nullptr) {
      ++weak_counter_->weak_ptr;
    }
  }
  return *this;
}

template <class U>
WeakPtr<U>::WeakPtr(WeakPtr<U>&& other) noexcept : weak_(other.weak_) {
  weak_counter_ = other.weak_counter_;
  other.weak_ = nullptr;
  other.weak_counter_ = nullptr;
}

template <class U>
WeakPtr<U>& WeakPtr<U>::operator=(WeakPtr<U>&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  auto tmp = std::move(other);
  Swap(tmp);
  return *this;
}

template <class U>
void WeakPtr<U>::Swap(WeakPtr<U>& other) {
  std::swap(weak_, other.weak_);
  std::swap(weak_counter_, other.weak_counter_);
}

template <class U>
WeakPtr<U>::WeakPtr(const SharedPtr<U>& other) : weak_(other.Get()) {
  weak_counter_ = other.GetCounter();
  if (weak_counter_ != nullptr) {
    ++weak_counter_->weak_ptr;
  }
}

template <class U>
U* WeakPtr<U>::Reset() {
  if (weak_counter_ == nullptr) {
    return weak_;
  }
  if (weak_counter_->strong_ptr + weak_counter_->weak_ptr > 0) {
    --weak_counter_->weak_ptr;
    weak_counter_ = nullptr;
    weak_ = nullptr;
  } else {
    delete weak_counter_;
  }
  return weak_;
}

template <class U>
U* WeakPtr<U>::GetWeak() const {
  return weak_;
}

template <class U>
Counter* WeakPtr<U>::GetCnt() const {
  return weak_counter_;
}

template <class U>
size_t WeakPtr<U>::UseCount() const {
  if (weak_counter_ == nullptr) {
    return 0;
  }
  return weak_counter_->strong_ptr;
}

template <class U>
bool WeakPtr<U>::Expired() const {
  if (weak_counter_ == nullptr) {
    return true;
  }
  return weak_counter_->strong_ptr == 0;
}

template <class U>
SharedPtr<U> WeakPtr<U>::Lock() const {
  if (Expired()) {
    return nullptr;
  }
  SharedPtr<U> obj(*this);
  return obj;
}

template <class U>
WeakPtr<U>::~WeakPtr() noexcept {
  if (weak_counter_ != nullptr) {
    --weak_counter_->weak_ptr;
    if (weak_counter_->strong_ptr + weak_counter_->weak_ptr == 0) {
      delete weak_counter_;
    }
  }
}

#endif
