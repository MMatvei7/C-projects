#include <cstring>
#include "string_view.h"

StringView::StringView() : str_(nullptr) {
}

StringView::StringView(const char* take_str) : str_(take_str) {
  size_ = strlen(take_str);
}

StringView::StringView(const char* take_str, size_t size) : str_(take_str), size_(size) {
}

char StringView::operator[](size_t i) {
  return str_[i];
}

char StringView::operator[](size_t i) const {
  return str_[i];
}

char StringView::At(size_t idx) {
  if (idx >= size_) {
    throw StringViewOutOfRange{};
  }
  return str_[idx];
}

char StringView::At(size_t idx) const {
  if (idx >= size_) {
    throw StringViewOutOfRange{};
  }
  return str_[idx];
}

char StringView::Front() {
  return str_[0];
}

char StringView::Front() const {
  return str_[0];
}

char StringView::Back() {
  return str_[size_ - 1];
}

char StringView::Back() const {
  return str_[size_ - 1];
}

size_t StringView::Size() const {
  return size_;
}

size_t StringView::Length() const {
  return size_;
}

bool StringView::Empty() const {
  return size_ == 0;
}

const char* StringView::Data() {
  return str_;
}

const char* StringView::Data() const {
  return str_;
}

void StringView::Swap(StringView& other) {
  auto tmp = str_;
  str_ = other.Data();
  other = tmp;
  auto size_tmp = size_;
  size_ = other.GetSize();
  other.SetSize(size_tmp);
}

void StringView::RemovePrefix(size_t i) {
  str_ += i;
  size_ -= i;
}

void StringView::RemoveSuffix(size_t i) {
  size_ -= i;
}

StringView StringView::Substr(size_t pos, size_t count) {
  if (pos >= size_) {
    throw StringViewOutOfRange{};
  }
  StringView ans(str_);
  ans.RemovePrefix(pos);
  //ans.SetSize(std::min(size_ - pos, count));
  return ans;
}
