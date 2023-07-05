#ifndef UNORDERED_SET_UNORDERED_SET_H_
#define UNORDERED_SET_UNORDERED_SET_H_
#define ITERATOR_IMPLEMENTED
#define FORWARD_LIST_IMPLEMENTED
#include <functional>
#include <forward_list>
#include <vector>
#include <optional>
#include <iterator>
#include <type_traits>

template <typename T, typename Hasher = std::hash<T>, typename KeyEqual = std::equal_to<T>>
class UnorderedSet {
 private:
  struct Node {
    T value;
    size_t cached;
    template <bool IsConst>
    friend class CommonIterator;
  };

  std::forward_list<Node> elements_;
  std::vector<std::optional<typename std::forward_list<Node>::iterator>> buckets_;
  size_t table_size_;
  const Hasher hasher_ = Hasher();
  const KeyEqual key_equal_ = KeyEqual();
  void TotalRehash(size_t);

 public:
  UnorderedSet();
  explicit UnorderedSet(size_t);
  UnorderedSet(typename std::forward_list<T>::iterator, typename std::forward_list<T>::iterator);
  UnorderedSet(const UnorderedSet&);
  UnorderedSet(UnorderedSet&&);
  UnorderedSet& operator=(const UnorderedSet&);
  UnorderedSet& operator=(UnorderedSet&&);
  size_t Size() const;
  bool Empty() const;
  void Clear();
  void Insert(const T&);
  void Insert(T&&);
  void Erase(const T&);
  bool Find(const T&) const;
  void Rehash(size_t);
  void Reserve(size_t);
  size_t BucketCount() const;
  size_t BucketSize(size_t) const;
  size_t Bucket(const T&) const;
  float LoadFactor() const;
  template <bool IsConst>
  class CommonIterator {
    std::conditional_t<IsConst, typename std::forward_list<Node>::const_iterator, typename std::forward_list<Node>::iterator> it_;

   public:
    CommonIterator(std::conditional_t<IsConst, typename std::forward_list<Node>::const_iterator, typename std::forward_list<Node>::iterator>);
    template <bool Const = IsConst>
    CommonIterator& operator=(const CommonIterator<Const>&);
    std::conditional_t<IsConst, const T&, T&> operator*() const;
    std::conditional_t<IsConst, const T*, T*> operator->() const;
    CommonIterator operator++(int);
    CommonIterator& operator++();
    bool operator!=(const CommonIterator&);
    std::conditional_t<IsConst, typename std::forward_list<Node>::const_iterator, typename std::forward_list<Node>::iterator> GetIt() const {
      return it_;
    }
  };
  using Iterator = CommonIterator<false>;
  using ConstIterator = CommonIterator<true>;
  using DifferenceType = typename std::forward_list<T>::iterator::difference_type;
  Iterator begin();
  Iterator end();
  ConstIterator begin() const;
  ConstIterator cbegin() const;
  ConstIterator end() const;
  ConstIterator cend() const;
  ~UnorderedSet() = default;
};

template <typename T, typename Hasher, typename KeyEqual>
template <bool IsConst>
template <bool Const>
typename UnorderedSet<T, Hasher, KeyEqual>::template CommonIterator<IsConst>& UnorderedSet<T, Hasher, KeyEqual>::CommonIterator<IsConst>::operator=(const UnorderedSet<T, Hasher, KeyEqual>::CommonIterator<Const>& iter) {
  it_ = iter.GetIt();
  return *this;
}

template <typename T, typename Hasher, typename KeyEqual>
template <bool IsConst>
bool UnorderedSet<T, Hasher, KeyEqual>::CommonIterator<IsConst>::operator!=(const UnorderedSet<T, Hasher, KeyEqual>::CommonIterator<IsConst>& iter) {
  return it_ != iter.it_;
}

template <typename T, typename Hasher, typename KeyEqual>
typename UnorderedSet<T, Hasher, KeyEqual>::ConstIterator UnorderedSet<T, Hasher, KeyEqual>::cend() const {
  return ConstIterator(elements_.cend());
}

