#ifndef ITERTOOLS_REVERSE___REVERSED_H_
#define ITERTOOLS_REVERSE___REVERSED_H_
#define REVERSE_REVERSED_IMPLEMENTED
#define TEMPORARY_REVERSED_IMPLEMENTED
#include <type_traits>
#include <iterator>

template <typename Container>
struct IterType {
  using Type = typename Container::iterator;
};

template <typename Container>
struct IterType<const Container> {
  using Type = typename Container::const_iterator;
};

template <typename Container>
struct IsConstReverseIter {
  using Type = typename Container::reverse_iterator;
};

template <typename Container>
struct IsConstReverseIter<const Container> {
  using Type = typename Container::const_reverse_iterator;
};

template <typename Container>
class Reverse {
 private:
  Container& container_;

 public:
  explicit Reverse(Container& other) : container_(other) {
  }
  // NOLINTNEXTLINE
  typename IsConstReverseIter<Container>::Type begin() const {
    return container_.rbegin();
  }
  // NOLINTNEXTLINE
  typename IsConstReverseIter<Container>::Type end() const {
    return container_.rend();
  }
  // NOLINTNEXTLINE
  typename IterType<Container>::Type rbegin() const {
    return container_.begin();
  }
  // NOLINTNEXTLINE
  typename IterType<Container>::Type rend() const {
    return container_.end();
  }
};

template <typename Container>
class Range {
 private:
  Container container_;

 public:
  explicit Range(const Container& other) : container_(other) {
  }

  explicit Range(Container&& other) : container_(std::move(other)) {
  }
  // NOLINTNEXTLINE
  auto begin() {
    return container_.rbegin();
  }
  // NOLINTNEXTLINE
  auto end() {
    return container_.rend();
  }
  // NOLINTNEXTLINE
  auto rbegin() {
    return container_.begin();
  }
  // NOLINTNEXTLINE
  auto rend() {
    return container_.end();
  }
  // NOLINTNEXTLINE
  auto begin() const {
    return container_.rbegin();
  }
  // NOLINTNEXTLINE
  auto end() const {
    return container_.rend();
  }
  // NOLINTNEXTLINE
  auto rbegin() const {
    return container_.begin();
  }
  // NOLINTNEXTLINE
  auto rend() const {
    return container_.end();
  }
};

template <typename Container>
Reverse<Container> Reversed(Container& container) {
  return Reverse<Container>(container);
}

template <typename Container>
Range<Container> Reversed(Container&& container) {
  return Range<Container>(std::move(container));
}

#endif