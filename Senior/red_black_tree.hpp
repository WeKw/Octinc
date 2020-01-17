/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_RED_BLAKC_TREE__
#  define OCTINC_RED_BLAKC_TREE__
#  ifndef STD_BITS_STDCPP_H
#    define STD_BITS_STDCPP_H
#    include <bits/stdc++.h>
#  endif
#  if __cplusplus < 201103L && !defined(nullptr)
#    define nullptr NULL
#  endif
namespace Octinc {
template <typename __Tp, typename __Cmp = std::less<__Tp> >
/*
__Cmp is the way to sort.
*/
class red_black_tree {
#define bro(x) (((x)->ftr->lc == (x)) ? ((x)->ftr->rc) : ((x)->ftr->lc))
#define islc(x) ((x) != nullptr && (x)->ftr->lc == (x))
#define isrc(x) ((x) != nullptr && (x)->ftr->rc == (x))
   protected:
    struct Node {
        __Tp val;
        bool RBc;
        Node* ftr;
        Node* lc;
        Node* rc;
        size_t s;
        Node(__Tp v = __Tp(), bool RB = true, Node* f = nullptr,
             Node* l = nullptr, Node* r = nullptr, size_t ss = 1)
            : val(v), RBc(RB), ftr(f), lc(l), rc(r), s(ss) {}
        Node* succ() {
            Node* ptn = rc;
            while (ptn->lc != nullptr) {
                --(ptn->s);
                ptn = ptn->lc;
            }
            return ptn;
        }
        Node* left_node() {
            Node* ptn = this;
            if (!lc) {
                while (ptn->ftr && ptn->ftr->lc == ptn) ptn = ptn->ftr;
                ptn = ptn->ftr;
            } else
                while (ptn->lc) ptn = ptn->lc;
            return ptn;
        }
        Node* right_node() {
            Node* ptn = this;
            if (!rc) {
                while (ptn->ftr && ptn->ftr->rc == ptn) ptn = ptn->ftr;
                ptn = ptn->ftr;
            } else
                while (ptn->rc) ptn = ptn->rc;
            return ptn;
        }
        void maintain() {
            s = 1;
            if (lc) s += lc->s;
            if (rc) s += rc->s;
        }
    };
    Node* _root;
    Node* _hot;
    void init(__Tp);
    void connect34(Node*, Node*, Node*, Node*, Node*, Node*, Node*);
    void SolveDoubleRed(Node*);
    void SolveDoubleBlack(Node*);
    Node* find(__Tp, const int);
    Node* rfind(__Tp, const int);
    Node* findkth(int, Node*);
    size_t find_rank(__Tp, Node*);
    void clear(Node*);

   public:
    struct iterator : public std::iterator<std::input_iterator_tag, __Tp> {
       private:
        Node* _real__node;

       public:
        iterator operator++() {
            if (_real__node == nullptr) return *this;

            _real__node = _real__node->right_node();
            return *this;
        }
        iterator operator--() {
            if (_real__node == nullptr) return *this;

            _real__node = _real__node->left_node();
            return *this;
        }
        iterator operator++(int) {
            if (_real__node == nullptr) return *this;

            iterator _old = *this;
            _real__node = _real__node->right_node();
            return _old;
        }
        iterator operator--(int) {
            if (_real__node == nullptr) return *this;
            iterator _old = *this;
            _real__node = _real__node->left_node();
            return *this;
        }
        const __Tp operator*() {
            if (_real__node == nullptr) return __Tp();

            return _real__node->val;
        }
        bool operator==(iterator other) {
            return _real__node == other._real__node;
        }
        bool operator!=(iterator other) {
            return _real__node != other._real__node;
        }
        iterator(Node* node_nn = nullptr) : _real__node(node_nn) {}
        iterator(__Tp const& val_vv) : _real__node(rfind(val_vv, 0)) {}
        iterator(iterator const& iter) : _real__node(iter._real__node) {}
    };
    red_black_tree() : _root(nullptr), _hot(nullptr) {}
    red_black_tree(const red_black_tree& other) { *this = other; }
    size_t get_rank(__Tp);
    /*
    Query the rank of the key.
    */
    iterator insert(__Tp);
    /*
    Insert a key.
    */
    bool remove(__Tp);
    /*
    Delete the key.
    */
    size_t size();
    bool empty();

    iterator begin() { return get_kth(1); }

    iterator end() { return iterator(nullptr); }

