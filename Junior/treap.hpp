/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_TREAP__
#define OCTINC_TREAP__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>

namespace Octinc {
#define mod(x) (Mod == 0 ? x : x % Mod)

template <typename __Tp, const __Tp Mod = 0>
/*
Mod is the modulus base after each operation.
Mod=0 means needn't do modulus.
*/
class treap {
   private:
    struct Node;

    Node* _root;
    Node* merge(Node*, Node*);

    void ranksplit(Node*, const size_t&, Node*&, Node*&);
    void dfs(Node*, std::ostream&);

   public:
    treap() {}
    treap(const treap& other) { *this = other; }

    struct iterator;

    void add_update(size_t, size_t, const __Tp);
    /*
    Interval addition.
    */

    void mul_update(size_t, size_t, const __Tp);
    /*
    Interval mutiplication.
    */

    __Tp query(size_t, size_t);
    /*
    Interval summation.
    */

    __Tp operator[](size_t i) { return query(i, i); }

    iterator insert(const __Tp);
    /*
    Insert values.
    */

    void print(std::ostream&);
    /*
    Print the entire sequence.
    */
};

template <typename __Tp, const __Tp Mod>
struct treap<__Tp, Mod>::Node {
    __Tp val;
    size_t pri, s;
    Node *lc, *rc;
    __Tp addflag, mulflag, sum;
    std::mt19937 rnd;

    Node(const __Tp& v = __Tp(), Node* l = nullptr, Node* r = nullptr,
         size_t ss = 1, const __Tp& aff = __Tp(0), const __Tp& mff = __Tp(1))
        : val(v),
          sum(v),
          pri(rnd()),
          lc(l),
          rc(r),
          s(ss),
          addflag(aff),
          mulflag(mff) {}

    static size_t size(Node* ptr) { return ptr != nullptr ? ptr->s : 0; }

    void maintain() {
        s = size(lc) + size(rc) + 1;
        sum = val;

        if (lc != nullptr)
            sum = mod(sum + lc->sum * lc->mulflag + lc->s * lc->addflag);
        if (rc != nullptr)
            sum = mod(sum + rc->sum * rc->mulflag + rc->s * rc->addflag);
    }

    void pushdown() {
        sum = mod(sum * mulflag + s * addflag);
        val = mod(val * mulflag + addflag);

        if (lc != nullptr) {
            lc->addflag = mod(lc->addflag * mulflag + addflag);
            lc->mulflag = mod(lc->mulflag * mulflag);
        }
        if (rc != nullptr) {
            rc->addflag = mod(rc->addflag * mulflag + addflag);
            rc->mulflag = mod(rc->mulflag * mulflag);
        }

        addflag = __Tp(0);
        mulflag = __Tp(1);
    }
};

template <typename __Tp, const __Tp Mod>
struct treap<__Tp, Mod>::iterator {
   private:
    Node* _real_node;

   public:
    __Tp operator*() const {
        if (_real_node == nullptr) return __Tp();
        return _real_node->val;
    }

    iterator(Node* ptr = nullptr) : _real_node(ptr) {}
    iterator(const iterator& iter) : _real_node(iter._real_node) {}
};

template <typename __Tp, const __Tp Mod>
void treap<__Tp, Mod>::dfs(Node* ptr, std::ostream& os) {
    if (!ptr) return;

    ptr->pushdown();
    dfs(ptr->lc, os);
    os << ptr->val << ' ';
    dfs(ptr->rc, os);
}

template <typename __Tp, const __Tp Mod>
void treap<__Tp, Mod>::print(std::ostream& os) {
    dfs(_root, os);
    os << std::endl;
}

template <typename __Tp, const __Tp Mod>
typename treap<__Tp, Mod>::Node* treap<__Tp, Mod>::merge(Node* x, Node* y) {
    if (!x) return y;
    if (!y) return x;

    if (x->pri < y->pri) {
        x->pushdown();
        x->rc = merge(x->rc, y);
        x->maintain();
        return x;
    } else {
        y->pushdown();
        y->lc = merge(x, y->lc);
        y->maintain();
        return y;
    }
}

template <typename __Tp, const __Tp Mod>
void treap<__Tp, Mod>::ranksplit(Node* nroot, const size_t& k, Node*& ltree,
                                 Node*& rtree) {
    if (!nroot) {
        ltree = rtree = nullptr;
        return;
    }

    nroot->pushdown();
    size_t s = Node::size(nroot->lc);

    if (s >= k) {
        rtree = nroot;
        ranksplit(nroot->lc, k, ltree, nroot->lc);
    } else {
        ltree = nroot;
        ranksplit(nroot->rc, k - s - 1, nroot->rc, rtree);
    }

    nroot->maintain();
}

template <typename __Tp, const __Tp Mod>
typename treap<__Tp, Mod>::iterator treap<__Tp, Mod>::insert(const __Tp x) {
    Node* ptr = new Node(x);

    if (!_root) {
        _root = ptr;
        return ptr;
    }

    _root = merge(_root, ptr);
    return ptr;
}

template <typename __Tp, const __Tp Mod>
void treap<__Tp, Mod>::add_update(size_t l, size_t r, const __Tp v) {
    Node *ptr, *ptr1, *ptr2;
    ranksplit(_root, l - 1, ptr1, ptr);
    ranksplit(ptr, r - l + 1, ptr, ptr2);
    ptr->addflag = mod(ptr->addflag + v);
    _root = merge(ptr1, merge(ptr, ptr2));
}

template <typename __Tp, const __Tp Mod>
void treap<__Tp, Mod>::mul_update(size_t l, size_t r, const __Tp v) {
    Node *ptr, *ptr1, *ptr2;
    ranksplit(_root, l - 1, ptr1, ptr);
    ranksplit(ptr, r - l + 1, ptr, ptr2);
    ptr->addflag = mod(ptr->addflag * v);
    ptr->mulflag = mod(ptr->mulflag * v);
    _root = merge(ptr1, merge(ptr, ptr2));
}

template <typename __Tp, const __Tp Mod>
__Tp treap<__Tp, Mod>::query(size_t l, size_t r) {
    Node *ptr, *ptr1, *ptr2;
    ranksplit(_root, l - 1, ptr1, ptr);
    ranksplit(ptr, r - l + 1, ptr, ptr2);
    ptr->pushdown();
    __Tp ans = ptr->sum;
    _root = merge(ptr1, merge(ptr, ptr2));
    return ans;
}
}  // namespace Octinc

#endif