template <typename T, typename Hasher, typename KeyEqual>
typename UnorderedSet<T, Hasher, KeyEqual>::ConstIterator UnorderedSet<T, Hasher, KeyEqual>::cbegin() const {
  return ConstIterator(elements_.cbegin());
}

template <typename T, typename Hasher, typename KeyEqual>
typename UnorderedSet<T, Hasher, KeyEqual>::ConstIterator UnorderedSet<T, Hasher, KeyEqual>::end() const {
  return ConstIterator(elements_.cend());
}

template <typename T, typename Hasher, typename KeyEqual>
typename UnorderedSet<T, Hasher, KeyEqual>::ConstIterator UnorderedSet<T, Hasher, KeyEqual>::begin() const {
  return ConstIterator(elements_.cbegin());
}


template <typename T, typename Hasher, typename KeyEqual>
typename UnorderedSet<T, Hasher, KeyEqual>::Iterator UnorderedSet<T, Hasher, KeyEqual>::end() {
  return Iterator(elements_.end());
}

template <typename T, typename Hasher, typename KeyEqual>
typename UnorderedSet<T, Hasher, KeyEqual>::Iterator UnorderedSet<T, Hasher, KeyEqual>::begin() {
  return Iterator(elements_.begin());
}

template <typename T, typename Hasher, typename KeyEqual>
template <bool IsConst>
UnorderedSet<T, Hasher, KeyEqual>::CommonIterator<IsConst>::CommonIterator(std::conditional_t<IsConst, typename std::forward_list<Node>::const_iterator, typename std::forward_list<Node>::iterator> ptr) : it_(ptr) {
}

template <typename T, typename Hasher, typename KeyEqual>
template <bool IsConst>
std::conditional_t<IsConst, const T&, T&> UnorderedSet<T, Hasher, KeyEqual>::CommonIterator<IsConst>::operator*() const {
  return it_->value;
}

template <typename T, typename Hasher, typename KeyEqual>
template <bool IsConst>
std::conditional_t<IsConst, const T*, T*> UnorderedSet<T, Hasher, KeyEqual>::CommonIterator<IsConst>::operator->() const {
  return &(it_->value);
}

template <typename T, typename Hasher, typename KeyEqual>
template <bool IsConst>
typename UnorderedSet<T, Hasher, KeyEqual>::template CommonIterator<IsConst> UnorderedSet<T, Hasher, KeyEqual>::CommonIterator<IsConst>::operator++(int) {
  auto tmp = it_;
  ++it_;
  return CommonIterator(tmp);
}

template <typename T, typename Hasher, typename KeyEqual>
template <bool IsConst>
typename UnorderedSet<T, Hasher, KeyEqual>::template CommonIterator<IsConst>& UnorderedSet<T, Hasher, KeyEqual>::CommonIterator<IsConst>::operator++() {
  ++it_;
  return *this;
}

template <typename T, typename Hasher, typename KeyEqual>
size_t UnorderedSet<T, Hasher, KeyEqual>::Bucket(const T& key) const {
  return hasher_(key) % buckets_.size();
}

template <typename T, typename Hasher, typename KeyEqual>
size_t UnorderedSet<T, Hasher, KeyEqual>::BucketSize(size_t id) const {
  size_t size_id = 0;
  if (id >= buckets_.size()) {
    return 0;
  }
  if (buckets_[id].has_value()) {
    auto id_it = std::next(buckets_[id].value());
    while (id_it != elements_.end() && id_it->cached == id) {
      ++size_id;
      ++id_it;
    }
  }
  return size_id;
}

template <typename T, typename Hasher, typename KeyEqual>
size_t UnorderedSet<T, Hasher, KeyEqual>::BucketCount() const {
  return buckets_.size();
}

template <typename T, typename Hasher, typename KeyEqual>
void UnorderedSet<T, Hasher, KeyEqual>::Reserve(size_t new_bucket_count) {
  if (new_bucket_count > buckets_.size()) {
    TotalRehash(new_bucket_count);
  }
}

