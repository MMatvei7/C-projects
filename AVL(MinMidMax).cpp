/*#include <iostream>
#include <cstring>

// Realization of AVL - tree

// first of all we need to contain tree components - nodes
// each node consists of 4 parts: key - value, 2 pointers to left and right chilldren and height of the tree
// with root at this node
struct Node {
  int64_t key;
  int64_t height = 1;
  int64_t left_subtree_size = 0;
  int64_t right_subtree_size = 0;
  Node* left = nullptr;
  Node* right = nullptr;
};

class AVLTree {
 private:
  Node* root_ = nullptr;
  size_t size_ = 0;

 public:
  int64_t LeftSubtreeSize(Node*);
  int64_t RightSubtreeSize(Node*);
  int64_t Height(Node*);
  int64_t BalanceFactor(Node*);
  void FixHeight(Node*);
  Node* RotateRight(Node*);
  Node* RotateLeft(Node*);
  Node* BigBalance(Node*);
  Node* Insert(Node*, int64_t);
  Node* EraseMin(Node*);
  Node* EraseMax(Node*);
  Node* Minimum(Node*);
  Node* Maximum(Node*);
  Node* Middle(Node*, int64_t);
  Node* Erase(Node*, int64_t);
  void ClearTree(Node*);
  void InorderTreeWalk(Node*);
  Node* Find(Node*, int64_t);
  Node* Add(Node*, int64_t);
  int64_t FindBigger(Node*, int64_t, int64_t);
  int64_t Next(Node*, int64_t);
  void ClearRoot() {
    delete root_;
  }
  Node* GetRoot() {
    return root_;
  }
  void SetRoot(Node* root) {
    root_ = root;
  }
  size_t GetSize() {
    return size_;
  }
  void SetSize(size_t size) {
    size_ = size;
  }
  void ClearT() {
    ClearTree(root_);
    root_ = nullptr;
  }
};
// get left subtree size
int64_t AVLTree::LeftSubtreeSize(Node* node) {
  return node == nullptr ? 0 : node->left_subtree_size;
}
// get right subtree size
int64_t AVLTree::RightSubtreeSize(Node* node) {
  return node == nullptr ? 0 : node->right_subtree_size;
}
// get height of the subtree
int64_t AVLTree::Height(Node* node) {
  return node == nullptr ? 0 : node->height;
}
// cause AVL - tree is balance tree we need to control heights of left and right subtrees
// for each node of AVL - tree the height difference between left and right subtrees cannot be more than 1
int64_t AVLTree::BalanceFactor(Node* node) {
  return Height(node->right) - Height(node->left);
}
// definetely there can be bad situation: when |balance factor| > 1 - so we need to fix height of tree
void AVLTree::FixHeight(Node* node) {
  // get height of left subtree
  int64_t left_height = Height(node->left);
  // get height of right subtree
  int64_t right_height = Height(node->right);
  // get the height of tree with node as a root
  node->height = std::max(left_height, right_height) + 1;
  // get size of the left subtree
  auto left_node_l = LeftSubtreeSize(node->left);
  auto left_node_r = RightSubtreeSize(node->left);
  node->left_subtree_size = node->left == nullptr ? 0 : left_node_l + left_node_r + 1;
  // get size of the right subtree
  auto right_node_l = RightSubtreeSize(node->right);
  auto right_node_r = LeftSubtreeSize(node->right);
  node->right_subtree_size = node->right == nullptr ? 0 : right_node_l + right_node_r + 1;
}
// to fix the problem above we need special methods - rotates to support AVL - tree property
// right rotate if |bfactor| of left subtree > 1
Node* AVLTree::RotateRight(Node* p) {
  Node* q = p->left;
  p->left = q->right;
  q->right = p;
  // get new height of p and q
  FixHeight(p);
  FixHeight(q);
  return q;
}
// left rotate if |bfactor| of right subtree > 1
Node* AVLTree::RotateLeft(Node* q) {
  Node* p = q->right;
  q->right = p->left;
  p->left = q;
  // get new height of p and q
  FixHeight(q);
  FixHeight(p);
  return p;
}
// but it can be not enough so we have to use rotate combinations
// so we need to go deep in tree structure
Node* AVLTree::BigBalance(Node* p) {
  FixHeight(p);
  if (BalanceFactor(p) == 2) {
    // if balance is destroyed we have 3 variants of p->right disbalance
    // in 2 cases (bfactor == 0 or bfactor == 1) we can get rid of disbalance merely with left rotate
    if (BalanceFactor(p->right) == -1) {
      p->right = RotateRight(p->right);
    }
    return RotateLeft(p);
  }
  if (BalanceFactor(p) == -2) {
    // this is simmetrical situation
    if (BalanceFactor(p->left) == 1) {
      p->left = RotateLeft(p->left);
    }
    return RotateRight(p);
  }
  return p;
}
// Add node by insertion
Node* AVLTree::Insert(Node* r, int64_t k) {
  // wanna insert node r with r.key = k
  // if the tree is empty we need to create it
  if (r == nullptr) {
    auto* current = new Node;
    current->key = k;
    return current;
  }
  if (k < r->key) {
    // then we need to search place to insert in left subtree
    r->left = Insert(r->left, k);
  } else {
    // then we need to search place to insert in right subtree
    r->right = Insert(r->right, k);
  }
  // after insertion we can lose AVL property so we need to fix it
  // we call bigbalance only once cause r = root
  return BigBalance(r);
}
// erase min tree element
Node* AVLTree::EraseMin(Node* p) {
  // if we have only root we should return nullptr so return p->right
  // if we have only right child we should erase p so return p->right
  if (p->left == nullptr) {
    return p->right;
  }
  // we well go down left subtree and delete last element
  p->left = EraseMin(p->left);
  // we could disbalabce by this operations so we need to balabce it
  // we need to do it only once cause p = root
  return BigBalance(p);
}
// erase max tree element
Node* AVLTree::EraseMax(Node* p) {
  // if we have only root we should return nullptr so return p->right
  // if we have only right child we should erase p so return p->right
  if (p->right == nullptr) {
    return p->left;
  }
  // we well go down left subtree and delete last element
  p->right = EraseMax(p->right);
  // we could disbalabce by this operations so we need to balabce it
  // we need to do it only once cause p = root
  return BigBalance(p);
}
// get minimum of tree
Node* AVLTree::Minimum(Node* r) {
  if (r == nullptr || r->left == nullptr) {
    return r;
  }
  return Minimum(r->left);
}
// get maximum of tree
Node* AVLTree::Maximum(Node* r) {
  if (r == nullptr || r->right == nullptr) {
    return r;
  }
  return Maximum(r->right);
}
// get tree median
Node* AVLTree::Middle(Node* node, int64_t k) {
  // this is like kth order statistic
  // if idx(k) > number of elements which are smaller than node then
  // we need to search in right subtree, otherwise in left subtree
  if (node->left_subtree_size == k) {
    return node;
  }
  if (node->left_subtree_size < k) {
    return Middle(node->right, k - node->left_subtree_size - 1);
  }
  return Middle(node->left, k);
}
// erase random node from tree by key
Node* AVLTree::Erase(Node* root, int64_t k) {
  if (root == nullptr) {
    return root;
  }
  if (k < root->key) {
    root->left = Erase(root->left, k);
  } else if (k > root->key) {
    root->right = Erase(root->right, k);
  } else {
    // root->key = k - after some steps we've finally found it
    Node* l = root->left;
    Node* r = root->right;
    delete root;
    if (r == nullptr) {
      return l;
    }
    Node* min = Minimum(r);
    // to support AVL property we need to replace root by minimum in right subtree:
    // left subtree elements maintain are at most minimum and all right subtree elements are at least minimum
    // so minimum has 2 children: left subtree and right subtree
    min->right = EraseMin(r);
    min->left = l;
    // after we swapped minimum and root and delete root tree could get disbalanced cause right subtree height
    // maybe changed
    return BigBalance(min);
  }
  // after all recursion steps we need to balance root
  // so at the end we've balanced all subtrees till the root and we can claim that tree saved AVL property
  return BigBalance(root);
}
// need to delete tree at the end of process
void AVLTree::ClearTree(Node* root) {
  if (root != nullptr) {
    ClearTree(root->left);
    ClearTree(root->right);
    delete root;
  }
}
// get sorted array
void AVLTree::InorderTreeWalk(Node* root) {
  if (root != nullptr) {
    InorderTreeWalk(root->left);
    std::cout << root->key << " ";
    InorderTreeWalk(root->right);
  }
}
// find element if exists and return pointer to this node
Node* AVLTree::Find(Node* x, int64_t k) {
  if (x == nullptr || x->key == k) {
    return x;
  }
  if (x->key > k) {
    return Find(x->left, k);
  }
  return Find(x->right, k);
}
// add to tree if element is unique
Node* AVLTree::Add(Node* root, int64_t k) {
  if (Find(root, k) != nullptr) {
    return root;
  }
  return Insert(root, k);
}
// return min elemenet >= i
int64_t AVLTree::FindBigger(Node* root, int64_t k, int64_t l) {
  if (root == nullptr) {
    return l;
  }
  if (root->key >= k) {
    // if we go left we may not find appropriate element so we need to save current which is appropriate
    return FindBigger(root->left, k, root->key);
  }
  return FindBigger(root->right, k, l);
}

int64_t AVLTree::Next(Node* root, int64_t k) {
  if (Find(root, k) != nullptr) {
    return k;
  }
  return FindBigger(root, k, -1);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int32_t n;
  int32_t a;
  std::cin >> n;
  AVLTree tree;
  char s[100];
  for (int32_t i = 0; i < n; ++i) {
    std::cin >> s;
    if (strcmp(s, "add") == 0) {
      std::cin >> a;
      tree.SetRoot(tree.Insert(tree.GetRoot(), a));
      tree.SetSize(tree.GetSize() + 1);
      // std::cout << "ok" << '\n';
    }
    if (strcmp(s, "min") == 0) {
      if (tree.GetSize() == 0) {
        std::cout << "error" << '\n';
      } else {
        std::cout << tree.Minimum(tree.GetRoot())->key << '\n';
        tree.SetRoot(tree.Erase(tree.GetRoot(), tree.Minimum(tree.GetRoot())->key));
        tree.SetSize(tree.GetSize() - 1);
      }
    }
    if (strcmp(s, "max") == 0) {
      if (tree.GetSize() == 0) {
        std::cout << "error" << '\n';
      } else {
        std::cout << tree.Maximum(tree.GetRoot())->key << '\n';
        tree.SetRoot(tree.Erase(tree.GetRoot(), tree.Maximum(tree.GetRoot())->key));
        tree.SetSize(tree.GetSize() - 1);
      }
    }
    if (strcmp(s, "mid") == 0) {
      auto ans = tree.Middle(tree.GetRoot(), (tree.GetSize() - 1) / 2)->key;
      std::cout << ans << '\n';
      tree.SetRoot(tree.Erase(tree.GetRoot(), ans));
      tree.SetSize(tree.GetSize() - 1);
    }
  }
  tree.ClearT();
  return 0;
}
*/
