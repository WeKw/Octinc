/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_SCAPEGOAT_TREE__
#define OCTINC_SCAPEGOAT_TREE__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <malloc.h>
#include <cstring>
#include <queue>

namespace Octinc {
template <typename __Tp, typename __Cmp = std::less<__Tp> >
class scapegoat_tree {
   private:
    struct Node;
    struct mem_pool;
    mem_pool* alloc;
    Node *head, *NIL;
    Node** to;
    __Tp v;
    size_t s;
    int cnt;

    void to_array(Node* e) {
        if (e == NIL) return;
        if (e->ch[0] != NIL) to_array(e->ch[0]);
        if (e->cnt) alloc->clr[cnt++] = e;
        if (e->ch[1] != NIL) to_array(e->ch[1]);
        if (!e->cnt) alloc->remove(e);
    }

    Node* to_tree(int l, int r) {
        if (l >= r) return NIL;

        int mid = (l + r) >> 1;
        Node& e = *(alloc->clr[mid]);
        e.ch[0] = to_tree(l, mid);
        e.ch[1] = to_tree(mid + 1, r);
        e.maintain();
        return &e;
    }

    void rebuild(Node*& e) {
        if (e != NIL) {
            cnt = 0;
            to_array(e);
            e = to_tree(0, cnt);
        }
    }

    void insert(Node*& e) {
        if (e == NIL) {
            e = alloc->creat_node(Node(v, s));
            e->ch[0] = e->ch[1] = NIL;
            return;
        }

        char d = e->cmp(v);

        if (d == -1)
            e->cnt += s;
        else
            insert(e->ch[d]);

        e->maintain();

        if (e->is_bad()) {
            to = &e;
            e->rmaintain();
        }
    }

    bool remove(Node*& e) {
        if (e == NIL) return false;

        char d = e->cmp(v);

        if (d == -1)
            e->cnt = std::max(size_t(0), e->cnt - s);
        else
            remove(e->ch[d]);

        e->maintain();
    }

   public:
    static constexpr double alpha = 0.724, beta = 3.5;

    struct iterator;

    scapegoat_tree() {
        alloc = new mem_pool;
        head = NIL = alloc->creat_node(Node());
        NIL->rsize = NIL->cover = 0;
        NIL->ch[0] = NIL->ch[1] = NIL;
    }

    scapegoat_tree(const scapegoat_tree& other) { *this = other; }

    iterator insert(__Tp vs, size_t ss = 1)
    /*
    Insert keys that number of are ss.
    */
    {
        v = vs;
        s = ss;
        to = &NIL;
        insert(head);
        rebuild(*to);
        return iterator(*to);
    }

    bool remove(__Tp vs, size_t ss = 1)
    /*
    Delete keys that number of are ss,
    and return whether it's successful.
    */
    {
        v = vs;
        s = ss;
        bool flg = remove(head);

        if ((head->cover - head->rsize) > head->cover * beta) rebuild(head);

        return flg;
    }

    size_t get_rank(__Tp v)
    /*
    Query the rank of the key.
    */
    {
        Node* e = head;
        size_t k = 1;
        char d;

        while (e != NIL && (d = e->cmp(v)) != -1) {
            if (d == 1) k += e->ch[0]->size + e->cnt;

            e = e->ch[d];
        }

        return k + e->ch[0]->size;
    }

    iterator get_kth(size_t k)
    /*
    Query the kth key.
    */
    {
        Node* e = head;
        char d;

        while ((d = e->cmpkth(k)) != -1) e = e->ch[d];

        return iterator(e);
    }
};

template <typename __Tp, typename __Cmp>
struct scapegoat_tree<__Tp, __Cmp>::Node {
    __Tp val;
    size_t cnt, size, cover, rsize;
    Node* ch[2];

    Node(__Tp v = __Tp(), size_t c = 0) {
        val = v;
        cnt = size = c;
        rsize = (c != 0);
        cover = 1;
    }

    void maintain() {
        size = cnt + ch[0]->size + ch[1]->size;
        rsize = (cnt != 0 ? 1 : 0) + ch[0]->rsize + ch[1]->rsize;
        cover = 1 + ch[0]->cover + ch[1]->cover;
    }

    void rmaintain() {
        maintain();
        cover = rsize;
    }

    bool is_bad() {
        return std::max(ch[0]->cover, ch[1]->cover) > (alpha * cover + 0.4);
    }

    char cmp(__Tp v) {
        __Cmp _cmp;
        if (v == val) return -1;
        return _cmp(v, val) || v == val;
    }

    char cmpkth(size_t& k) {
        size_t p = k - ch[0]->size;

        if (p <= 0) return 0;

        k = p;
        p -= cnt;

        if (p <= 0) return -1;

        k = p;

        return 1;
    }
};

template <typename __Tp, typename __Cmp>
struct scapegoat_tree<__Tp, __Cmp>::iterator {
   private:
    Node* _real_node;
    std::pair<__Tp, size_t> pr;

   public:
    iterator() {}
    iterator(Node* ptr) : _real_node(ptr) {}
    iterator(const iterator& iter) : _real_node(iter._real_node) {}

    const std::pair<__Tp, size_t> operator*() {
        return std::make_pair(_real_node->val, _real_node->cover);
    }

    const std::pair<__Tp, size_t>* operator->() {
        pr.first = _real_node->val;
        pr.second = _real_node->cover;
        return &pr;
    }
};

template <typename __Tp, typename __Cmp>
struct scapegoat_tree<__Tp, __Cmp>::mem_pool {
    Node *begin, *end, **clr;
    unsigned long long s;
    std::queue<Node*> gc;

    void extend() {
        s <<= 1;
        end = (begin = (Node*)malloc(sizeof(Node) * s)) + s;
    }

    mem_pool() {
        s = 1 << 15;
        extend();
        clr = (Node**)malloc(sizeof(Node*) * 100010);
    }

    Node* creat_node(Node a) {
        Node* ans;

        if (gc.empty()) {
            *(ans = begin) = a;

            if (++begin == end) extend();
        } else {
            *(ans = gc.front()) = a;
            gc.pop();
        }

        return ans;
    }

    void remove(Node* a) { gc.push(a); }
};
}  // namespace Octinc

#endif