template <typename T, typename Hasher, typename KeyEqual>
void UnorderedSet<T, Hasher, KeyEqual>::Rehash(size_t new_bucket_count) {
  if (BucketCount() == new_bucket_count || table_size_ > new_bucket_count) {
    return;
  }
  TotalRehash(new_bucket_count);
}

template <typename T, typename Hasher, typename KeyEqual>
bool UnorderedSet<T, Hasher, KeyEqual>::Find(const T& value) const {
  if (buckets_.empty()) {
    return false;
  }
  size_t search_hash = hasher_(value) % buckets_.size();
  if (!buckets_[search_hash].has_value()) {
    return false;
  }
  auto it = std::next(buckets_[search_hash].value());
  while (it != elements_.end() && it->cached == search_hash) {
    if (key_equal_(it->value, value)) {
      return true;
    }
    ++it;
  }
  return false;
}

template <typename T, typename Hasher, typename KeyEqual>
void UnorderedSet<T, Hasher, KeyEqual>::TotalRehash(size_t new_size) {
  if (buckets_.empty()) {
    buckets_.resize(new_size);
    return;
  }
  std::vector<std::optional<typename std::forward_list<Node>::iterator>> new_buckets(new_size);
  std::forward_list<Node> new_elements;
  for (auto i : elements_) {
    size_t hash = hasher_(i.value) % new_size;
    if (!new_buckets[hash].has_value()) {
      if (new_elements.empty()) {
        new_elements.push_front(Node{i.value, hash});
      } else {
        size_t replace_hash = hasher_(new_elements.begin()->value) % new_size;
        new_elements.push_front(Node{i.value, hash});
        new_buckets[replace_hash] = new_elements.begin();
      }
      new_buckets[hash] = new_elements.before_begin();
    } else {
      new_elements.insert_after(new_buckets[hash].value(), Node{i.value, hash});
    }
  }
  buckets_.clear();
  elements_.clear();
  auto invalidated = new_elements.front().cached;
  buckets_.swap(new_buckets);
  elements_.splice_after(elements_.before_begin(), new_elements);
  buckets_[invalidated] = elements_.before_begin();
}

template <typename T, typename Hasher, typename KeyEqual>
float UnorderedSet<T, Hasher, KeyEqual>::LoadFactor() const {
  return table_size_ == 0 ? 0 : static_cast<float>(table_size_) / static_cast<float>(buckets_.size());
}

template <typename T, typename Hasher, typename KeyEqual>
void UnorderedSet<T, Hasher, KeyEqual>::Erase(const T& value) {
  if (buckets_.empty()) {
    return;
  }
  size_t search_hash = hasher_(value) % buckets_.size();
  if (!buckets_[search_hash].has_value()) {
    return;
  }
  auto it = std::next(buckets_[search_hash].value());
  auto prev_it = buckets_[search_hash].value();
  while (it != elements_.end() && it->cached == search_hash) {
    if (key_equal_(it->value, value)) {
      --table_size_;
      size_t deleted_hash = hasher_(value) % buckets_.size();
      if (prev_it == elements_.before_begin()) {

        auto next_it = std::next(it);
        if (next_it != elements_.end() && next_it->cached != deleted_hash) {
          buckets_[deleted_hash].reset();
          elements_.erase_after(prev_it);
          buckets_[next_it->cached] = elements_.before_begin();
          return;
        }
        if (next_it == elements_.end()) {
          buckets_[deleted_hash].reset();
          elements_.erase_after(prev_it);
          return;
        }
        buckets_[deleted_hash].reset();
        elements_.erase_after(prev_it);
        buckets_[elements_.front().cached] = elements_.before_begin();
      } else {

        if (std::next(it) != elements_.end() && std::next(it)->cached == deleted_hash &&
            deleted_hash == prev_it->cached) {
          elements_.erase_after(prev_it);
          return;
        }

        if (std::next(it) == elements_.end()) {
          if (prev_it->cached != deleted_hash) {
            buckets_[deleted_hash].reset();
          }
          elements_.erase_after(prev_it);
          return;
        }

        // case if the element has unique hash
        if (prev_it->cached != deleted_hash && deleted_hash != std::next(it)->cached) {
          buckets_[deleted_hash].reset();
          buckets_[std::next(it)->cached].reset();
          elements_.erase_after(prev_it);
          buckets_[std::next(prev_it)->cached] = prev_it;
          return;
        }

        // if value is the bucket end
        if (deleted_hash != std::next(it)->cached) {
          buckets_[std::next(it)->cached].reset();
          elements_.erase_after(prev_it);
          buckets_[std::next(prev_it)->cached] = prev_it;
          return;
        }

        // if value hash is equal to prev_it hash
        if (deleted_hash == std::next(it)->cached && deleted_hash != prev_it->cached) {
          buckets_[deleted_hash].reset();
          elements_.erase_after(prev_it);
          buckets_[std::next(prev_it)->cached] = prev_it;
        }
      }
      return;
    }
    ++it;
    ++prev_it;
  }
}

