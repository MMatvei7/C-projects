/*#include <iostream>
#include <cstring>

// create what Stack and Deque consist of
struct Node {
  Node* prev = nullptr;
  Node* next = nullptr;
  int64_t key = 0;
};
// create Deque
class Deque {
 private:
  Node* top_ = nullptr;
  Node* root_ = nullptr;
  size_t size_ = 0;

 public:
  // add first node to deque
  void AddFirst(const int64_t& value) {
    auto new_node = new Node;
    new_node->key = value;
    top_ = new_node;
    root_ = new_node;
  }
  // add new element at the end of deque
  void PushBack(const int64_t& value) {
    ++size_;
    if (top_ == nullptr) {
      AddFirst(value);
      return;
    }
    // we wanna add last in queue
    // we need to make link with the current last and conversely
    // current is below
    auto new_node = new Node;
    new_node->key = value;
    new_node->prev = top_;
    top_->next = new_node;
    top_ = new_node;
  }
  void PushFront(const int64_t& value) {
    ++size_;
    if (root_ == nullptr) {
      AddFirst(value);
      return;
    }
    // we wanna add head in queue
    // we need to make link with the current last and conversely
    // current is above
    auto new_node = new Node;
    new_node->key = value;
    new_node->next = root_;
    root_->prev = new_node;
    root_ = new_node;
  }
  // delete the oldest element
  void PopBack() {
    if (size_ == 0) {
      return;
    }
    if (size_ == 1) {
      delete top_;
      root_ = nullptr;
      top_ = nullptr;
      --size_;
      return;
    }
    --size_;
    auto tmp_prev = top_->prev;
    tmp_prev->next = nullptr;
    delete top_;
    top_ = tmp_prev;
  }

  // delete the newest element
  void PopFront() {
    if (size_ == 0) {
      return;
    }
    if (size_ == 1) {
      delete top_;
      root_ = nullptr;
      top_ = nullptr;
      --size_;
      return;
    }
    --size_;
    auto tmp_next = root_->next;
    tmp_next->prev = nullptr;
    delete root_;
    root_ = tmp_next;
  }
  // get begin_value
  void Front() {
    if (size_ == 0) {
      std::cout << "error" << '\n';
      return;
    }
    std::cout << root_->key << '\n';
  }
  void Back() {
    if (size_ == 0) {
      std::cout << "error" << '\n';
      return;
    }
    std::cout << top_->key << '\n';
  }
  void Size() {
    std::cout << size_ << '\n';
  }
  void Clear() {
    while (size_ > 0) {
      PopBack();
    }
  }
  void Exit() {
    Clear();
  }
};

int main() {
  int32_t k;
  int32_t a;
  std::cin >> k;
  Deque priority_queue;
  auto s = new char[100]{};
  for (int32_t i = 0; i < k; ++i) {
    scanf("%s", s);
    if (strcmp(s, "push_back") == 0) {
      std::cin >> a;
      priority_queue.PushBack(a);
      std::cout << "ok" << '\n';
    }
    if (strcmp(s, "push_front") == 0) {
      std::cin >> a;
      priority_queue.PushFront(a);
      std::cout << "ok" << '\n';
    }
    if (strcmp(s, "pop_back") == 0) {
      priority_queue.Back();
      priority_queue.PopBack();
    }
    if (strcmp(s, "pop_front") == 0) {
      priority_queue.Front();
      priority_queue.PopFront();
    }
    if (strcmp(s, "back") == 0) {
      priority_queue.Back();
    }
    if (strcmp(s, "front") == 0) {
      priority_queue.Front();
    }
    if (strcmp(s, "size") == 0) {
      priority_queue.Size();
    }
    if (strcmp(s, "clear") == 0) {
      priority_queue.Clear();
      std::cout << "ok" << '\n';
    }
    if (strcmp(s, "exit") == 0) {
      priority_queue.Exit();
      std::cout << "bye" << '\n';
      delete[] s;
      return 0;
    }
  }
  priority_queue.Exit();
  delete[] s;
  return 0;
}
*/
