/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_RMQ__
#define OCTINC_RMQ__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <cstring>
#include <iostream>
#include <vector>

namespace Octinc {
template <typename __Tp>
class minimum_Q {
   public:
    __Tp operator()(const __Tp& a, const __Tp& b) const {
        return a < b ? a : b;
    }
};

template <typename __Tp>
class maximum_Q {
   public:
    __Tp operator()(const __Tp& a, const __Tp& b) const {
        return a > b ? a : b;
    }
};

template <typename __Tp, typename __Query = minimum_Q<__Tp>,
          const int MaxLog = 33>
class st_table {
   private:
    __Tp* st_cmp[MaxLog];
    int* pre_log;

   public:
    st_table() {
        for (int i = 0; i < MaxLog; ++i) st_cmp[i] = nullptr;

        pre_log = nullptr;
    }

    st_table(__Tp* a, int n) { build(a, n); }

    void clear() {
        for (int i = 0; i < MaxLog; ++i)
            if (st_cmp[i] != nullptr) delete st_cmp[i];

        if (pre_log != nullptr) delete pre_log;
    }

    void build(__Tp* a, size_t n)
    /*
    Build the table with the array a which the length of is n.
    */
    {
        clear();

        for (int i = 0; i < MaxLog; ++i)
            st_cmp[i] = new __Tp[n + 5],
            memset(st_cmp[i], 0, sizeof(__Tp) * (n + 5));

        pre_log = new int[n + 5];
        memset(pre_log, 0, sizeof(__Tp) * (n + 5));

        pre_log[0] = -1;

        for (int i = 1; i <= n; ++i)
            st_cmp[0][i] = a[i - 1], pre_log[i] = pre_log[i >> 1] + 1;

        __Query _qry;

        for (int i = 1; i <= pre_log[n] + 1; ++i)
            for (int j = 1; j + (1 << i) - 1 <= n; ++j)
                st_cmp[i][j] =
                    _qry(st_cmp[i - 1][j], st_cmp[i - 1][j + (1 << i - 1)]);
    }

    __Tp query(size_t l, size_t r)
    /*
    RMQ.
    */
    {
        int k = pre_log[r - l + 1];
        __Query _qry;
        return _qry(st_cmp[k][l], st_cmp[k][r - (1 << k) + 1]);
    }
};

template <typename __Tp, typename __Query = minimum_Q<__Tp> >
class segment_tree {
   private:
    struct Node {
        Node *lc, *rc;
        __Tp val;

        Node(__Tp val = __Tp(), Node* lc = nullptr, Node* rc = nullptr)
            : val(val), lc(lc), rc(rc) {}

        void pushup() {
            __Query _qry;
            val = _qry(lc->val, rc->val);
        }
    };

    Node* _root;
    size_t left, right;

    void __build(Node*& x, __Tp* a, size_t l, size_t r) {
        if (x == nullptr) x = new Node();

        if (l == r) {
            x->val = a[l];
            x->lc = new Node();
            x->rc = new Node();
            return;
        }

        size_t mid = (l + r) >> 1;
        __build(x->lc, a, l, mid);
        __build(x->rc, a, mid + 1, r);
        x->pushup();
    }

    void __update(Node* x, size_t pos, __Tp v, size_t l, size_t r) {
        if (l == r) {
            x->val = v;
            return;
        }

        size_t mid = (l + r) >> 1;

        if (pos <= mid)
            __update(x->lc, pos, v, l, mid);
        else
            __update(x->rc, pos, v, mid + 1, r);

        x->pushup();
    }

    __Tp __query(Node* x, size_t L, size_t R, size_t l, size_t r) {
        if (L == l && R == r) return x->val;

        size_t mid = (l + r) >> 1;

        if (R <= mid && x->lc != nullptr)
            return __query(x->lc, L, R, l, mid);
        else if (L > mid && x->rc != nullptr)
            return __query(x->rc, L, R, mid + 1, r);

        __Query _qry;
        __Tp lq = __query(x->lc, L, mid, l, mid),
             rq = __query(x->rc, mid + 1, R, mid + 1, r);

        return _qry(lq, rq);
    }

    void __clear(Node*& x) {
        if (x == nullptr) return;
        if (x->lc != nullptr) __clear(x->lc);
        if (x->rc != nullptr) __clear(x->rc);
        delete x;
    }

   public:
    segment_tree() {
        _root = nullptr;
        left = right = 0;
    }

    segment_tree(__Tp* a, size_t l, size_t r) {
        _root = nullptr;
        build(a, l, r);
    }