template <typename T, typename Hasher, typename KeyEqual>
void UnorderedSet<T, Hasher, KeyEqual>::Insert(T&& value) {
  if (LoadFactor() >= 1 || buckets_.empty()) {
    if (buckets_.empty()) {
      buckets_.resize(1);
    } else {
      TotalRehash(2 * buckets_.size());
    }
  }
  if (!Find(value)) {
    ++table_size_;
    size_t hash = hasher_(value) % buckets_.size();
    if (!buckets_[hash].has_value()) {
      if (elements_.empty()) {
        elements_.push_front(Node{std::move(value), hash});
      } else {
        size_t replace_hash = hasher_(elements_.begin()->value) % buckets_.size();
        elements_.push_front(Node{std::move(value), hash});
        buckets_[replace_hash] = elements_.begin();
      }
      buckets_[hash] = elements_.before_begin();
    } else {
      elements_.insert_after(buckets_[hash].value(), Node{std::move(value), hash});
    }
  }
}

template <typename T, typename Hasher, typename KeyEqual>
void UnorderedSet<T, Hasher, KeyEqual>::Insert(const T& value) {
  if (LoadFactor() >= 1 || buckets_.empty()) {
    if (buckets_.empty()) {
      buckets_.resize(1);
    } else {
      TotalRehash(2 * buckets_.size());
    }
  }
  if (!Find(value)) {
    ++table_size_;
    size_t hash = hasher_(value) % buckets_.size();
    if (!buckets_[hash].has_value()) {
      if (elements_.empty()) {
        elements_.push_front(Node{value, hash});
      } else {
        size_t replace_hash = hasher_(elements_.begin()->value) % buckets_.size();
        elements_.push_front(Node{value, hash});
        buckets_[replace_hash] = elements_.begin();
      }
      buckets_[hash] = elements_.before_begin();
    } else {
      elements_.insert_after(buckets_[hash].value(), Node{value, hash});
    }
  }
}

template <typename T, typename Hasher, typename KeyEqual>
void UnorderedSet<T, Hasher, KeyEqual>::Clear() {
  buckets_.clear();
  elements_.clear();
  table_size_ = 0;
}

template <typename T, typename Hasher, typename KeyEqual>
bool UnorderedSet<T, Hasher, KeyEqual>::Empty() const {
  return table_size_ == 0;
}

template <typename T, typename Hasher, typename KeyEqual>
size_t UnorderedSet<T, Hasher, KeyEqual>::Size() const {
  return table_size_;
}

template <typename T, typename Hasher, typename KeyEqual>
UnorderedSet<T, Hasher, KeyEqual>& UnorderedSet<T, Hasher, KeyEqual>::operator=(UnorderedSet&& hashset) {
  if (this == &hashset) {
    return *this;
  }

  auto invalidated = hasher_(hashset.elements_.front().value) % hashset.buckets_.size();
  buckets_ = std::move(hashset.buckets_);
  elements_ = std::move(hashset.elements_);
  buckets_[invalidated] = elements_.before_begin();
  table_size_ = hashset.table_size_;
  hashset.table_size_ = 0;
  return *this;
}

