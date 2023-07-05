#ifndef COMPILETIME__ITEROPS_H_
#define COMPILETIME__ITEROPS_H_
#include <iterator>
#include <type_traits>

template <typename Iterator>
void Advance(Iterator& iter, int n) {
  if constexpr (std::is_same_v<typename std::iterator_traits<Iterator>::iterator_category, std::forward_iterator_tag>) {
    for (int i = 0; i < n; ++i) {
      ++iter;
    }
  } else {
    if constexpr (std::is_same_v<typename std::iterator_traits<Iterator>::iterator_category,
                                 std::random_access_iterator_tag>) {
      iter += n;
    } else {
      if (n >= 0) {
        for (int i = 0; i < n; ++i) {
          ++iter;
        }
      } else {
        for (int i = 0; i > n; --i) {
          --iter;
        }
      }
    }
  }
}

template <typename Iterator>
Iterator Next(Iterator& it, int n = 1) {
  auto iter = it;
  if constexpr (std::is_same_v<typename std::iterator_traits<Iterator>::iterator_category, std::forward_iterator_tag>) {
    for (int i = 0; i < n; ++i) {
      ++iter;
    }
  } else {
    if constexpr (std::is_same_v<typename std::iterator_traits<Iterator>::iterator_category,
                                 std::random_access_iterator_tag>) {
      iter += n;
    } else {
      if (n >= 0) {
        for (int i = 0; i < n; ++i) {
          ++iter;
        }
      } else {
        for (int i = 0; i > n; --i) {
          --iter;
        }
      }
    }
  }
  return iter;
}

template <typename Iterator>
Iterator Prev(Iterator& it, int n = 1) {
  auto iter = it;
  if constexpr (std::is_same_v<typename std::iterator_traits<Iterator>::iterator_category, std::random_access_iterator_tag>) {
    iter -= n;
  } else {
    if  (n >= 0) {
      for (int i = 0; i < n; ++i) {
        --iter;
      }
    } else {
      for (int i = 0; i > n; --i) {
        ++iter;
      }
    }
  }
  return iter;
}

template <typename Iterator>
int Distance(Iterator& begin, Iterator& end) {
  int dist = 0;
  if constexpr (std::is_same_v<typename std::iterator_traits<Iterator>::iterator_category, std::random_access_iterator_tag>) {
    dist = end - begin;
  } else {
    for (auto i = begin; i != end; ++i) {
      ++dist;
    }
  }
  return dist;
}

#endif
