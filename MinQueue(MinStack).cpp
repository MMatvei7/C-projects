/*#include <iostream>
#include <cstring>

// create what Stack and Deque consist of
struct Node {
  Node* prev = nullptr;
  int64_t key = 0;
  int64_t min_to_key = 0;
};
// create min Stack: we need to contain 2 values in 1 Node: key
// and min key for all stack from the beginning to current moment
class MinStack {
 private:
  Node* top_ = nullptr;
  Node* root_ = nullptr;
  size_t size_ = 0;

 public:
  // add new node
  void Push(int64_t value) {
    auto new_top = new Node;
    new_top->key = value;
    // count current minimum
    new_top->min_to_key = size_ > 0 ? std::min(top_->min_to_key, value) : value;
    // wanna do new_top = &top_node
    new_top->prev = top_;
    // wanna do top = &new_top_node
    top_ = new_top;
    if (size_ == 0) {
      root_ = new_top;
    }
    ++size_;
  }
  // remove last element from stack
  void Pop() {
    if (size_ > 0) {
      // create to pointers to the top_node cause we cannot forget last node (we need delete it)
      Node* old_top = top_;
      --size_;
      top_ = top_->prev;
      delete old_top;
    }
  }
  // get back element
  int64_t Back() {
    return top_->key;
  }
  int64_t GetFront() {
    return root_->key;
  }
  // get stack size
  size_t Size() {
    return size_;
  }
  // clear stack
  void Clear() {
    while (top_ != nullptr) {
      Pop();
    }
  }
  // get minimum for O(1)
  int64_t GetMin() {
    return top_->min_to_key;
  }
  // delete last elements before finishing programm
  void Exit() {
    if (size_ > 0) {
      Clear();
    }
  }
};

// we will add new elements exactly to stack_back and pop elements only
// from stack_front: if stack_front is empty - then remove all from stack_back to stack_front
// it will not ruin Queue property cause all elements will be reversed
class MinQueue {
 private:
  MinStack* stack_front_ = new MinStack;
  MinStack* stack_back_ = new MinStack;

 public:
  // push to minqueue
  void Enqueue(int64_t value) {
    stack_back_->Push(value);
  }
  // remove front element from queue
  void Dequeue() {
    if (stack_front_->Size() > 0) {
      auto ans = stack_front_->Back();
      stack_front_->Pop();
      std::cout << ans << '\n';
    } else {
      while (stack_back_->Size() != 0) {
        stack_front_->Push(stack_back_->Back());
        stack_back_->Pop();
      }
      std::cout << stack_front_->Back() << '\n';
      stack_front_->Pop();
    }
  }
  // get front element
  int64_t Front() {
    if (stack_front_->Size() != 0) {
      return stack_front_->Back();
    }
    return stack_back_->GetFront();
  }
  // get queue size
  size_t Size() {
    return stack_back_->Size() + stack_front_->Size();
  }
  // get min
  int64_t GetQueueMin() {
    if (stack_front_->Size() == 0 || stack_back_->Size() == 0) {
      return stack_back_->Size() > 0 ? stack_back_->GetMin() : stack_front_->GetMin();
    }
    return std::min(stack_back_->GetMin(), stack_front_->GetMin());
  }
  // clear queue
  void Clear() {
    while (stack_back_->Size() != 0) {
      stack_back_->Pop();
    }
    while (stack_front_->Size() != 0) {
      stack_front_->Pop();
    }
  }
  void Exit() {
    delete stack_back_;
    delete stack_front_;
  }
};

int main() {
  int32_t m;
  int64_t a;
  std::cin >> m;
  auto priority_queue = new MinQueue;
  auto s = new char[100]{};
  for (int32_t i = 0; i < m; ++i) {
    scanf("%s", s);
    if (strcmp(s, "enqueue") == 0) {
      std::cin >> a;
      priority_queue->Enqueue(a);
      std::cout << "ok" << '\n';
    }
    if (strcmp(s, "dequeue") == 0) {
      if (priority_queue->Size() == 0) {
        std::cout << "error" << '\n';
      } else {
        priority_queue->Dequeue();
      }
    }
    if (strcmp(s, "front") == 0) {
      if (priority_queue->Size() == 0) {
        std::cout << "error" << '\n';
      } else {
        std::cout << priority_queue->Front() << '\n';
      }
    }
    if (strcmp(s, "size") == 0) {
      std::cout << priority_queue->Size() << '\n';
    }
    if (strcmp(s, "clear") == 0) {
      priority_queue->Clear();
      std::cout << "ok" << '\n';
    }
    if (strcmp(s, "min") == 0) {
      if (priority_queue->Size() == 0) {
        std::cout << "error" << '\n';
      } else {
        std::cout << priority_queue->GetQueueMin() << '\n';
      }
    }
  }
  priority_queue->Clear();
  priority_queue->Exit();
  delete priority_queue;
  delete[] s;
  return 0;
}
*/