template <typename T, typename Hasher, typename KeyEqual>
UnorderedSet<T, Hasher, KeyEqual>& UnorderedSet<T, Hasher, KeyEqual>::operator=(const UnorderedSet& hashset) {
  if (this == &hashset) {
    return *this;
  }

  buckets_ = hashset.buckets_;
  elements_ = hashset.elements_;
  table_size_ = hashset.table_size_;
  return *this;
}

template <typename T, typename Hasher, typename KeyEqual>
UnorderedSet<T, Hasher, KeyEqual>::UnorderedSet(UnorderedSet&& hashset) {
  auto invalidated = hasher_(hashset.elements_.front().value) % hashset.buckets_.size();
  buckets_ = std::move(hashset.buckets_);
  elements_ = std::move(hashset.elements_);
  buckets_[invalidated] = elements_.before_begin();
  table_size_ = hashset.table_size_;
  hashset.table_size_ = 0;
}

template <typename T, typename Hasher, typename KeyEqual>
UnorderedSet<T, Hasher, KeyEqual>::UnorderedSet(const UnorderedSet& hashset) {
  buckets_ = hashset.buckets_;
  elements_ = hashset.elements_;
  table_size_ = hashset.table_size_;
}

template <typename T, typename Hasher, typename KeyEqual>
UnorderedSet<T, Hasher, KeyEqual>::UnorderedSet(typename std::forward_list<T>::iterator begin,
                                                typename std::forward_list<T>::iterator end)
    : buckets_(std::distance(begin, end)) {
  table_size_ = 0;
  for (auto i = begin; i != end; ++i) {
    Insert(*i);
  }
}

template <typename T, typename Hasher, typename KeyEqual>
UnorderedSet<T, Hasher, KeyEqual>::UnorderedSet(size_t count)
    : buckets_(count), table_size_(0), hasher_(Hasher()), key_equal_(KeyEqual()) {
}

template <typename T, typename Hasher, typename KeyEqual>
UnorderedSet<T, Hasher, KeyEqual>::UnorderedSet() : table_size_(0), hasher_(Hasher()), key_equal_(KeyEqual()) {
}

