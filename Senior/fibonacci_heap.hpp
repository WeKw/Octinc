/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_FIBONACCI_HEAP_HPP
#  define OCTINC_FIBONACCI_HEAP_HPP
#  ifndef STD_BITS_STDCPP_H
#    define STD_BITS_STDCPP_H
#    include <bits/stdc++.h>
#  endif
#  if __cplusplus < 201103L && !defined(nullptr)
#    define nullptr NULL
#  endif

namespace Octinc {
template <typename __Tp, typename __Cmp = std::less<__Tp>,
          typename __Abs = std::less<__Tp> >
class fibonacci_heap
/*
__Cmp is the way to sort,
__Abs is the absolute sorting way.
*/
{
   private:
    int keyNum;
    int maxDegree;
    struct Node;
    Node *min;
    Node **cons;

   public:
    fibonacci_heap();

    struct iterator;

    iterator insert(__Tp key);
    /*
    Insert a node with key,
    and return the iterator of the node.
    */
    void pop();
    /*
    Delete the top of the heap.
    */
    void combine(fibonacci_heap<__Tp, __Cmp, __Abs> *other);
    /*
    Combine two heaps.
    */
    iterator top();
    /*
    Return the iterator of the top of the heap.
    */
    bool update(iterator, __Tp);
    /*
    Update the key of the node whose key was oldkey to newkey.
    */
    void remove(__Tp key);
    /*
    Remove the node whose key was the key.
    */
    bool contains(__Tp key);
    /*
    Return whether the heap contains the node whose key was the key.
    */

    size_t size();
    bool empty();
    void clear();

   private:
    void remove_node(Node *node);
    void insert_node(Node *node, Node *root);
    void cat_list(Node *a, Node *b);
    void insert(Node *node);
    Node *__pop();
    void link(Node *node, Node *root);
    void make_cons();
    void consolidate();
    void renew_deg(Node *parent, int degree);
    void cut(Node *node, Node *parent);
    void cas_cut(Node *node);
    bool decrease(Node *node, __Tp key);
    bool increase(Node *node, __Tp key);
    bool update(Node *node, __Tp key);
    Node *search(Node *root, __Tp key);
    Node *search(__Tp key);
    void remove(Node *node);
};

template <typename __Tp, typename __Cmp, typename __Abs>
struct fibonacci_heap<__Tp, __Cmp, __Abs>::Node {
   public:
    __Tp key;
    int degree;
    Node *left;
    Node *right;
    Node *child;
    Node *parent;
    bool marked;

    Node(__Tp value)
        : key(value),
          degree(0),
          marked(false),
          left(nullptr),
          right(nullptr),
          child(nullptr),
          parent(nullptr) {
        key = value;
        degree = 0;
        marked = false;
        left = this;
        right = this;
        parent = nullptr;
        child = nullptr;
    }
};

template <typename __Tp, typename __Cmp, typename __Abs>
fibonacci_heap<__Tp, __Cmp, __Abs>::fibonacci_heap() {
    keyNum = 0;
    maxDegree = 0;
    min = nullptr;
    cons = nullptr;
}

template <typename __Tp, typename __Cmp, typename __Abs>
struct fibonacci_heap<__Tp, __Cmp, __Abs>::iterator {
   private:
    Node *_real_node;
    friend class fibonacci_heap;

   public:
    const __Tp operator*() const {
        if (_real_node == nullptr) return __Tp();
        return _real_node->key;
    }

