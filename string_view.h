#ifndef string_view_h
#define string_view_h
#include <stdexcept>

class StringViewOutOfRange : public std::out_of_range {
 public:
  StringViewOutOfRange() : std::out_of_range("StringViewOutOfRange") {
  }
};

class StringView {
 private:
  const char* str_;
  size_t size_;

 public:
  StringView();
  StringView(const char*);  // NOLINT
  StringView(const char*, size_t);
  char operator[](size_t);
  char operator[](size_t) const;
  char At(size_t);
  char At(size_t) const;
  char Front();
  char Front() const;
  char Back();
  char Back() const;
  size_t Size() const;
  size_t Length() const;
  bool Empty() const;
  const char* Data();
  const char* Data() const;
  void Swap(StringView&);
  void RemovePrefix(size_t);
  void RemoveSuffix(size_t);
  size_t GetSize() {
    return size_;
  }
  void SetSize(size_t size) {
    size_ = size;
  }
  StringView Substr(size_t pos, size_t count = 1e18);
};

#endif
