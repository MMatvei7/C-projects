#ifndef ITERTOOLS_RANGE___RANGE_H_
#define ITERTOOLS_RANGE___RANGE_H_
#define REVERSE_RANGE_IMPLEMENTED
#include <iterator>
#include <type_traits>

class Range {
 public:
  int64_t begin_;
  int64_t end_;
  int64_t step_;
  int64_t finish_;

  class Iterator {
   private:
    int64_t pos_;
    int64_t iteration_;

   public:
    // NOLINTNEXTLINE
    using iterator_category = std::bidirectional_iterator_tag;
    // NOLINTNEXTLINE
    using value_type = int64_t;
    // NOLINTNEXTLINE
    using difference_type = int64_t;
    // NOLINTNEXTLINE
    using pointer = int64_t*;
    // NOLINTNEXTLINE
    using reference = value_type;
    Iterator() = default;
    inline Iterator(int64_t, int64_t);
    Iterator operator++(int);
    Iterator& operator++();
    Iterator operator--(int);
    Iterator& operator--();
    bool operator!=(Iterator) const;
    bool operator==(Iterator) const;
    std::iterator_traits<Iterator>::reference& operator*() {
      return pos_;
    }
  };

 public:
  Range() = default;
  inline explicit Range(int64_t);
  inline Range(int64_t, int64_t);
  inline Range(int64_t, int64_t, int64_t);
  // NOLINTNEXTLINE
  Iterator begin() const {
    return Iterator(begin_, step_);
  }
  // NOLINTNEXTLINE
  Iterator end() const {
    return Iterator(finish_, step_);
  }
  // NOLINTNEXTLINE
  std::reverse_iterator<Iterator> rbegin() const {
    return std::reverse_iterator(end());
  }
  // NOLINTNEXTLINE
  std::reverse_iterator<Iterator> rend() const {
    return std::reverse_iterator(begin());
  }
};

inline Range::Iterator Range::Iterator::operator++(int) {
  auto tmp = pos_;
  pos_ += iteration_;
  return Iterator(tmp, iteration_);
}

inline Range::Iterator& Range::Iterator::operator++() {
  pos_ += iteration_;
  return *this;
}

inline bool Range::Iterator::operator!=(Range::Iterator other) const {
  return pos_ != other.pos_;
}

Range::Iterator::Iterator(int64_t position, int64_t user_step) : pos_(position), iteration_(user_step) {
}

inline Range::Iterator Range::Iterator::operator--(int) {
  auto tmp = *this;
  pos_ -= iteration_;
  return tmp;
}

inline Range::Iterator& Range::Iterator::operator--() {
  pos_ -= iteration_;
  return *this;
}

inline bool Range::Iterator::operator==(Range::Iterator other) const {
  return !(*this != other);
}

Range::Range(int64_t start) : begin_(0), end_(start), step_(1) {
  if (start <= 0) {
    begin_ = start;
  }
  finish_ = end_;
}

Range::Range(int64_t start, int64_t finish) : begin_(start), end_(finish), step_(1) {
  if (finish <= start) {
    end_ = start;
  }
  finish_ = end_;
}

Range::Range(int64_t start, int64_t finish, int64_t user_step) : begin_(start), end_(finish), step_(user_step) {
  if ((finish < start && user_step > 0) || (finish > start && user_step < 0) || user_step == 0) {
    begin_ = end_ = start;
    finish_ = start;
    step_ = 1;
  } else {
    auto dist = end_ - begin_;
    auto flag = dist > 0 ? dist % (step_ > 0 ? step_ : -step_) : (-dist) % (step_ > 0 ? step_ : -step_);
    finish_ = (flag == 0 ? end_ : (abs(dist) / abs(step_)) * step_ + begin_ + step_);
  }
}

#endif