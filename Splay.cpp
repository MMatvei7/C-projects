/*#include <iostream>

// Splay tree realization
// most common requests are at the root of the tree
// every request should be at the root of the tree

struct Node {
  Node* left = nullptr;
  Node* right = nullptr;
  Node* parent = nullptr;
  int64_t key = 0;
};

struct DividedTrees {
  Node* left_root = nullptr;
  Node* right_root = nullptr;
};

class SplayTree {
 private:
  Node* root_ = nullptr;

 public:
  // need to set parent of current node
  void SetParent(Node*, Node*);
  // set parent to both nodes
  void KeepParent(Node*);
  // zig - to answer request we wanna have it exactly in the root
  // so we need to do left or right rotate
  void Rotate(Node*, Node*);
  // to raise request ver we need to combine zig, zig-zig, zig-zag
  Node* Splay(Node*);
  // find node by key
  Node* Find(Node*, int64_t);
  // if we wanna insert some value we need to divide tree on 2 parts:
  // left subtree must be at most value and right subtree at least value
  // it is comfortable cause we wanna put value to the tree root
  // then we need structure where we will contain 2 roots;
  DividedTrees* Split(DividedTrees*, Node*, int64_t);
  // add new element to Splay tree
  Node* Insert(Node*, int64_t);
  // merge 2 sunbtrees
  // due to BST property right subtree min will be at most all right suntree elements
  // and at least all left subtree elements
  Node* Merge(Node*, Node*);
  // to remove element we need to move it to the root and then merge left and right subtrees
  Node* Erase(Node*, int64_t);
  // get min elem
  int64_t Next(Node*, int64_t);
  int64_t FindBigger(Node*, int64_t, int64_t);
  // clear tree
  void ClearTree(Node*);
  // sort array
  void InorderTreeWalk(Node*);
  // make set from tree
  Node* AddUnique(Node*, int64_t);
  // get root
  Node* Root() {
    return root_;
  }
  // set root
  void SetRoot(Node* root) {
    root_ = root;
  }
};

void SplayTree::SetParent(Node* child, Node* parent) {
  if (child != nullptr) {
    child->parent = parent;
  }
}

void SplayTree::KeepParent(Node* ver) {
  SetParent(ver->left, ver);
  SetParent(ver->right, ver);
}

void SplayTree::Rotate(Node* child, Node* parent) {
  auto grand_parent = parent->parent;
  if (grand_parent != nullptr) {
    if (grand_parent->left == parent) {
      grand_parent->left = child;
    } else {
      grand_parent->right = child;
    }
  }
  if (parent->left == child) {
    // right rotate
    parent->left = child->right;
    child->right = parent;
  } else {
    // left rotate
    parent->right = child->left;
    child->left = parent;
  }
  // set final links
  KeepParent(child);
  KeepParent(parent);
  SetParent(child, grand_parent);
}

Node* SplayTree::Splay(Node* ver) {
  if (ver->parent == nullptr) {
    SetRoot(ver);
    return ver;
  }
  auto parent = ver->parent;
  auto grand_parent = parent->parent;
  if (grand_parent == nullptr) {
    // then zig is enough
    Rotate(ver, parent);
    SetRoot(ver);
    return ver;
  }
  // zig-zig or zig-zag
  if ((grand_parent->left == parent && parent->left == ver) ||
      (grand_parent->right == parent && parent->right == ver)) {
    // then we have to do two right rotates
    Rotate(parent, grand_parent);
    Rotate(ver, parent);
  } else {
    // then at first we need left(right) rotate and then right(left) rotate
    Rotate(ver, parent);
    Rotate(ver, grand_parent);
  }
  // and this configurations until we have ver at the tree root
  return Splay(ver);
}

Node* SplayTree::Find(Node* ver, int64_t value) {
  if (ver == nullptr) {
    return nullptr;
  }
  if (ver->key == value) {
    // cause we wanna recent request at the root of the tree
    return Splay(ver);
  }
  if (value < ver->key && ver->left != nullptr) {
    return Find(ver->left, value);
  }
  if (value > ver->key && ver->right != nullptr) {
    return Find(ver->right, value);
  }
  // we consider find as a request so we wanna ver(if we've found it) at the tree root
  // and other near keys will be closer to the root
  return Splay(ver);
}

DividedTrees* SplayTree::Split(DividedTrees* two_trees, Node* root, int64_t value) {
  if (root == nullptr) {
    two_trees->left_root = nullptr;
    two_trees->right_root = nullptr;
    SetRoot(root);
    return two_trees;
  }
  root = Find(root, value);
  if (root->key == value) {
    SetParent(root->left, nullptr);
    SetParent(root->right, nullptr);
    two_trees->left_root = root->left;
    two_trees->right_root = root->right;
    delete root;
    SetRoot(root);
    return two_trees;
  }
  if (root->key > value) {
    auto left = root->left;
    root->left = nullptr;
    SetParent(left, nullptr);
    two_trees->left_root = left;
    two_trees->right_root = root;
    SetRoot(root);
    return two_trees;
  }
  auto right = root->right;
  root->right = nullptr;
  SetParent(right, nullptr);
  two_trees->left_root = root;
  two_trees->right_root = right;
  SetRoot(root);
  return two_trees;
}

Node* SplayTree::Insert(Node* root, int64_t value) {
  auto two_trees = new DividedTrees;
  two_trees = Split(two_trees, root, value);
  auto left = two_trees->left_root;
  auto right = two_trees->right_root;
  auto new_root = new Node;
  new_root->left = left;
  new_root->right = right;
  new_root->key = value;
  KeepParent(new_root);
  SetRoot(new_root);
  delete two_trees;
  return new_root;
}

Node* SplayTree::Merge(Node* left, Node* right) {
  if (right == nullptr) {
    SetRoot(left);
    return left;
  }
  if (left == nullptr) {
    SetRoot(right);
    return right;
  }
  // min right elem become right tree root
  right = Find(right, left->key);
  right->left = left;
  left->parent = right;
  SetRoot(right);
  return right;
}

Node* SplayTree::Erase(Node* root, int64_t value) {
  root = Find(root, value);
  SetParent(root->left, nullptr);
  SetParent(root->right, nullptr);
  auto left = root->left;
  auto right = root->right;
  delete root;
  return Merge(left, right);
}

void SplayTree::InorderTreeWalk(Node* root) {
  if (root != nullptr) {
    InorderTreeWalk(root->left);
    std::cout << root->key << " ";
    InorderTreeWalk(root->right);
  }
}

// return min elemenet >= i
int64_t SplayTree::FindBigger(Node* root, int64_t k, int64_t l) {
  if (root == nullptr) {
    return l;
  }
  if (root->key >= k) {
    // if we go left we may not find appropriate element so we need to save current which is appropriate
    return FindBigger(root->left, k, root->key);
  }
  return FindBigger(root->right, k, l);
}

int64_t SplayTree::Next(Node* root, int64_t k) {
  if (Find(root, k) != nullptr && Find(root, k)->key == k) {
    return k;
  }
  return FindBigger(root, k, -1);
}

void SplayTree::ClearTree(Node* root) {
  if (root == nullptr) {
    return;
  }
  if (root->left != nullptr) {
    ClearTree(root->left);
  }
  if (root->right != nullptr) {
    ClearTree(root->right);
  }
  delete root;
}

int main() {
  int32_t n;
  const int64_t mod = 1000000000;
  int64_t save_value = 0;
  std::cin >> n;
  auto arr = new int64_t[n];
  SplayTree tree;
  auto oredered_sequance = new char[n];
  char operation;
  for (int32_t i = 0; i < n; ++i) {
    std::cin >> operation >> arr[i];
    oredered_sequance[i] = operation;
    if (operation == '?') {
      tree.Find(tree.Root(), arr[i]);
      save_value = tree.Next(tree.Root(), arr[i]);
      std::cout << save_value << '\n';
    } else if ((operation == '+' && (i - 1 >= 0 && oredered_sequance[i - 1] == '+')) || i == 0) {
      tree.Insert(tree.Root(), arr[i]);
    } else {
      tree.Insert(tree.Root(), (arr[i] + save_value) % mod);
    }
  }
  // tree.InorderTreeWalk(tree.Root());
  tree.ClearTree(tree.Root());
  delete[] oredered_sequance;
  delete[] arr;
  return 0;
}
*/
