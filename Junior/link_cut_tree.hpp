/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_LINK_CUT_TREE__
#define OCTINC_LINK_CUT_TREE__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <algorithm>
#include <map>

namespace Octinc {
namespace detail {
template <typename Value_type, typename Functor>
class LCT_splay : public Functor {
   public:
    struct Node;
    Node* __new_node(const Value_type&);
};

template <typename Value_type, typename Functor>
struct LCT_splay<Value_type, Functor>::Node {
    Value_type val, sum;
    Node* ftr;
    Node* ch[2];
    Node*& lc;
    Node*& rc;
    bool rev;

    Node(const Value_type& v)
        : val(v), sum(v), ftr(nullptr), lc(ch[0]), rc(ch[1]), rev(false) {
        ch[0] = ch[1] = nullptr;
    }

    void reverse();
    void pushdown();
    void pushup();
    void maintain();
    bool is_root();
    void rotate();
    void splay();
};

template <typename Value_type, typename Functor>
void LCT_splay<Value_type, Functor>::Node::reverse() {
    rev ^= 1;
}

template <typename Value_type, typename Functor>
void LCT_splay<Value_type, Functor>::Node::pushdown() {
    if (!rev) return;

    rev = false;
    Node* ptr = lc;
    lc = rc;
    rc = ptr;

    if (lc != nullptr) lc->reverse();
    if (rc != nullptr) rc->reverse();
}

template <typename Value_type, typename Functor>
void LCT_splay<Value_type, Functor>::Node::pushup() {
    if (!is_root()) this->ftr->pushup();

    pushdown();
}

template <typename Value_type, typename Functor>
void LCT_splay<Value_type, Functor>::Node::maintain() {
    sum = val;
    Functor _func;
    if (lc != nullptr) sum = _func(lc->sum, sum);
    if (rc != nullptr) sum = _func(sum, rc->sum);
}

template <typename Value_type, typename Functor>
bool LCT_splay<Value_type, Functor>::Node::is_root() {
    return ftr == nullptr || (ftr->lc != this && ftr->rc != this);
}

template <typename Value_type, typename Functor>
void LCT_splay<Value_type, Functor>::Node::rotate() {
    Node *nftr = ftr, *gftr = ftr->ftr;
    bool is_rc = nftr->rc == this;
    bool is_rf = gftr != nullptr && gftr->rc == nftr;
    ftr = gftr;

    if (!nftr->is_root()) gftr->ch[is_rf] = this;

    nftr->ch[is_rc] = this->ch[!is_rc];

    if (this->ch[!is_rc] != nullptr) this->ch[!is_rc]->ftr = nftr;

    nftr->ftr = this;
    this->ch[!is_rc] = nftr;
    nftr->maintain();
    maintain();
}

template <typename Value_type, typename Functor>
void LCT_splay<Value_type, Functor>::Node::splay() {
    pushup();

    while (!is_root()) {
        Node *nftr = ftr, *gftr = ftr->ftr;

        if (nftr->is_root())
            rotate();
        else {
            if ((gftr->lc == nftr) ^ (nftr->lc == this))
                rotate();
            else
                nftr->rotate();

            rotate();
        }
    }
}

template <typename Value_type, typename Functor>
typename LCT_splay<Value_type, Functor>::Node*
LCT_splay<Value_type, Functor>::__new_node(const Value_type& v) {
    return new Node(v);
}
}  // namespace detail

template <typename Value_type, typename Functor>
class link_cut_tree : public detail::LCT_splay<Value_type, Functor>
/*
Notice:must be guaranteed to be a tree after each operation.
*/
{
    typedef typename detail::LCT_splay<Value_type, Functor>::Node Node;

   private:
    void access(Node*);
    void make_root(Node*);
    Node* find_root(Node*);
    bool split(Node*, Node*);

   public:
    link_cut_tree() {}
    link_cut_tree(const link_cut_tree& other) { *this = other; }

    struct iterator;

    iterator insert(const Value_type&);
    /*
    Insert a node with weight.
    */

    bool link(const iterator&, const iterator&);
    /*
    Connect an edge between the two nodes,
    and return whether it's successful.
    */

    bool cut(const iterator&, const iterator&);
    /*
    Delete the edge between the two nodes,
    and return whether it's successful.
    */

    std::pair<bool, Value_type> query(iterator, iterator);
    /*
    Query the sum of the weights on the path between the two nodes,
    and return whether it's successful.
    */

    bool modify(const iterator&, const Value_type&);
    /*
    Modify the weight of the node,
    and return whether it's successful.
    */

    iterator LCA(const iterator&, const iterator&);
    /*
    Query the LCA(least common ancestor) of two nodes.
    */
};

template <typename Value_type, typename Functor>
struct link_cut_tree<Value_type, Functor>::iterator {
   private:
    Node* _real_node;
    friend class link_cut_tree;

