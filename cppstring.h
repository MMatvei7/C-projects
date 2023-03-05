#ifndef cppstring_h
#define cppstring_h
#include <cstdlib>
#include <stdexcept>

class StringOutOfRange : public std::out_of_range {
 public:
  StringOutOfRange() : std::out_of_range("StringOutOfRange") {
  }
};

class String {
 private:
  size_t size_;
  size_t capacity_;
  char* str_ = nullptr;
  void Reallocate(size_t);

 public:
  String();
  String(size_t, char);
  String(const char*);  // NOLINT
  String(const char*, size_t);
  String(const String&);
  String(String&&) noexcept;
  String& operator=(const String&);
  String& operator=(String&&) noexcept;
  char& operator[](size_t);
  char operator[](size_t) const;
  char& At(size_t);
  const char& At(size_t) const;
  char& Front();
  const char& Front() const;
  char& Back();
  const char& Back() const;
  char* Data();
  const char* Data() const;
  char* CStr();
  const char* CStr() const;
  bool Empty() const;
  size_t Size() const;
  size_t Length() const;
  size_t Capacity() const;
  void Clear();
  void Swap(String&);
  void PopBack();
  void PushBack(char);
  String& operator+=(const String&);
  void Resize(size_t, char);
  void Reserve(size_t);
  void ShrinkToFit();
  bool operator<(const String&) const;
  ~String() noexcept;
};

String operator+(const String&, const String&);
bool operator==(const String&, const String&);
bool operator!=(const String&, const String&);
bool operator>(const String&, const String&);
bool operator>=(const String&, const String&);
bool operator<=(const String&, const String&);
std::ostream& operator<<(std::ostream& out, const String&);

#endif
