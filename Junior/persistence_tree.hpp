/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_PERSISTENCE_TREE__
#define OCTINC_PERSISTENCE_TREE__

#include <cstdint>

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

namespace Octinc {
template <typename __Tp>
class persistence_tree {
   private:
    struct Node {
        Node *lc, *rc;
        __Tp val;
        Node(__Tp v = __Tp()) : val(v), lc(nullptr), rc(nullptr) {}
    };

    Node* _root;
    size_t left, right;

    void __build(Node*& x, __Tp* a, size_t l, size_t r) {
        if (x == nullptr) x = new Node();

        if (l == r) {
            x->val = a[l];
            return;
        }

        size_t mid = (l + r) >> 1;
        __build(x->lc, a, l, mid);
        __build(x->rc, a, mid + 1, r);
    }

    void __insert(Node*& x, Node* pre, size_t l, size_t r, size_t q, __Tp v) {
        if (x == nullptr) x = new Node();
        x->lc = pre->lc;
        x->rc = pre->rc;
        x->val = pre->val;

        if (l == r) {
            x->val = v;
            return;
        }

        size_t mid = (l + r) >> 1;

        if (q <= mid)
            __insert(x->lc, pre->lc, l, mid, q, v);
        else
            __insert(x->rc, pre->rc, mid + 1, r, q, v);
    }

    Node* __query(Node* x, size_t l, size_t r, size_t q) {
        if (l == r) return x;

        size_t mid = (l + r) >> 1;

        if (q <= mid)
            return __query(x->lc, l, mid, q);
        else
            return __query(x->rc, mid + 1, r, q);
    }

    void __clear(Node*& x) {
        if (x == nullptr) return;
        if (x->lc != nullptr) __clear(x->lc);
        if (x->rc != nullptr) __clear(x->rc);
        delete x;
    }

   public:
    persistence_tree() {
        _root = nullptr;
        left = right = 0;
    }

    persistence_tree(__Tp* a, size_t l, size_t r) {
        _root = nullptr;
        build(a, l, r);
    }

    struct iterator {
       private:
        Node* _real_node;
        friend class persistence_tree;

       public:
        const __Tp operator*() {
            if (_real_node == nullptr ||
                (_real_node->lc != nullptr && _real_node->rc != nullptr))
                return __Tp();
            return _real_node->val;
        }

        iterator(Node* ptr = nullptr) : _real_node(ptr) {}
        iterator(const iterator& iter) : _real_node(iter._real_node) {}
    };

    iterator build(__Tp* a, size_t l, size_t r)
    /*
    Build the tree with the array a in interval [l,r].
    */
    {
        clear();
        __build(_root, a, l, r);
        left = l, right = r;
        return iterator(_root);
    }

    void insert(iterator iter, size_t q, __Tp v)
    /*
    Insert a node with value v to the tree with iterators pointing to nodes as
    roots.
    */
    {
        __insert(new Node(), iter._real_node, left, right, q, v);
    }

    iterator query(iterator iter, size_t q)
    /*
    Query the qth number in the tree.
    */
    {
        return iterator(__query(iter._real_node, left, right, q));
    }

    iterator root()
    /*
    Get the iterator of the root.
    */
    {
        return iterartor(_root);
    }

    void clear() {
        if (_root != nullptr) __clear(_root);

        left = right = 0;
    }
};
}  // namespace Octinc

#endif