   public:
    iterator(Node* p = nullptr) : _real_node(p) {}
    iterator(const iterator& iter) : _real_node(iter._real_node) {}

    const Value_type operator*() const { return _real_node->val; }

    bool operator==(const iterator& iter) {
        return _real_node == iter._real_node;
    }

    bool operator!=(const iterator& iter) { return !(*this == iter); }

    iterator operator=(const iterator& iter) {
        _real_node = iter._real_node;
        return *this;
    }
};

template <typename Value_type, typename Functor>
void link_cut_tree<Value_type, Functor>::access(Node* ptr) {
    for (Node* nptr = nullptr; ptr != nullptr; nptr = ptr, ptr = ptr->ftr) {
        ptr->splay();
        ptr->rc = nptr;
        ptr->maintain();
    }
}

template <typename Value_type, typename Functor>
void link_cut_tree<Value_type, Functor>::make_root(Node* ptr) {
    access(ptr);
    ptr->splay();
    ptr->reverse();
}

template <typename Value_type, typename Functor>
typename link_cut_tree<Value_type, Functor>::Node*
link_cut_tree<Value_type, Functor>::find_root(Node* ptr) {
    access(ptr);
    ptr->splay();

    while (ptr->lc != nullptr) ptr->pushdown(), ptr = ptr->lc;

    ptr->splay();
    return ptr;
}

template <typename Value_type, typename Functor>
bool link_cut_tree<Value_type, Functor>::split(Node* sptr, Node* eptr) {
    make_root(sptr);

    if (find_root(eptr) != sptr) return false;

    eptr->splay();
    return true;
}

template <typename Value_type, typename Functor>
typename link_cut_tree<Value_type, Functor>::iterator
link_cut_tree<Value_type, Functor>::insert(const Value_type& v) {
    return iterator(detail::LCT_splay<Value_type, Functor>::__new_node(v));
}

template <typename Value_type, typename Functor>
bool link_cut_tree<Value_type, Functor>::link(const iterator& siter,
                                              const iterator& eiter) {
    Node *sptr = siter._real_node, *eptr = eiter._real_node;
    make_root(sptr);

    if (find_root(eptr) == sptr) return false;

    sptr->ftr = eptr;

    return true;
}

template <typename Value_type, typename Functor>
bool link_cut_tree<Value_type, Functor>::cut(const iterator& siter,
                                             const iterator& eiter) {
    Node *sptr = siter._real_node, *eptr = eiter._real_node;
    make_root(sptr);

    if (find_root(eptr) != sptr || eptr->ftr != sptr || eptr->lc != nullptr)
        return false;

    eptr->ftr = nullptr;
    sptr->lc = nullptr;
    sptr->maintain();
    return true;
}

template <typename Value_type, typename Functor>
std::pair<bool, Value_type> link_cut_tree<Value_type, Functor>::query(
    iterator siter, iterator eiter) {
    Node *sptr = siter._real_node, *eptr = eiter._real_node;

    if (!split(sptr, eptr)) return std::make_pair(false, Value_type());

    return std::make_pair(true, eptr->sum);
}

template <typename Value_type, typename Functor>
bool link_cut_tree<Value_type, Functor>::modify(const iterator& iter,
                                                const Value_type& v) {
    Node* ptr = iter._real_node;

    if (ptr == nullptr) return false;

    ptr->splay();
    ptr->val = v;
    ptr->maintain();

    return true;
}

template <typename Value_type, typename Functor>
typename link_cut_tree<Value_type, Functor>::iterator
link_cut_tree<Value_type, Functor>::LCA(const iterator& siter,
                                        const iterator& eiter) {
    Node *sptr = siter._real_node, *eptr = eiter._real_node;
    access(sptr);

    Node *now1 = sptr, *now2 = eptr;

    while (!now1->is_root()) now1 = now1->ftr;
    while (!now2->is_root()) now2 = now2->ftr;

    if (now1 == now2) return iterator(eptr);

    access(eptr);
    now1 = sptr, now2 = eptr;

    while (!now1->is_root()) now1 = now1->ftr;
    while (!now2->is_root()) now2 = now2->ftr;

    if (now1 == now2) return iterator(sptr);

    return iterator(now2->ftr);
}
}  // namespace Octinc

#endif
