/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#include <cstdint>
#include <iostream>

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <algorithm>
#include <cstdint>

namespace Octinc {
template <typename __Tp, typename __Cmp = std::less<__Tp> >
class leftist_tree {
   private:
    size_t siz;

    struct Node {
        __Tp val;
        Node *lc, *rc;
        size_t npl;
        Node(__Tp v) : val(v), lc(nullptr), rc(nullptr), npl(1) {}
    };

    Node* merge(Node* l, Node* r) {
        if (l == nullptr) return r;
        if (r == nullptr) return l;

        __Cmp _cmp;

        if (_cmp(r->val, l->val)) std::swap(l, r);

        Node* ptr = new Node(*l);
        ptr->rc = merge(ptr->rc, r);

        if (ptr->lc == nullptr || _cmp(ptr->lc->npl, ptr->rc->npl))
            std::swap(ptr->lc, ptr->rc);

        ptr->npl = ptr->rc != nullptr ? ptr->rc->npl + 1 : 1;

        return ptr;
    }

    Node* push(__Tp val, Node* rt) { return merge(new Node(val), rt); }

    __Tp top(Node* rt) { return rt->val; }

    Node* pop(Node* rt) { return merge(rt->lc, rt->rc); }

    Node* _root;

   public:
    leftist_tree() : siz(0) { _root = new Node(__Tp()); }
    leftist_tree(const leftist_tree& other) { *this = other; }

    struct iterator {
       private:
        Node* _real_node;
        std::pair<__Tp, size_t> pr;
        friend leftist_tree;

       public:
        iterator(Node* ptr = nullptr) : _real_node(ptr) {}
        iterator(const iterator& iter) : _real_node(iter._real_node) {}

        const std::pair<__Tp, size_t> operator*() {
            return std::make_pair(_real_node->val, _real_node->npl);
        }

        const std::pair<__Tp, size_t>* operator->() {
            pr.first = _real_node->val;
            pr.second = _real_node->npl;
            return &pr;
        }
    };

    /*
    The following operations are all persistent.
    */

    iterator merge(iterator siter, iterator eiter)
    /*
    Merge two heaps with the iterator pointer as the top of the heap and return
    to the top of the new heap.
    */
    {
        return iterator(merge(siter._real_node, eiter._real_node));
    }

    iterator push(__Tp val)
    /*
    Push a node with value into the whole heap.
    */
    {
        if (siz == 0) {
            _root = new Node(val);
            return iterator(_root);
        } else
            return iterator(push(val, _root));
    }

    iterator push(__Tp val, iterator iter)
    /*
    Push a node with value into the iterator pointer as the top of the heap.
    */
    {
        ++siz;
        return push(val, iter._real_node);
    }

    iterator top()
    /*
    Return the iterator of the top of the heap.
    */
    {
        return iterator(_root);
    }

    iterator pop()
    /*
    Remove the node from the whole heap.
    */
    {
        --siz;
        return iterator(pop(_root));
    }

    iterator pop(iterator iter)
    /*
    Remove the node from the iterator pointer as the top of the heap.
    */
    {
        --siz;
        return iterator(pop(iter._real_node));
    }
};
}  // namespace Octinc