#endif
/*
#ifndef UNORDERED_SET_UNORDERED_SET_H_
#define UNORDERED_SET_UNORDERED_SET_H_
#define ITERATOR_IMPLEMENTED
#define FORWARD_LIST_IMPLEMENTED
#include <functional>
#include <forward_list>
#include <vector>
#include <optional>
#include <iterator>
#include <type_traits>

template <typename T, typename Hasher = std::hash<T>, typename KeyEqual = std::equal_to<T>>
class UnorderedSet {
 private:

  struct Node {
    T value;
    size_t cached;
    template <bool IsConst>
    friend class CommonIterator;
  };


  std::forward_list<T> elements_;
  std::vector<std::optional<typename std::forward_list<T>::iterator>> buckets_;
  size_t table_size_;
  const Hasher hasher_ = Hasher();
  const KeyEqual key_equal_ = KeyEqual();
  void TotalRehash(size_t);

 public:
  UnorderedSet();
  explicit UnorderedSet(size_t);
  UnorderedSet(typename std::forward_list<T>::iterator, typename std::forward_list<T>::iterator);
  UnorderedSet(const UnorderedSet&);
  UnorderedSet(UnorderedSet&&);
  UnorderedSet& operator=(const UnorderedSet&);
  UnorderedSet& operator=(UnorderedSet&&);
  size_t Size() const;
  bool Empty() const;
  void Clear();
  void Insert(const T&);
  void Insert(T&&);
  void Erase(const T&);
  bool Find(const T&) const;
  void Rehash(size_t);
  void Reserve(size_t);
  size_t BucketCount() const;
  size_t BucketSize(size_t) const;
  size_t Bucket(const T&) const;
  size_t Hash(const T& value) const {
    return hasher_(value) % buckets_.size();
  }
  float LoadFactor() const;
  using Iterator = typename std::forward_list<T>::const_iterator;
  using ConstIterator = typename std::forward_list<T>::const_iterator;
  using DifferenceType = typename std::forward_list<T>::iterator::difference_type;
  // NOLINT
  Iterator begin() {
    return elements_.begin();
  }
  // NOLINT
  Iterator end() {
    return elements_.end();
  }
  // NOLINT
  ConstIterator begin() const {
    return elements_.cbegin();
  }
  // NOLINT
  ConstIterator cbegin() const {
    return elements_.cbegin();
  }
  // NOLINT
  ConstIterator end() const {
    return elements_.cend();
  }
  // NOLINT
  ConstIterator cend() const {
    return elements_.cend();
  }
  ~UnorderedSet() = default;
};

template <typename T, typename Hasher, typename KeyEqual>
size_t UnorderedSet<T, Hasher, KeyEqual>::Bucket(const T& key) const {
  return hasher_(key) % buckets_.size();
}

template <typename T, typename Hasher, typename KeyEqual>
size_t UnorderedSet<T, Hasher, KeyEqual>::BucketSize(size_t id) const {
  size_t size_id = 0;
  if (id >= buckets_.size()) {
    return 0;
  }
  if (buckets_[id].has_value()) {
    auto id_it = std::next(buckets_[id].value());
    while (id_it != elements_.end() && Hash(*id_it) == id) {
      ++size_id;
      ++id_it;
    }
  }
  return size_id;
}

template <typename T, typename Hasher, typename KeyEqual>
size_t UnorderedSet<T, Hasher, KeyEqual>::BucketCount() const {
  return buckets_.size();
}

template <typename T, typename Hasher, typename KeyEqual>
void UnorderedSet<T, Hasher, KeyEqual>::Reserve(size_t new_bucket_count) {
  if (new_bucket_count > buckets_.size()) {
    TotalRehash(new_bucket_count);
  }
}

template <typename T, typename Hasher, typename KeyEqual>
void UnorderedSet<T, Hasher, KeyEqual>::Rehash(size_t new_bucket_count) {
  if (BucketCount() == new_bucket_count || table_size_ > new_bucket_count) {
    return;
  }
  TotalRehash(new_bucket_count);
}

template <typename T, typename Hasher, typename KeyEqual>
bool UnorderedSet<T, Hasher, KeyEqual>::Find(const T& value) const {
  if (buckets_.empty()) {
    return false;
  }
  size_t search_hash = hasher_(value) % buckets_.size();
  if (!buckets_[search_hash].has_value()) {
    return false;
  }
  auto it = std::next(buckets_[search_hash].value());
  while (it != elements_.end() && Hash(*it) == search_hash) {
    if (key_equal_(*it, value)) {
      return true;
    }
    ++it;
  }
  return false;
}

template <typename T, typename Hasher, typename KeyEqual>
void UnorderedSet<T, Hasher, KeyEqual>::TotalRehash(size_t new_size) {
  if (buckets_.empty()) {
    buckets_.resize(new_size);
    return;
  }
  std::vector<std::optional<typename std::forward_list<T>::iterator>> new_buckets(new_size);
  std::forward_list<T> new_elements;
  for (auto i : elements_) {
    size_t hash = hasher_(i) % new_size;
    if (!new_buckets[hash].has_value()) {
      if (new_elements.empty()) {
        new_elements.push_front(i);
      } else {
        size_t replace_hash = hasher_(*new_elements.begin()) % new_size;
        new_elements.push_front(i);
        new_buckets[replace_hash] = new_elements.begin();
      }
      new_buckets[hash] = new_elements.before_begin();
    } else {
      new_elements.insert_after(new_buckets[hash].value(), i);
    }
  }
  buckets_.clear();
  elements_.clear();
  auto invalidated = hasher_(new_elements.front()) % new_size;
  buckets_.swap(new_buckets);
  elements_.splice_after(elements_.before_begin(), new_elements);
  buckets_[invalidated] = elements_.before_begin();
}

template <typename T, typename Hasher, typename KeyEqual>
float UnorderedSet<T, Hasher, KeyEqual>::LoadFactor() const {
  return table_size_ == 0 ? 0 : static_cast<float>(table_size_) / static_cast<float>(buckets_.size());
}

template <typename T, typename Hasher, typename KeyEqual>
void UnorderedSet<T, Hasher, KeyEqual>::Erase(const T& value) {
  if (buckets_.empty()) {
    return;
  }
  size_t search_hash = hasher_(value) % buckets_.size();
  if (!buckets_[search_hash].has_value()) {
    return;
  }
  auto it = std::next(buckets_[search_hash].value());
  auto prev_it = buckets_[search_hash].value();
  while (it != elements_.end() && Hash(*it) == search_hash) {
    if (key_equal_(*it, value)) {
      --table_size_;
      size_t deleted_hash = hasher_(value) % buckets_.size();
      if (prev_it == elements_.before_begin()) {

        auto next_it = std::next(it);
        if (next_it != elements_.end() && Hash(*next_it) != deleted_hash) {
          buckets_[deleted_hash].reset();
          elements_.erase_after(prev_it);
          buckets_[Hash(*next_it)] = elements_.before_begin();
          return;
        }
        if (next_it == elements_.end()) {
          buckets_[deleted_hash].reset();
          elements_.erase_after(prev_it);
          return;
        }
        buckets_[deleted_hash].reset();
        elements_.erase_after(prev_it);
        buckets_[Hash(*elements_.begin())] = elements_.before_begin();
      } else {

        if (std::next(it) != elements_.end() && Hash(*std::next(it)) == deleted_hash &&
            deleted_hash == Hash(*prev_it)) {
          elements_.erase_after(prev_it);
          return;
        }

        if (std::next(it) == elements_.end()) {
          if (Hash(*prev_it) != deleted_hash) {
            buckets_[deleted_hash].reset();
          }
          elements_.erase_after(prev_it);
          return;
        }

        // case if the element has unique hash
        if (Hash(*prev_it) != deleted_hash && deleted_hash != Hash(*std::next(it))) {
          buckets_[deleted_hash].reset();
          buckets_[Hash(*std::next(it))].reset();
          elements_.erase_after(prev_it);
          buckets_[Hash(*std::next(prev_it))] = prev_it;
          return;
        }

        // if value is the bucket end
        if (deleted_hash != Hash(*std::next(it))) {
          buckets_[Hash(*std::next(it))].reset();
          elements_.erase_after(prev_it);
          buckets_[Hash(*std::next(prev_it))] = prev_it;
          return;
        }

        // if value hash is equal to prev_it hash
        if (deleted_hash == Hash(*std::next(it)) && deleted_hash != Hash(*prev_it)) {
          buckets_[deleted_hash].reset();
          elements_.erase_after(prev_it);
          buckets_[Hash(*std::next(prev_it))] = prev_it;
        }
      }
      return;
    }
    ++it;
    ++prev_it;
  }
}

template <typename T, typename Hasher, typename KeyEqual>
void UnorderedSet<T, Hasher, KeyEqual>::Insert(T&& value) {
  if (LoadFactor() >= 1 || buckets_.empty()) {
    if (buckets_.empty()) {
      buckets_.resize(1);
    } else {
      TotalRehash(2 * buckets_.size());
    }
  }
  if (!Find(value)) {
    ++table_size_;
    size_t hash = hasher_(value) % buckets_.size();
    if (!buckets_[hash].has_value()) {
      if (elements_.empty()) {
        elements_.push_front(std::move(value));
      } else {
        size_t replace_hash = hasher_(*elements_.begin()) % buckets_.size();
        elements_.push_front(std::move(value));
        buckets_[replace_hash] = elements_.begin();
      }
      buckets_[hash] = elements_.before_begin();
    } else {
      elements_.insert_after(buckets_[hash].value(), std::move(value));
    }
  }
}

template <typename T, typename Hasher, typename KeyEqual>
void UnorderedSet<T, Hasher, KeyEqual>::Insert(const T& value) {
  if (LoadFactor() >= 1 || buckets_.empty()) {
    if (buckets_.empty()) {
      buckets_.resize(1);
    } else {
      TotalRehash(2 * buckets_.size());
    }
  }
  if (!Find(value)) {
    ++table_size_;
    size_t hash = hasher_(value) % buckets_.size();
    if (!buckets_[hash].has_value()) {
      if (elements_.empty()) {
        elements_.push_front(value);
      } else {
        size_t replace_hash = hasher_(*elements_.begin()) % buckets_.size();
        elements_.push_front(value);
        buckets_[replace_hash] = elements_.begin();
      }
      buckets_[hash] = elements_.before_begin();
    } else {
      elements_.insert_after(buckets_[hash].value(), value);
    }
  }
}

template <typename T, typename Hasher, typename KeyEqual>
void UnorderedSet<T, Hasher, KeyEqual>::Clear() {
  buckets_.clear();
  elements_.clear();
  table_size_ = 0;
}

template <typename T, typename Hasher, typename KeyEqual>
bool UnorderedSet<T, Hasher, KeyEqual>::Empty() const {
  return table_size_ == 0;
}

template <typename T, typename Hasher, typename KeyEqual>
size_t UnorderedSet<T, Hasher, KeyEqual>::Size() const {
  return table_size_;
}

template <typename T, typename Hasher, typename KeyEqual>
UnorderedSet<T, Hasher, KeyEqual>& UnorderedSet<T, Hasher, KeyEqual>::operator=(UnorderedSet&& hashset) {
  if (this == &hashset) {
    return *this;
  }

  auto invalidated = hasher_(hashset.elements_.front()) % hashset.buckets_.size();
  buckets_ = std::move(hashset.buckets_);
  elements_ = std::move(hashset.elements_);
  buckets_[invalidated] = elements_.before_begin();
  table_size_ = hashset.table_size_;
  hashset.table_size_ = 0;
  return *this;
}

template <typename T, typename Hasher, typename KeyEqual>
UnorderedSet<T, Hasher, KeyEqual>& UnorderedSet<T, Hasher, KeyEqual>::operator=(const UnorderedSet& hashset) {
  if (this == &hashset) {
    return *this;
  }

  buckets_ = hashset.buckets_;
  elements_ = hashset.elements_;
  table_size_ = hashset.table_size_;
  return *this;
}

template <typename T, typename Hasher, typename KeyEqual>
UnorderedSet<T, Hasher, KeyEqual>::UnorderedSet(UnorderedSet&& hashset) {
  auto invalidated = hasher_(hashset.elements_.front()) % hashset.buckets_.size();
  buckets_ = std::move(hashset.buckets_);
  elements_ = std::move(hashset.elements_);
  buckets_[invalidated] = elements_.before_begin();
  table_size_ = hashset.table_size_;
  hashset.table_size_ = 0;
}

template <typename T, typename Hasher, typename KeyEqual>
UnorderedSet<T, Hasher, KeyEqual>::UnorderedSet(const UnorderedSet& hashset) {
  buckets_ = hashset.buckets_;
  elements_ = hashset.elements_;
  table_size_ = hashset.table_size_;
}

template <typename T, typename Hasher, typename KeyEqual>
UnorderedSet<T, Hasher, KeyEqual>::UnorderedSet(typename std::forward_list<T>::iterator begin,
                                                typename std::forward_list<T>::iterator end)
    : buckets_(std::distance(begin, end)) {
  table_size_ = 0;
  for (auto i = begin; i != end; ++i) {
    Insert(*i);
  }
}

template <typename T, typename Hasher, typename KeyEqual>
UnorderedSet<T, Hasher, KeyEqual>::UnorderedSet(size_t count)
    : buckets_(count), table_size_(0), hasher_(Hasher()), key_equal_(KeyEqual()) {
}

template <typename T, typename Hasher, typename KeyEqual>
UnorderedSet<T, Hasher, KeyEqual>::UnorderedSet() : table_size_(0), hasher_(Hasher()), key_equal_(KeyEqual()) {
}

#endif
*/