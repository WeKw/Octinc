/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_SPLAY_TREE__
#define OCTINC_SPLAY_TREE__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <cstdint>
#include <iostream>

namespace Octinc {
template <typename __Tp, typename __Cmp = std::less<__Tp> >
class splay_tree {
   private:
    struct Node;
    size_t siz;
    Node* _root;

    Node* search(Node*, __Tp) const;
    Node* minimum(Node*);
    Node* maximum(Node*);
    Node* splay(Node*, __Tp);
    Node* insert(Node*&, Node*);
    Node* remove(Node*&, __Tp);
    void clear(Node*&);

   public:
    splay_tree();
    splay_tree(const splay_tree& other) { *this = other; }
    ~splay_tree();

    struct iterator;

    iterator search(__Tp);
    /*
    Search the node whose key is the key.
    */
    iterator minimum();
    /*
    Query the minimum key.
    */
    iterator maximum();
    /*
    Query the maximum key.
    */
    iterator lower_bound(__Tp);
    /*
    Query the smallest number larger than the key.
    */
    iterator upper_bound(__Tp);
    /*
    Query the largest number smaller than the key.
    */
    iterator insert(__Tp);
    /*
    Insert a key.
    */
    iterator remove(__Tp);
    /*
    Delete the key.
    */
    iterator splay(__Tp);
    iterator begin();
    iterator end();
    void clear();
    /*
    Clear the whole tree.
    */
    size_t size() { return siz; }

    bool empty() { return !_root; }
};

template <typename __Tp, typename __Cmp>
struct splay_tree<__Tp, __Cmp>::Node {
    __Tp key;
    Node *left, *right;