    iterator(Node *ptr = nullptr) : _real_node(ptr) {}
    iterator(const iterator &iter) : _real_node(iter._real_node) {}
};

template <typename __Tp, typename __Cmp, typename __Abs>
void fibonacci_heap<__Tp, __Cmp, __Abs>::remove_node(Node *node) {
    node->left->right = node->right;
    node->right->left = node->left;
}

template <typename __Tp, typename __Cmp, typename __Abs>
void fibonacci_heap<__Tp, __Cmp, __Abs>::insert_node(Node *node, Node *root) {
    node->left = root->left;
    root->left->right = node;
    node->right = root;
    root->left = node;
}

template <typename __Tp, typename __Cmp, typename __Abs>
void fibonacci_heap<__Tp, __Cmp, __Abs>::insert(Node *node) {
    if (keyNum == 0)
        min = node;
    else {
        insert_node(node, min);
        __Cmp _cmp;
        if (_cmp(node->key, min->key)) min = node;
    }
    keyNum++;
}

template <typename __Tp, typename __Cmp, typename __Abs>
typename fibonacci_heap<__Tp, __Cmp, __Abs>::iterator
fibonacci_heap<__Tp, __Cmp, __Abs>::insert(__Tp key) {
    Node *node;

    node = new Node(key);

    if (node == nullptr) return iterator(nullptr);

    insert(node);

    return iterator(node);
}

template <typename __Tp, typename __Cmp, typename __Abs>
void fibonacci_heap<__Tp, __Cmp, __Abs>::cat_list(Node *a, Node *b) {
    Node *tmp;

    tmp = a->right;
    a->right = b->right;
    b->right->left = a;
    b->right = tmp;
    tmp->left = b;
}

template <typename __Tp, typename __Cmp, typename __Abs>
void fibonacci_heap<__Tp, __Cmp, __Abs>::combine(
    fibonacci_heap<__Tp, __Cmp, __Abs> *other) {
    if (other == nullptr) return;

    if (other->maxDegree > this->maxDegree) std::swap(*this, *other);

    if ((this->min) == nullptr) {
        this->min = other->min;
        this->keyNum = other->keyNum;
        free(other->cons);
        return;
    } else if ((other->min) == nullptr) {
        free(other->cons);
        return;
    } else {
        cat_list(this->min, other->min);

        __Cmp _cmp;

        if (_cmp(other->min->key, this->min->key)) this->min = other->min;
        this->keyNum += other->keyNum;
        free(other->cons);
        return;
    }
}

template <typename __Tp, typename __Cmp, typename __Abs>
typename fibonacci_heap<__Tp, __Cmp, __Abs>::Node *
fibonacci_heap<__Tp, __Cmp, __Abs>::__pop() {
    Node *p = min;

    if (p == p->right)
        min = nullptr;
    else {
        remove_node(p);
        min = p->right;
    }
    p->left = p->right = p;

    return p;
}

template <typename __Tp, typename __Cmp, typename __Abs>
void fibonacci_heap<__Tp, __Cmp, __Abs>::link(Node *node, Node *root) {
    remove_node(node);
    if (root->child == nullptr)
        root->child = node;
    else
        insert_node(node, root->child);

    node->parent = root;
    root->degree++;
    node->marked = false;
}

template <typename __Tp, typename __Cmp, typename __Abs>
void fibonacci_heap<__Tp, __Cmp, __Abs>::make_cons() {
    int old = maxDegree;

    maxDegree = (log(keyNum * 1.0) / log(2.0)) + 1;
    if (old >= maxDegree) return;

    cons = (Node **)realloc(
        cons, sizeof(fibonacci_heap<__Tp, __Cmp, __Abs> *) * (maxDegree + 1));
}

template <typename __Tp, typename __Cmp, typename __Abs>
void fibonacci_heap<__Tp, __Cmp, __Abs>::consolidate() {
    int i, d, D;
    Node *x, *y, *tmp;

    make_cons();
    D = maxDegree + 1;

    for (i = 0; i < D; i++) cons[i] = nullptr;

    __Cmp _cmp;

    while (min != nullptr) {
        x = __pop();
        d = x->degree;
        while (cons[d] != nullptr) {
            y = cons[d];
            if (_cmp(y->key, x->key)) std::swap(x, y);

            link(y, x);
            cons[d] = nullptr;
            d++;
        }
        cons[d] = x;
    }
    min = nullptr;

    for (i = 0; i < D; i++) {
        if (cons[i] != nullptr) {
            if (min == nullptr)
                min = cons[i];
            else {
                insert_node(cons[i], min);
                if (_cmp((cons[i])->key, min->key)) min = cons[i];
            }
        }
    }
}

template <typename __Tp, typename __Cmp, typename __Abs>
void fibonacci_heap<__Tp, __Cmp, __Abs>::pop() {
    if (min == nullptr) return;

    Node *child = nullptr;
    Node *m = min;
    while (m->child != nullptr) {
        child = m->child;
        remove_node(child);
        if (child->right == child)
            m->child = nullptr;
        else
            m->child = child->right;

        insert_node(child, min);
        child->parent = nullptr;
    }

    remove_node(m);
    if (m->right == m)
        min = nullptr;
    else {
        min = m->right;
        consolidate();
    }
    keyNum--;

    delete m;
}

template <typename __Tp, typename __Cmp, typename __Abs>
typename fibonacci_heap<__Tp, __Cmp, __Abs>::iterator
fibonacci_heap<__Tp, __Cmp, __Abs>::top() {
    if (min == nullptr) return iterator(nullptr);
    return iterator(min);
}

template <typename __Tp, typename __Cmp, typename __Abs>
void fibonacci_heap<__Tp, __Cmp, __Abs>::renew_deg(Node *parent, int degree) {
    parent->degree -= degree;
    if (parent->parent != nullptr) renew_deg(parent->parent, degree);
}

template <typename __Tp, typename __Cmp, typename __Abs>
void fibonacci_heap<__Tp, __Cmp, __Abs>::cut(Node *node, Node *parent) {
    remove_node(node);
    renew_deg(parent, node->degree);
    if (node == node->right)
        parent->child = nullptr;
    else
        parent->child = node->right;

    node->parent = nullptr;
    node->left = node->right = node;
    node->marked = false;
    insert_node(node, min);
}

template <typename __Tp, typename __Cmp, typename __Abs>
void fibonacci_heap<__Tp, __Cmp, __Abs>::cas_cut(Node *node) {
    Node *parent = node->parent;
    if (parent != nullptr) {
        if (node->marked == false)
            node->marked = true;
        else {
            cut(node, parent);
            cas_cut(parent);
        }
    }
}

template <typename __Tp, typename __Cmp, typename __Abs>
bool fibonacci_heap<__Tp, __Cmp, __Abs>::decrease(Node *node, __Tp key) {
    Node *parent = nullptr;

    __Abs _absc;

    if (min == nullptr || node == nullptr) return false;

    if (_absc(node->key, key) || key == node->key) return false;

    node->key = key;
    parent = node->parent;

    __Cmp _cmp;

    if (parent != nullptr && _cmp(node->key, parent->key)) {
        cut(node, parent);
        cas_cut(parent);
    }

    if (_cmp(node->key, min->key)) min = node;

    return true;
}

template <typename __Tp, typename __Cmp, typename __Abs>
bool fibonacci_heap<__Tp, __Cmp, __Abs>::increase(Node *node, __Tp key) {
    Node *child, *parent, *right;
    __Abs _absc;

    if (min == nullptr || node == nullptr) return false;

    if (_absc(key, node->key) || key == node->key) return false;

    while (node->child != nullptr) {
        child = node->child;
        remove_node(child);
        if (child->right == child)
            node->child = nullptr;
        else
            node->child = child->right;

        insert_node(child, min);
        child->parent = nullptr;
    }
    node->degree = 0;
    node->key = key;

    parent = node->parent;

    __Cmp _cmp;

    if (parent != nullptr) {
        cut(node, parent);
        cas_cut(parent);
    } else if (min == node) {
        right = node->right;
        while (right != node) {
            if (_cmp(right->key, node->key)) min = right;
            right = right->right;
        }
    }

    return true;
}

template <typename __Tp, typename __Cmp, typename __Abs>
bool fibonacci_heap<__Tp, __Cmp, __Abs>::update(Node *node, __Tp key) {
    if (node == nullptr) return false;

    __Abs _absc;

    if (_absc(key, node->key))
        return decrease(node, key);
    else if (_absc(node->key, key))
        return increase(node, key);
    else
        return false;
}

template <typename __Tp, typename __Cmp, typename __Abs>
bool fibonacci_heap<__Tp, __Cmp, __Abs>::update(iterator iter, __Tp newkey) {
    Node *node = iter._real_node;
    return update(node, newkey);
}

template <typename __Tp, typename __Cmp, typename __Abs>
typename fibonacci_heap<__Tp, __Cmp, __Abs>::Node *
fibonacci_heap<__Tp, __Cmp, __Abs>::search(Node *root, __Tp key) {
    Node *t = root;
    Node *p = nullptr;

    if (root == nullptr) return root;

    do {
        if (t->key == key) {
            p = t;
            break;
        } else {
            if ((p = search(t->child, key)) != nullptr) break;
        }
        t = t->right;
    } while (t != root);

    return p;
}

template <typename __Tp, typename __Cmp, typename __Abs>
typename fibonacci_heap<__Tp, __Cmp, __Abs>::Node *
fibonacci_heap<__Tp, __Cmp, __Abs>::search(__Tp key) {
    if (min == nullptr) return nullptr;

    return search(min, key);
}

template <typename __Tp, typename __Cmp, typename __Abs>
bool fibonacci_heap<__Tp, __Cmp, __Abs>::contains(__Tp key) {
    return search(key) != nullptr ? true : false;
}

template <typename __Tp, typename __Cmp, typename __Abs>
void fibonacci_heap<__Tp, __Cmp, __Abs>::remove(Node *node) {
    __Tp m = min->key - (__Tp)1;
    decrease(node, m - (__Tp)1);
    pop();
}

template <typename __Tp, typename __Cmp, typename __Abs>
void fibonacci_heap<__Tp, __Cmp, __Abs>::remove(__Tp key) {
    Node *node;

    if (min == nullptr) return;

    node = search(key);
    if (node == nullptr) return;

    remove(node);
}

template <typename __Tp, typename __Cmp, typename __Abs>
void fibonacci_heap<__Tp, __Cmp, __Abs>::clear() {
    if (min != nullptr) delete min;

    if (cons != nullptr) free(cons);
}

template <typename __Tp, typename __Cmp, typename __Abs>
size_t fibonacci_heap<__Tp, __Cmp, __Abs>::size() {
    return keyNum;
}

template <typename __Tp, typename __Cmp, typename __Abs>
bool fibonacci_heap<__Tp, __Cmp, __Abs>::empty() {
    return !keyNum;
}
}  // namespace Octinc

#endif
