#ifndef OPTIONAL_H_
#define OPTIONAL_H_
#include <utility>
#include <stdexcept>

class BadOptionalAccess : public std::runtime_error {
 public:
  BadOptionalAccess() : std::runtime_error("BadOptionalAccess") {
  }
};

template <typename T>
class Optional {
 private:
  char data_[sizeof(T)];
  bool has_value_ = false;

  T& GetValue() {
    return *reinterpret_cast<T*>(data_);
  }

  const T& GetValue() const {
    return *reinterpret_cast<const T*>(data_);
  }

 public:
  Optional() = default;
  Optional(const Optional&);
  Optional(Optional&&) noexcept;
  explicit Optional(const T&);
  explicit Optional(T&&);
  Optional& operator=(const Optional&);
  Optional& operator=(Optional&&);
  T& operator=(const T&);
  T& operator=(T&&);
  bool HasValue() const;
  explicit operator bool() const;
  T& Value();
  const T& Value() const;
  T& operator*();
  const T& operator*() const;
  template <typename... Args>
  T& Emplace(Args&&...);
  void Reset();
  void Swap(Optional&);
  ~Optional() noexcept;
};

template <typename T>
Optional<T>::Optional(const Optional<T>& other) {
  if (other.has_value_) {
    new (data_) T(other.GetValue());
    has_value_ = true;
  }
}

template <typename T>
Optional<T>::Optional(Optional<T>&& other) noexcept {
  if (other.has_value_) {
    new (data_) T(std::move(other.GetValue()));
    has_value_ = true;
  }
}

template <typename T>
Optional<T>::Optional(const T& value) {
  new (data_) T(value);
  has_value_ = true;
}

template <typename T>
Optional<T>::Optional(T&& value) {
  new (data_) T(std::move(value));
  has_value_ = true;
}

template <typename T>
Optional<T>& Optional<T>::operator=(const Optional<T>& other) {
  if (this == &other) {
    return *this;
  }

  if (!other.has_value_) {
    if (!has_value_) {
      return *this;
    }
    has_value_ = false;
    GetValue().~T();
    return *this;
  }

  if (!has_value_) {
    has_value_ = true;
    new (data_) T(other.GetValue());
  } else {
    GetValue() = other.GetValue();
  }

  return *this;
}

template <typename T>
Optional<T>& Optional<T>::operator=(Optional<T>&& other) {
  if (this == &other) {
    return *this;
  }

  if (!other.has_value_) {
    if (!has_value_) {
      return *this;
    }
    has_value_ = false;
    GetValue().~T();
    return *this;
  }

  if (!has_value_) {
    has_value_ = true;
    new (data_) T(std::move(other.GetValue()));
  } else {
    GetValue() = std::move(other.GetValue());
  }

  return *this;
}

template <typename T>
T& Optional<T>::operator=(const T& value) {
  if (!has_value_) {
    new (data_) T(value);
    has_value_ = true;
  } else {
    GetValue() = value;
  }
  return GetValue();
}

template <typename T>
T& Optional<T>::operator=(T&& value) {
  if (!has_value_) {
    new (data_) T(std::move(value));
    has_value_ = true;
  } else {
    GetValue() = std::move(value);
  }
  return GetValue();
}

template <typename T>
bool Optional<T>::HasValue() const {
  return has_value_;
}

template <typename T>
Optional<T>::operator bool() const {
  return has_value_;
}

template <typename T>
T& Optional<T>::Value() {
  if (!has_value_) {
    throw BadOptionalAccess();
  }
  return GetValue();
}

template <typename T>
const T& Optional<T>::Value() const {
  if (!has_value_) {
    throw BadOptionalAccess();
  }
  return GetValue();
}

template <typename T>
T& Optional<T>::operator*() {
  return *reinterpret_cast<T*>(data_);
}

template <typename T>
const T& Optional<T>::operator*() const {
  return *reinterpret_cast<const T*>(data_);
}

template <typename T>
template <typename... Args>
T& Optional<T>::Emplace(Args&&... args) {
  if (has_value_) {
    GetValue().~T();
  }
  new (data_) T(std::forward<Args>(args)...);
  has_value_ = true;
  return GetValue();
}

template <typename T>
void Optional<T>::Reset() {
  if (has_value_) {
    GetValue().~T();
  };
  has_value_ = false;
}

template <typename T>
void Optional<T>::Swap(Optional& other) {
  if (has_value_ && other.has_value_) {
    std::swap(GetValue(), other.GetValue());
    return;
  }
  if (!has_value_ && !other.has_value_) {
    return;
  }
  if (has_value_) {
    new (other.data_) T(std::move(GetValue()));
    std::swap(has_value_, other.has_value_);
    GetValue().~T();
    return;
  }
  other.Swap(*this);
}

template <typename T>
Optional<T>::~Optional() noexcept {
  if (has_value_) {
    GetValue().~T();
  }
}

#endif