/*#include <iostream>

// Binary Search Tree (BST) realization

struct Node {
  Node* left = nullptr;
  Node* right = nullptr;
  Node* parent = nullptr;
  int64_t key = 0;
};

struct BST {
 private:
  Node* root_ = nullptr;

 public:
  // to sort array
  void InorderTreeWalk(Node*);
  // casual walk
  void PreorderTreeWalk(Node*);
  // reversed walk
  void PostorderTreeWalk(Node*);
  // find value in the BST
  Node* Find(Node*, int64_t);
  // find minimum
  Node* Minimum(Node*);
  // add new element
  void Insert(Node* node);
  // clear tree
  void ClearTree(Node*);
  // get root
  Node* GetRoot() {
    return root_;
  }
  // get tree height
  int64_t Height(Node*);
  // get leafs
  void GetLeafs(Node*);
  // get parents with both children
  void GetFullParent(Node*);
};

void BST::InorderTreeWalk(Node* root) {
  if (root != nullptr) {
    InorderTreeWalk(root->left);
    std::cout << root->key << " ";
    InorderTreeWalk(root->right);
  }
}

void BST::PreorderTreeWalk(Node* root) {
  if (root != nullptr) {
    std::cout << root->key << " ";
    PreorderTreeWalk(root->left);
    PreorderTreeWalk(root->right);
  }
}

void BST::PostorderTreeWalk(Node* root) {
  if (root != nullptr) {
    PostorderTreeWalk(root->left);
    PostorderTreeWalk(root->right);
    std::cout << root->key << " ";
  }
}

Node* BST::Find(Node* root, int64_t value) {
  if (root == nullptr || root->key == value) {
    return root;
  }
  if (root->key > value) {
    return Find(root->left, value);
  }
  return Find(root->right, value);
}

Node* BST::Minimum(Node* root) {
  if (root == nullptr || root->left == nullptr) {
    return root;
  }
  return Minimum(root->left);
}

void BST::Insert(Node* node) {
  Node* parent = nullptr;
  Node* current = root_;
  // find appropriate place for node
  while (current != nullptr) {
    parent = current;
    if (node->key < current->key) {
      current = current->left;
    } else {
      current = current->right;
    }
  }
  node->parent = parent;
  // set last links
  if (parent == nullptr) {
    root_ = node;
  } else if (node->key < parent->key) {
    parent->left = node;
  } else {
    parent->right = node;
  }
}

int64_t BST::Height(Node* root) {
  if (root == nullptr) {
    return 0;
  }
  auto left_subtree = Height(root->left);
  auto right_subtree = Height(root->right);
  return std::max(left_subtree, right_subtree) + 1;
}

void BST::GetLeafs(Node* root) {
  if (root != nullptr) {
    GetLeafs(root->left);
    if (root->left == nullptr && root->right == nullptr) {
      std::cout << root->key << " ";
    }
    GetLeafs(root->right);
  }
}

void BST::GetFullParent(Node* root) {
  if (root != nullptr) {
    GetFullParent(root->left);
    if (root->left != nullptr && root->right != nullptr) {
      std::cout << root->key << '\n';
    }
    GetFullParent(root->right);
  }
}

void BST::ClearTree(Node* root) {
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
  int64_t n;
  std::cin >> n;
  BST tree;
  while (n != 0) {
    auto x = new Node;
    x->key = n;
    if (tree.Find(tree.GetRoot(), n) == nullptr) {
      tree.Insert(x);
    } else {
      delete x;
    }
    std::cin >> n;
  }
  tree.GetFullParent(tree.GetRoot());
  tree.ClearTree(tree.GetRoot());
  return 0;
}
*/
