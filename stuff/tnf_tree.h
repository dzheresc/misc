#ifndef STUFF_TNF_TREE_H
#define STUFF_TNF_TREE_H

#include <queue>

#include <functional>
//#include <utility>

namespace tnf {
template <class T> class list {
  struct link {
    T val;
    link *next;

    explicit link(T _v, link *_n) : val(_v), next(_n) {}
  };

  link *head, *tail, *cur;

public:
  using PrintFunction = std::function<void(T &)>;

  list() { head = tail = cur = nullptr; }

  // add first
  void push_front(T v) {
    link *p = new link(v, head);
    cur = head = p;
    if (!tail)
      tail = p;
  }

  // add last
  void push_back(T v) {
    link *p = new link(v, nullptr);
    if (tail)
      tail->next = p;
    tail = p;
    if (!head)
      cur = head = p;
  }

  // insert after current position
  void insert(T v) {
    if (!cur)
      push_front(v);
    else {
      link *p = new link(v, cur->next);
      cur->next = p;
    }
  }

  void reverse() {
    tail = head;
    // current, prev, temp
    link *c = head, *p = 0, *t = 0;
    while (c) {
      t = c->next;
      c->next = p;
      p = c;
      c = t;
    }

    head = p;
  }

  void rewind() { cur = head; }

  T next() {
    if (!cur)
      return 0;
    T v = cur->val;
    cur = cur->next;
    return v;
  }

  void print(PrintFunction pf) {
    rewind();
    for (link *p = head; p; p = p->next)
      pf(p->val);
  }
};

template <class T> class lrtree {
  struct node {
    T val;
    int dups;

    node *left, *right;

    node(T _v, node *_l = nullptr, node *_r = nullptr)
        : val(_v), left(_l), right(_r), dups(0) {}
  };

  node *root;

public:
  using PrintFunction = std::function<void(T &)>;

  lrtree() { root = nullptr; }

  void push(T v) {
    if (!root) {
      root = new node(v);
      return;
    }

    node *t = root;
    while (t) {
      if (v == t->val) {
        t->dups++;
        return;
      }
      if (v < t->val) {
        if (t->left)
          t = t->left;
        else {
          t->left = new node(v);
          return;
        }
      } else // go right
      {
        if (t->right)
          t = t->right;
        else {
          t->right = new node(v);
          return;
        }
      }
    }
  }

  void preorder(node *t, PrintFunction pf) {
    if (!t)
      return;

    // node, left, right
    pf(t->val);
    preorder(t->left, pf);
    preorder(t->right, pf);
  }

  void preorder(PrintFunction pf) { preorder(root, pf); }

  void inorder(node *t, PrintFunction pf) {
    if (!t)
      return;

    // left, node, right
    inorder(t->left, pf);
    pf(t->val);
    inorder(t->right, pf);
  }
  void inorder(PrintFunction pf) { inorder(root, pf); }

  void postorder(node *t, PrintFunction pf) {
    if (!t)
      return;

    // left, right, node
    postorder(t->left, pf);
    postorder(t->right, pf);
    pf(t->val);
  }

  void postorder(PrintFunction pf) { postorder(root, pf); }

  void bfs(PrintFunction pf) {
    std::queue<node *> q;
    // head, left, right
    q.push(root);
    while (!q.empty()) {
      node *t = q.front();
      q.pop();
      pf(t->val);
      if (t->left)
        q.push(t->left);
      if (t->right)
        q.push(t->right);
    }
  }
};

} // namespace tnf
#endif // STUFF_TNF_TREE_H
