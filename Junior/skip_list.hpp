/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_SKIP_LIST__
#define OCTINC_SKIP_LIST__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <algorithm>
#include <cstdint>
#include <map>
#include <random>

namespace Octinc {
template <typename __Key, typename __Value, typename __Cmp = std::less<__Key>,
          uint32_t __Lvl = 15>
class skip_list {
   private:
    struct Node;

    void new_node(Node*& p, uint32_t level = 0) {
        size_t list_size = sizeof(Node) + level * sizeof(Node*);
        p = (Node*)malloc(list_size);
    }

    uint32_t randomLevel();
    uint32_t level;
    std::size_t list_size;
    Node* header;
    Node* NIL;

   public:
    skip_list();
    skip_list(const skip_list& other) { *this = other; }
    ~skip_list();

    struct iterator : public std::iterator<std::input_iterator_tag,
                                           std::pair<__Key, __Value> >
    /*
    Class iterator.
    Note that the iterator of the skip list
    only supports forward iterations.
    */
    {
       protected:
        Node* _real_node;
        std::pair<__Key, __Value> _data;

       public:
        iterator() {}
        iterator(Node* node) : _real_node(node) {}
        iterator(const iterator& iter) : _real_node(iter._real_node) {}
        ~iterator() {}

        bool operator==(iterator iter) { return iter._real_node == _real_node; }

        bool operator!=(iterator iter) { return iter._real_node != _real_node; }

        iterator operator++(int) {
            _real_node = _real_node->forward[0];
            return *this;
        }

        iterator operator++() {
            iterator _old = *this;
            _real_node = _real_node->forward[0];
            return _old;
        }

        const std::pair<__Key, __Value> operator*() {
            return std::pair<__Key&, __Value&>(_real_node->key,
                                               _real_node->val);
        }

        const std::pair<__Key, __Value>* operator->() {
            _data.first = _real_node->key;
            _data.second = _real_node->val;
            return &_data;
        }
    };

    bool insert(const __Key, const __Value);
    /*
    Insert the key and the corresponding value,
    and return whether the insertion was successful.
    */
    bool remove(const __Key, __Value&);
    /*Delete the key and the corresponding value,
    store the value in the tot,
    and return whether the deletion is successful.
    */
    bool remove(const __Key);
    /*
    Delete the key and the corresponding value,
    and return whether the deletion is successful.
    */
    bool search(const __Key, __Value&);
    /*
    Find the value corresponding to the key,
    store it in tot,
    and return whether the key exists.
    */

    bool empty() { return size() == 0; }

    iterator begin() { return iterator(header); }
    iterator end() { return iterator(NIL); }
    __Value operator[](const __Key key) {
        __Value val;
        return search(key, val) ? val : __Value();
    }
    size_t size() { return list_size; }
};

template <typename __Key, typename __Value, typename __Cmp, uint32_t __Lvl>
struct skip_list<__Key, __Value, __Cmp, __Lvl>::Node {
    __Key key;
    __Value val;
    Node* forward[1];
};

template <typename __Key, typename __Value, typename __Cmp, uint32_t __Lvl>
skip_list<__Key, __Value, __Cmp, __Lvl>::skip_list() : list_size(0), level(0) {
    new_node(NIL);
    NIL->key = 0x7fffffff;

    new_node(header, __Lvl);

    for (int i = 0; i < __Lvl; ++i) header->forward[i] = NIL;
}

template <typename __Key, typename __Value, typename __Cmp, uint32_t __Lvl>
skip_list<__Key, __Value, __Cmp, __Lvl>::~skip_list() {
    Node* p = header;
    Node* q;

    while (p != NIL) {
        q = p->forward[0];
        free(p);
        p = q;
    }

    free(p);
}

template <typename __Key, typename __Value, typename __Cmp, uint32_t __Lvl>
uint32_t skip_list<__Key, __Value, __Cmp, __Lvl>::randomLevel() {
    uint32_t lev = 0;
    std::random_device rd;

    while (true) {
        if (rd() % 2 == 0)
            ++lev;
        else
            break;
    }

    return lev < __Lvl ? lev : __Lvl - 1;
}

template <typename __Key, typename __Value, typename __Cmp, uint32_t __Lvl>
bool skip_list<__Key, __Value, __Cmp, __Lvl>::insert(const __Key key,
                                                     const __Value val) {
    Node *update[__Lvl], *p = header;
    __Cmp _cmp;

    for (int i = level; i >= 0; --i) {
        while (p->forward[i] != NIL && _cmp(p->forward[i]->key, key))
            p = p->forward[i];

        update[i] = p;
    }

    p = p->forward[0];

    if (p != NIL && p->key == key) {
        p->val = val;
        return false;
    }

    uint32_t lev = randomLevel();

    if (lev > level) {
        lev = ++level;
        update[lev] = header;
    }

    Node* newNode;
    new_node(newNode, lev);
    newNode->key = key;
    newNode->val = val;

    for (int i = lev; i >= 0; --i) {
        p = update[i];
        newNode->forward[i] = p->forward[i];
        p->forward[i] = newNode;
    }

    ++list_size;
    return true;
}

template <typename __Key, typename __Value, typename __Cmp, uint32_t __Lvl>
bool skip_list<__Key, __Value, __Cmp, __Lvl>::remove(const __Key key,
                                                     __Value& tot) {
    Node *update[__Lvl], p = header;
    __Cmp _cmp;

    for (int i = level; i >= 0; --i) {
        while (p->forward[i] != NIL && _cmp(p->forward[i]->key, key))
            p = p->forward[i];

        update[i] = p;
    }

    p = p->forward[0];

    if (p != NIL && p->key == key) {
        tot = p->val;

        for (uint32_t i = 0; i <= level; ++i) {
            if (update[i]->forward[i] != p) break;

            update[i]->forward[i] = p->forward[i];
        }

        free(p);

        while (level > 0 && header->forward[level] == NIL) --level;

        --list_size;

        return true;
    }

    return false;
}

template <typename __Key, typename __Value, typename __Cmp, uint32_t __Lvl>
bool skip_list<__Key, __Value, __Cmp, __Lvl>::remove(const __Key key) {
    Node *update[__Lvl], *p = header;
    __Cmp _cmp;

    for (int i = level; i >= 0; --i) {
        while (p->forward[i] != NIL && _cmp(p->forward[i]->key, key))
            p = p->forward[i];

        update[i] = p;
    }

    p = p->forward[0];

    if (p != NIL && p->key == key) {
        for (uint32_t i = 0; i <= level; ++i) {
            if (update[i]->forward[i] != p) break;

            update[i]->forward[i] = p->forward[i];
        }

        free(p);

        while (level > 0 && header->forward[level] == NIL) --level;

        --list_size;

        return true;
    }

    return false;
}

template <typename __Key, typename __Value, typename __Cmp, uint32_t __Lvl>
bool skip_list<__Key, __Value, __Cmp, __Lvl>::search(const __Key key,
                                                     __Value& tot) {
    Node* p = header;
    __Cmp _cmp;

    for (int i = level; i >= 0; --i)
        while (p->forward[i] != NIL && _cmp(p->forward[i]->key, key))
            p = p->forward[i];

    p = p->forward[0];

    if (p != NIL && key == p->key) {
        tot = p->val;
        return true;
    }

    return false;
}
}  // namespace Octinc

#endif
