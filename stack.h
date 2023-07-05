#ifndef STACK__STACK_H_
#define STACK__STACK_H_
#include <deque>

template <typename T, typename Container = std::deque<T>>
class Stack {
 private:
  Container container_;

 public:
  Stack() = default;
  explicit Stack(const Container&);
  template <typename InputIter>
  Stack(InputIter begin, InputIter end);
  const T& Top() const;
  T& Top();
  bool Empty() const;
  size_t Size() const;
  void Push(const T&);
  void Push(T&&);
  void Pop();
  void Swap(Stack&);
  template <typename... Args>
  void Emplace(Args&&... args);
};

template <typename T, typename Container>
Stack<T, Container>::Stack(const Container& other) : container_(other) {
}

template <typename T, typename Container>
template <typename InputIter>
Stack<T, Container>::Stack(InputIter begin, InputIter end) {
  for (auto it = begin; it != end; ++it) {
    container_.push_back(*it);
  }
}

template <typename T, typename Container>
const T& Stack<T, Container>::Top() const {
  return container_.back();
}

template <typename T, typename Container>
T& Stack<T, Container>::Top() {
  return container_.back();
}

template <typename T, typename Container>
bool Stack<T, Container>::Empty() const {
  return container_.empty();
}

template <typename T, typename Container>
size_t Stack<T, Container>::Size() const {
  return container_.size();
}

template <typename T, typename Container>
void Stack<T, Container>::Push(const T& value) {
  container_.push_back(value);
}

template <typename T, typename Container>
void Stack<T, Container>::Push(T&& value) {
  container_.push_back(std::move(value));
}

template <typename T, typename Container>
void Stack<T, Container>::Pop() {
  container_.pop_back();
}

template <typename T, typename Container>
void Stack<T, Container>::Swap(Stack& other) {
  container_.swap(other.container_);
}

template <typename T, typename Container>
template <typename... Args>
void Stack<T, Container>::Emplace(Args&&... args) {
  container_.emplace_back(std::forward<Args>(args)...);
}

#endif
