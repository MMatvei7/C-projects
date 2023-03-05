#include "cppstring.h"
#include <iostream>
#include <cstring>

void String::Reallocate(size_t new_size) {
  if (new_size == capacity_) {
    return;
  }
  auto new_memory = new char[new_size];
  for (size_t i = 0; i < size_; ++i) {
    new_memory[i] = str_[i];
  }
  capacity_ = new_size;
  delete[] str_;
  str_ = new_memory;
}

String::String() : size_(0), capacity_(0), str_(nullptr) {
}

String::String(size_t size, char symbol) : size_(size), capacity_(size) {
  if (size_ == 0) {
    str_ = nullptr;
  } else {
    str_ = new char[size];
    for (size_t i = 0; i < size; ++i) {
      str_[i] = symbol;
    }
  }
}

String::String(const char* str) {
  size_t size = 0;
  while (str[size] != '\0') {
    ++size;
  }
  if (size == 0) {
    size_ = capacity_ = 0;
    str_ = nullptr;
  } else {
    size_ = size;
    capacity_ = size;
    str_ = new char[size];
    for (size_t i = 0; i < size; ++i) {
      str_[i] = str[i];
    }
  }
}

String::String(const char* str, size_t size) : size_(size), capacity_(size) {
  if (size_ == 0) {
    str_ = nullptr;
  } else {
    str_ = new char[size];
    for (size_t i = 0; i < size; ++i) {
      str_[i] = str[i];
    }
  }
}

String::String(const String& str) : size_(str.size_), capacity_(str.capacity_) {
  if (size_ == 0) {
    str_ = nullptr;
  } else {
    str_ = new char[str.capacity_];
    for (size_t i = 0; i < str.size_; ++i) {
      str_[i] = str[i];
    }
  }
}

String::String(String&& str) noexcept : size_(str.size_), capacity_(str.capacity_), str_(str.str_) {
  str.str_ = nullptr;
  str.size_ = str.capacity_ = 0;
}

String& String::operator=(const String& other) {
  auto tmp = other;
  Swap(tmp);
  return *this;
}

String& String::operator=(String&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  auto tmp = std::move(other);
  Swap(tmp);
  return *this;
}

char& String::operator[](size_t idx) {
  return str_[idx];
}

char String::operator[](size_t idx) const {
  return str_[idx];
}

char& String::At(size_t idx) {
  if (idx >= size_) {
    throw StringOutOfRange{};
  }
  return str_[idx];
}

const char& String::At(size_t idx) const {
  if (idx >= size_) {
    throw StringOutOfRange{};
  }
  return str_[idx];
}

char& String::Front() {
  return str_[0];
}

const char& String::Front() const {
  return str_[0];
}

char& String::Back() {
  return str_[size_ - 1];
}

const char& String::Back() const {
  return str_[size_ - 1];
}

char* String::Data() {
  return str_;
}

const char* String::Data() const {
  return str_;
}

char* String::CStr() {
  return str_;
}

const char* String::CStr() const {
  return str_;
}

bool String::Empty() const {
  return size_ == 0;
}

size_t String::Size() const {
  return size_;
}

size_t String::Length() const {
  return size_;
}

size_t String::Capacity() const {
  return capacity_;
}

void String::Clear() {
  size_ = 0;
}

void String::Swap(String& other) {
  std::swap(str_, other.str_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}

void String::PopBack() {
  if (size_ != 0) {
    --size_;
  }
}

void String::PushBack(char symbol) {
  if (size_ == capacity_) {
    Reallocate(std::max<size_t>(2 * capacity_, 1));
  }
  str_[size_] = symbol;
  ++size_;
}

String& String::operator+=(const String& other) {
  if (size_ + other.size_ >= capacity_) {
    Reallocate(std::max(other.size_ + size_, 2 * capacity_));
  }
  for (size_t i = 0; i < other.size_; ++i, ++size_) {
    str_[size_] = other[i];
  }
  return *this;
}

void String::Resize(size_t new_size, char symbol) {
  if (new_size > capacity_) {
    Reallocate(new_size);
  }
  for (size_t i = size_; i < new_size; ++i) {
    str_[i] = symbol;
  }
  size_ = new_size;
}

void String::Reserve(size_t new_capacity) {
  if (new_capacity > capacity_) {
    Reallocate(new_capacity);
  }
}

void String::ShrinkToFit() {
  Reallocate(size_);
}

String operator+(const String& str1, const String& str2) {
  String tmp = str1;
  return tmp += str2;
}

bool String::operator<(const String& other) const {
  if (other.str_ == nullptr || str_ == nullptr) {
    return size_ < other.size_;
  }

  auto short_len = std::min(size_, other.size_);
  size_t i = 0;
  while (i < short_len) {
    if (str_[i] == other[i]) {
      ++i;
    } else if (str_[i] < other[i]) {
      return true;
    } else {
      return false;
    }
  }
  return size_ < other.size_;
}

bool operator==(const String& str1, const String& str2) {
  return !(str1 < str2) && !(str2 < str1);
}

bool operator!=(const String& str1, const String& str2) {
  return !(str1 == str2);
}

bool operator>(const String& str1, const String& str2) {
  return !(str1 < str2) && !(str1 == str2);
}

bool operator>=(const String& str1, const String& str2) {
  return !(str1 < str2);
}

bool operator<=(const String& str1, const String& str2) {
  return !(str2 < str1);
}

String::~String() noexcept {
  size_ = 0;
  capacity_ = 0;
  delete[] str_;
}

std::ostream& operator<<(std::ostream& out, const String& str) {
  for (size_t i = 0; i < str.Size(); ++i) {
    out << str[i];
  }
  return out;
}