    Node() : left(nullptr), right(nullptr) {}
    Node(__Tp value, Node* l, Node* r) : key(value), left(l), right(r) {}
};

template <typename __Tp, typename __Cmp>
splay_tree<__Tp, __Cmp>::splay_tree() : _root(nullptr) {}

template <typename __Tp, typename __Cmp>
splay_tree<__Tp, __Cmp>::~splay_tree() {
    clear(_root);
}

template <typename __Tp, typename __Cmp>
struct splay_tree<__Tp, __Cmp>::iterator
    : public std::iterator<std::input_iterator_tag, __Tp> {
   private:
    Node* _real_node;
    friend class splay_tree;

   public:
    iterator(Node* ptr = nullptr) : _real_node(ptr) {}
    iterator(const iterator& iter) : _real_node(iter._real_node) {}

    iterator operator++() {
        if (_real_node == nullptr) return *this;

        _real_node = _real_node->right;
        return *this;
    }
    iterator operator++(int) {
        if (_real_node == nullptr) return *this;

        iterator iter = *this;
        _real_node = _real_node->right;
        return *this;
    }
    iterator operator--() {
        if (_real_node == nullptr) return *this;

        _real_node = _real_node->left;
        return *this;
    }
    iterator operator--(int) {
        if (_real_node == nullptr) return *this;

        iterator iter = *this;
        _real_node = _real_node->left;
        return *this;
    }

    const __Tp operator*() {
        if (_real_node == nullptr) return __Tp();
        return _real_node->key;
    }

    bool operator==(iterator other) { return _real_node == other._real_node; }
    bool operator!=(iterator other) { return _real_node != other._real_node; }
};

template <typename __Tp, typename __Cmp>
typename splay_tree<__Tp, __Cmp>::Node* splay_tree<__Tp, __Cmp>::search(
    Node* x, __Tp key) const {
    __Cmp _cmp;

    while ((x != nullptr) && (x->key != key)) {
        if (_cmp(key, x->key))
            x = x->left;
        else
            x = x->right;
    }

    return x;
}

template <typename __Tp, typename __Cmp>
typename splay_tree<__Tp, __Cmp>::iterator splay_tree<__Tp, __Cmp>::search(
    __Tp key) {
    return iterator(search(_root, key));
}

template <typename __Tp, typename __Cmp>
typename splay_tree<__Tp, __Cmp>::Node* splay_tree<__Tp, __Cmp>::minimum(
    Node* tree) {
    if (tree == nullptr) return nullptr;

    while (tree->left != nullptr) tree = tree->left;

    return tree;
}

template <typename __Tp, typename __Cmp>
typename splay_tree<__Tp, __Cmp>::iterator splay_tree<__Tp, __Cmp>::minimum() {
    Node* p = minimum(_root);

    if (p != nullptr) return iterator(p);

    return iterator(nullptr);
}

template <typename __Tp, typename __Cmp>
typename splay_tree<__Tp, __Cmp>::iterator splay_tree<__Tp, __Cmp>::upper_bound(
    __Tp key) {
    Node* p = minimum(splay(_root, key)->right);

    if (p != nullptr) return iterator(p);

    return iterator(nullptr);
}

template <typename __Tp, typename __Cmp>
typename splay_tree<__Tp, __Cmp>::Node* splay_tree<__Tp, __Cmp>::maximum(
    Node* tree) {
    if (tree == nullptr) return nullptr;

    while (tree->right != nullptr) tree = tree->right;

    return tree;
}

template <typename __Tp, typename __Cmp>
typename splay_tree<__Tp, __Cmp>::iterator splay_tree<__Tp, __Cmp>::maximum() {
    Node* p = maximum(_root);

    if (p != nullptr) return iterator(p);

    return iterator(nullptr);
}

template <typename __Tp, typename __Cmp>
typename splay_tree<__Tp, __Cmp>::iterator splay_tree<__Tp, __Cmp>::lower_bound(
    __Tp key) {
    Node* p = maximum(splay(_root, key)->left);

    if (p != nullptr) return iterator(p);

    return iterator(nullptr);
}

template <typename __Tp, typename __Cmp>
typename splay_tree<__Tp, __Cmp>::iterator splay_tree<__Tp, __Cmp>::begin() {
    return iterator(_root);
}

template <typename __Tp, typename __Cmp>
typename splay_tree<__Tp, __Cmp>::iterator splay_tree<__Tp, __Cmp>::end() {
    return iterator(nullptr);
}

template <typename __Tp, typename __Cmp>
typename splay_tree<__Tp, __Cmp>::Node* splay_tree<__Tp, __Cmp>::splay(
    Node* tree, __Tp key) {
    Node N, *l, *r, *c;

    if (tree == nullptr) return tree;

    N.left = N.right = nullptr;
    l = r = &N;
    __Cmp _cmp;

    while (true) {
        if (_cmp(key, tree->key)) {
            if (tree->left == nullptr) break;

            if (_cmp(key, tree->left->key)) {
                c = tree->left;
                tree->left = c->right;
                c->right = tree;
                tree = c;

                if (tree->left == nullptr) break;
            }

            r->left = tree;
            r = tree;
            tree = tree->left;
        } else if (_cmp(tree->key, key)) {
            if (tree->right == nullptr) break;

            if (_cmp(tree->right->key, key)) {
                c = tree->right;
                tree->right = c->left;
                c->left = tree;
                tree = c;

                if (tree->right == nullptr) break;
            }

            l->right = tree;
            l = tree;
            tree = tree->right;
        } else
            break;
    }

    l->right = tree->left;
    r->left = tree->right;
    tree->left = N.right;
    tree->right = N.left;

    return tree;
}

template <typename __Tp, typename __Cmp>
typename splay_tree<__Tp, __Cmp>::iterator splay_tree<__Tp, __Cmp>::splay(
    __Tp key) {
    _root = splay(_root, key);
    return iterator(_root);
}

template <typename __Tp, typename __Cmp>
typename splay_tree<__Tp, __Cmp>::Node* splay_tree<__Tp, __Cmp>::insert(
    Node*& tree, Node* z) {
    Node *y = nullptr, *x = tree;
    __Cmp _cmp;

    while (x != nullptr) {
        y = x;

        if (_cmp(z->key, x->key))
            x = x->left;
        else if (_cmp(x->key, z->key))
            x = x->right;
        else {
            delete z;
            return tree;
        }
    }

    if (y == nullptr)
        tree = z;
    else if (_cmp(z->key, y->key))
        y->left = z;
    else
        y->right = z;

    return tree;
}

template <typename __Tp, typename __Cmp>
typename splay_tree<__Tp, __Cmp>::iterator splay_tree<__Tp, __Cmp>::insert(
    __Tp key) {
    Node* z = nullptr;
    ++siz;

    if ((z = new Node(key, nullptr, nullptr)) == nullptr)
        return iterator(nullptr);

    _root = insert(_root, z);
    _root = splay(_root, key);
    return iterator(_root);
}

template <typename __Tp, typename __Cmp>
typename splay_tree<__Tp, __Cmp>::Node* splay_tree<__Tp, __Cmp>::remove(
    Node*& tree, __Tp key) {
    Node* x;

    if (tree == nullptr) return nullptr;

    if (search(tree, key) == nullptr) return tree;

    tree = splay(tree, key);

    if (tree->left != nullptr) {
        x = splay(tree->left, key);
        x->right = tree->right;
    } else
        x = tree->right;

    delete tree;

    return x;
}

template <typename __Tp, typename __Cmp>
typename splay_tree<__Tp, __Cmp>::iterator splay_tree<__Tp, __Cmp>::remove(
    __Tp key) {
    --siz;
    _root = remove(_root, key);
    return iterator(_root);
}

template <typename __Tp, typename __Cmp>
void splay_tree<__Tp, __Cmp>::clear(Node*& tree) {
    if (tree == nullptr) return;

    if (tree->left != nullptr) clear(tree->left);
    if (tree->right != nullptr) clear(tree->right);

    delete tree;
}

template <typename __Tp, typename __Cmp>
void splay_tree<__Tp, __Cmp>::clear() {
    clear(_root);
}

}  // namespace Octinc

#endif