    iterator get_kth(int);
    /*
    Query the kth key.
    */
    iterator lower_bound(__Tp);
    /*
    Query the largest number which is smaller than the key.
    */
    iterator upper_bound(__Tp);
    /*
    Query the smallest number which is larger than the key.
    */
    void clear();
};
template <typename __Tp, typename __Cmp>
typename red_black_tree<__Tp, __Cmp>::iterator
red_black_tree<__Tp, __Cmp>::insert(__Tp v) {
    Node* ptn = find(v, 1);
    if (_hot == nullptr) {
        init(v);
        return iterator(_root);
    }
    ptn = new Node(v, true, _hot, nullptr, nullptr, 1);
    __Cmp _cmp;

    if (_cmp(_hot->val, v) || _hot->val == v)
        _hot->rc = ptn;
    else
        _hot->lc = ptn;
    SolveDoubleRed(ptn);
    return iterator(ptn);
}
template <typename __Tp, typename __Cmp>
void red_black_tree<__Tp, __Cmp>::init(__Tp v) {
    _root = new Node(v, false, nullptr, nullptr, nullptr, 1);
}
template <typename __Tp, typename __Cmp>
typename red_black_tree<__Tp, __Cmp>::Node* red_black_tree<__Tp, __Cmp>::find(
    __Tp v, const int op) {
    Node* ptn = _root;
    _hot = nullptr;
    __Cmp _cmp;
    while (ptn != nullptr) {
        _hot = ptn;
        ptn->s += op;
        if (_cmp(v, ptn->val))
            ptn = ptn->lc;
        else
            ptn = ptn->rc;
    }
    return ptn;
}

template <typename __Tp, typename __Cmp>
typename red_black_tree<__Tp, __Cmp>::Node* red_black_tree<__Tp, __Cmp>::rfind(
    __Tp v, const int op) {
    Node* ptn = _root;
    _hot = nullptr;
    __Cmp _cmp;
    while (ptn != nullptr && ptn->val != v) {
        _hot = ptn;
        ptn->s += op;
        if (_cmp(v, ptn->val))
            ptn = ptn->lc;
        else
            ptn = ptn->rc;
    }
    return ptn;
}
template <typename __Tp, typename __Cmp>
void red_black_tree<__Tp, __Cmp>::SolveDoubleRed(Node* nn) {
    while ((!(nn->ftr)) || nn->ftr->RBc) {
        if (nn == _root) {
            _root->RBc = false;
            return;
        }
        Node* pftr = nn->ftr;
        if (!(pftr->RBc)) return;
        Node* uncle = bro(nn->ftr);
        Node* grdftr = nn->ftr->ftr;
        if (uncle != nullptr && uncle->RBc) {
            grdftr->RBc = true;
            uncle->RBc = false;
            pftr->RBc = false;
            nn = grdftr;
        } else {
            if (islc(pftr)) {
                if (islc(nn)) {
                    pftr->ftr = grdftr->ftr;
                    if (grdftr == _root)
                        _root = pftr;
                    else if (grdftr->ftr->lc == grdftr)
                        grdftr->ftr->lc = pftr;
                    else
                        grdftr->ftr->rc = pftr;
                    connect34(pftr, nn, grdftr, nn->lc, nn->rc, pftr->rc,
                              uncle);
                    pftr->RBc = false;
                    grdftr->RBc = true;
                } else {
                    nn->ftr = grdftr->ftr;
                    if (grdftr == _root)
                        _root = nn;
                    else if (grdftr->ftr->lc == grdftr)
                        grdftr->ftr->lc = nn;
                    else
                        grdftr->ftr->rc = nn;
                    connect34(nn, pftr, grdftr, pftr->lc, nn->lc, nn->rc,
                              uncle);
                    nn->RBc = false;
                    grdftr->RBc = true;
                }
            } else {
                if (islc(nn)) {
                    nn->ftr = grdftr->ftr;
                    if (grdftr == _root)
                        _root = nn;
                    else if (grdftr->ftr->lc == grdftr)
                        grdftr->ftr->lc = nn;
                    else
                        grdftr->ftr->rc = nn;
                    connect34(nn, grdftr, pftr, uncle, nn->lc, nn->rc,
                              pftr->rc);
                    nn->RBc = false;
                    grdftr->RBc = true;
                } else {
                    pftr->ftr = grdftr->ftr;
                    if (grdftr == _root)
                        _root = pftr;
                    else if (grdftr->ftr->lc == grdftr)
                        grdftr->ftr->lc = pftr;
                    else
                        grdftr->ftr->rc = pftr;
                    connect34(pftr, grdftr, nn, uncle, pftr->lc, nn->lc,
                              nn->rc);
                    pftr->RBc = false;
                    grdftr->RBc = true;
                }
            }
            return;
        }
    }
}
template <typename __Tp, typename __Cmp>
void red_black_tree<__Tp, __Cmp>::connect34(Node* nroot, Node* nlc, Node* nrc,
                                            Node* ntree1, Node* ntree2,
                                            Node* ntree3, Node* ntree4) {
    nlc->lc = ntree1;
    if (ntree1 != nullptr) ntree1->ftr = nlc;
    nlc->rc = ntree2;
    if (ntree2 != nullptr) ntree2->ftr = nlc;
    nrc->lc = ntree3;
    if (ntree3 != nullptr) ntree3->ftr = nrc;
    nrc->rc = ntree4;
    if (ntree4 != nullptr) ntree4->ftr = nrc;
    nroot->lc = nlc;
    nlc->ftr = nroot;
    nroot->rc = nrc;
    nrc->ftr = nroot;
    nlc->maintain();
    nrc->maintain();
    nroot->maintain();
}
template <typename __Tp, typename __Cmp>
typename red_black_tree<__Tp, __Cmp>::iterator
red_black_tree<__Tp, __Cmp>::lower_bound(__Tp v) {
    Node* ptn = _root;
    __Cmp _cmp;
    while (ptn) {
        _hot = ptn;
        if (_cmp(ptn->val, v)) {
            ptn = ptn->rc;
        } else {
            ptn = ptn->lc;
        }
    }
    if (_cmp(_hot->val, v)) {
        ptn = _hot;
    } else {
        ptn = _hot->left_node();
    }
    return iterator(ptn);
}
template <typename __Tp, typename __Cmp>
typename red_black_tree<__Tp, __Cmp>::iterator
red_black_tree<__Tp, __Cmp>::upper_bound(__Tp v) {
    Node* ptn = _root;
    __Cmp _cmp;
    while (ptn) {
        _hot = ptn;
        if (_cmp(v, ptn->val)) {
            ptn = ptn->lc;
        } else {
            ptn = ptn->rc;
        }
    }
    if (_cmp(v, _hot->val)) {
        ptn = _hot;
    } else {
        ptn = _hot->right_node();
    }
    return iterator(ptn);
}
template <typename __Tp, typename __Cmp>
typename red_black_tree<__Tp, __Cmp>::iterator
red_black_tree<__Tp, __Cmp>::get_kth(int rank) {
    return iterator(findkth(rank, _root));
}

template <typename __Tp, typename __Cmp>
typename red_black_tree<__Tp, __Cmp>::Node*
red_black_tree<__Tp, __Cmp>::findkth(int rank, Node* ptn) {
    if (!(ptn->lc)) {
        if (rank == 1) {
            return ptn;
        } else {
            return findkth(rank - 1, ptn->rc);
        }
    } else {
        if (ptn->lc->s == rank - 1)
            return ptn;
        else if (ptn->lc->s >= rank)
            return findkth(rank, ptn->lc);
        else
            return findkth(rank - (ptn->lc->s) - 1, ptn->rc);
    }
}
template <typename __Tp, typename __Cmp>
size_t red_black_tree<__Tp, __Cmp>::get_rank(__Tp v) {
    return find_rank(v, _root);
}
template <typename __Tp, typename __Cmp>
size_t red_black_tree<__Tp, __Cmp>::find_rank(__Tp v, Node* ptn) {
    __Cmp _cmp;

    if (!ptn)
        return 1;
    else if (_cmp(v, ptn->val) || ptn->val == v)
        return find_rank(v, ptn->lc);
    else
        return (1 + ((ptn->lc) ? (ptn->lc->s) : 0) + find_rank(v, ptn->rc));
}
template <typename __Tp, typename __Cmp>
size_t red_black_tree<__Tp, __Cmp>::size() {
    return _root->s;
}

template <typename __Tp, typename __Cmp>
bool red_black_tree<__Tp, __Cmp>::empty() {
    return !_root;
}
template <typename __Tp, typename __Cmp>
bool red_black_tree<__Tp, __Cmp>::remove(__Tp v) {
    Node* ptn = rfind(v, -1);
    if (!ptn) return false;
    Node* node_suc;
    while (ptn->lc || ptn->rc) {
        if (!(ptn->lc)) {
            node_suc = ptn->rc;
        } else if (!(ptn->rc)) {
            node_suc = ptn->lc;
        } else {
            node_suc = ptn->succ();
        }
        --(ptn->s);
        ptn->val = node_suc->val;
        ptn = node_suc;
    }
    if (!(ptn->RBc)) {
        --(ptn->s);
        SolveDoubleBlack(ptn);
    }
    if (ptn == _root) {
        _root = nullptr;
        delete ptn;
        return true;
    }
    if (ptn->ftr->lc == ptn)
        ptn->ftr->lc = nullptr;
    else
        ptn->ftr->rc = nullptr;
    delete ptn;
    return true;
}
template <typename __Tp, typename __Cmp>
void red_black_tree<__Tp, __Cmp>::SolveDoubleBlack(Node* nn) {
    while (nn != _root) {
        Node* pftr = nn->ftr;
        Node* bthr = bro(nn);
        if (bthr->RBc) {
            bthr->RBc = false;
            pftr->RBc = true;
            if (_root == pftr) _root = bthr;
            if (pftr->ftr) {
                if (pftr->ftr->lc == pftr)
                    pftr->ftr->lc = bthr;
                else
                    pftr->ftr->rc = bthr;
            }
            bthr->ftr = pftr->ftr;
            if (islc(nn)) {
                connect34(bthr, pftr, bthr->rc, nn, bthr->lc, bthr->rc->lc,
                          bthr->rc->rc);
            } else {
                connect34(bthr, bthr->lc, pftr, bthr->lc->lc, bthr->lc->rc,
                          bthr->rc, nn);
            }
            bthr = bro(nn);
            pftr = nn->ftr;
        }
        if (bthr->lc && bthr->lc->RBc) {
            bool oldRBc = pftr->RBc;
            pftr->RBc = false;
            if (pftr->lc == nn) {
                if (pftr->ftr) {
                    if (pftr->ftr->lc == pftr)
                        pftr->ftr->lc = bthr->lc;
                    else
                        pftr->ftr->rc = bthr->lc;
                }
                bthr->lc->ftr = pftr->ftr;
                if (_root == pftr) _root = bthr->lc;
                connect34(bthr->lc, pftr, bthr, pftr->lc, bthr->lc->lc,
                          bthr->lc->rc, bthr->rc);
            } else {
                bthr->lc->RBc = false;
                if (pftr->ftr) {
                    if (pftr->ftr->lc == pftr)
                        pftr->ftr->lc = bthr;
                    else
                        pftr->ftr->rc = bthr;
                }
                bthr->ftr = pftr->ftr;
                if (_root == pftr) _root = bthr;
                connect34(bthr, bthr->lc, pftr, bthr->lc->lc, bthr->lc->rc,
                          bthr->rc, pftr->rc);
            }
            pftr->ftr->RBc = oldRBc;
            return;
        } else if (bthr->rc && bthr->rc->RBc) {
            bool oldRBc = pftr->RBc;
            pftr->RBc = false;
            if (pftr->lc == nn) {
                bthr->rc->RBc = false;
                if (pftr->ftr) {
                    if (pftr->ftr->lc == pftr)
                        pftr->ftr->lc = bthr;
                    else
                        pftr->ftr->rc = bthr;
                }
                bthr->ftr = pftr->ftr;
                if (_root == pftr) _root = bthr;
                connect34(bthr, pftr, bthr->rc, pftr->lc, bthr->lc,
                          bthr->rc->lc, bthr->rc->rc);
            } else {
                if (pftr->ftr) {
                    if (pftr->ftr->lc == pftr)
                        pftr->ftr->lc = bthr->rc;
                    else
                        pftr->ftr->rc = bthr->rc;
                }
                bthr->rc->ftr = pftr->ftr;
                if (_root == pftr) _root = bthr->rc;
                connect34(bthr->rc, bthr, pftr, bthr->lc, bthr->rc->lc,
                          bthr->rc->rc, pftr->rc);
            }
            pftr->ftr->RBc = oldRBc;
            return;
        }
        if (pftr->RBc) {
            pftr->RBc = false;
            bthr->RBc = true;
            return;
        } else {
            bthr->RBc = true;
            nn = pftr;
        }
    }
}

template <typename __Tp, typename __Cmp>
void red_black_tree<__Tp, __Cmp>::clear(Node* ptr) {
    if (ptr == nullptr) return;

    if (ptr->lc != nullptr) clear(ptr->lc);

    if (ptr->rc != nullptr) clear(ptr->rc);

    delete ptr;
}

template <typename __Tp, typename __Cmp>
void red_black_tree<__Tp, __Cmp>::clear() {
    clear(_root);
    _root = nullptr;
}

#undef bro
#undef islc
#undef isrc
}  // namespace Octinc

#endif