    void build(__Tp* a, size_t l, size_t r)
    /*
    Build the tree with the array a in interval [l,r].
    */
    {
        clear();
        __build(_root, a, l, r);
        left = l, right = r;
    }

    void update(size_t pos, __Tp v)
    /*
    Modify the value of the position to v.
    */
    {
        __update(_root, pos, v, left, right);
    }

    __Tp query(size_t l, size_t r)
    /*
    RMQ.
    */
    {
        return __query(_root, l, r, left, right);
    }

    void clear() {
        if (_root != nullptr) __clear(_root);

        left = right = 0;
    }
};

template <size_t n, const size_t MaxLog = 23>
class st_LCA
/*
n is the number of the node of the tree.
MaxLog is a little more than log(n).
The nodes of the tree are labeled by 1,2,...,n.
*/
{
   private:
    struct Node {
        std::vector<Node*> adj;
        size_t label;
        int dep;

        Node(size_t l = 0) : label(l) { adj.clear(); }
        ~Node() {
            for (auto& x : adj)
                if (x != nullptr) delete x;
        }
    };

    void __build(Node* x, Node* fa = nullptr) {
        if (x == nullptr) return;
        if (fa == nullptr)
            x->dep = 1;
        else
            x->dep = fa->dep + 1;

        f[x->label][0] = (fa == nullptr ? 0 : fa->label);

        for (int i = 1; (1 << i) <= x->dep; ++i)
            f[x->label][i] = f[f[x->label][i - 1]][i - 1];

        for (auto& y : x->adj)
            if (y != fa) __build(y, x);
    }

    Node* __LCA(Node* x, Node* y) {
        if (x->dep < y->dep) std::swap(x, y);

        while (x->dep > y->dep) x = e[f[x->label][lg[x->dep - y->dep] - 1] - 1];

        if (x == y) return x;

        for (int k = lg[x->dep]; k >= 0; --k)
            if (f[x->label][k] != f[y->label][k])
                x = e[f[x->label][k] - 1], y = e[f[y->label][k] - 1];

        return e[f[x->label][0] - 1];
    }

    Node* _root;
    int* lg;
    size_t* f[MaxLog];
    std::vector<Node*> e;

   public:
    struct iterator {
       private:
        Node* _real_node;
        friend class st_LCA;

       public:
        const size_t operator*() {
            if (_real_node == nullptr) return 0;
            return _real_node->label;
        }

        iterator(Node* ptr = nullptr) : _real_node(ptr) {}
        iterator(const iterator& iter) : _real_node(iter._real_node) {}
    };

    st_LCA() {
        _root = nullptr;
        lg = new int[n + 5];
        memset(lg, 0, sizeof(int) * (n + 5));

        for (int i = 0; i < MaxLog; ++i)
            f[i] = new size_t[n + 5], memset(f[i], 0, sizeof(size_t) * (n + 5));

        for (int i = 0; i < n; ++i) e.push_back(new Node(i + 1));
    }

    iterator set_root(size_t l)
    /*
    Set the root of the tree by the label.
    */
    {
        for (int i = 0; i < n; ++i)
            if (e[i]->label == l) _root = e[i];

        return iterator(_root);
    }

    iterator get_node(size_t s)
    /*
    Get the node whose label is s.
    */
    {
        return iterator(e[s - 1]);
    }

    iterator operator[](size_t s) { return get_node(s); }

    iterator root()
    /*
    Get the root.
    */
    {
        return iterator(_root);
    }

    void add_edge(iterator xiter, iterator yiter)
    /*
    Insert an edge into node x and y.
    */
    {
        Node *x = xiter._real_node, *y = yiter._real_node;
        x->adj.push_back(y);
        y->adj.push_back(x);
    }

    void build()
    /*
    Statistical the information of the tree.
    */
    {
        for (int i = 1; i <= n; ++i) lg[i] = lg[i - 1] + (1 << lg[i - 1] == i);
        __build(_root);
    }

    void clear() {
        _root = nullptr;
        memset(lg, 0, sizeof(int) * (n + 5));

        for (int i = 0; i < MaxLog; ++i)
            memset(f[i], 0, sizeof(size_t) * (n + 5));

        for (int i = 0; i < n; ++i) e[i]->adj.clear();
    }

    iterator LCA(iterator x, iterator y)
    /*
    Query the LCA(Least Common Ancestor) of two nodes.
    */
    {
        return __LCA(x._real_node, y._real_node);
    }
};
}  // namespace Octinc

#endif
