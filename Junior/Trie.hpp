/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_TRIE__
#define OCTINC_TRIE__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <string>
#include <vector>

namespace Octinc {
template <const size_t CHARSET>
/*
CHARSET is the maximum ASCII code of chars.
*/
class Trie {
   private:
    enum Color {
        Red,
        Black,
    };

    struct Node {
        Color col;
        Node* nxt[CHARSET];
    };

    void clear(Node*) const;
    void insert(std::string, Node*&, int);
    void remove(std::string, Node*&, int);

    Node* find(std::string, Node*, int) const;
    int search(std::string, Node*, int, int) const;
    void collect(std::string, Node*, std::vector<std::string>&) const;
    void collect(std::string, std::string, Node*,
                 std::vector<std::string>&) const;

    Node* _root;

   public:
    Trie();
    ~Trie();

    std::vector<std::string> keys();
    /*
    Get all the string in the tree.
    */
    void insert(std::string);
    /*
    Inset a string into the tree.
    */
    void remove(std::string);
    /*
    Remove the string from the tree.
    */
    void clear();
    bool empty();
    bool find(std::string) const;
    /*
    Determine whether the string exists in the tree.
    */
    std::string longest_prefix(std::string) const;
    /*
    Query the longest prefix of the specified string in the tree.
    */
    std::vector<std::string> prefix(std::string) const;
    /*
    Query all the prefix of the specified string in the tree.
    */
    std::vector<std::string> match(std::string) const;
    /*
    Query all the matched string in the tree.
    */
};

template <const size_t CHARSET>
Trie<CHARSET>::Trie() {
    _root = new Node();
    _root->col = Black;

    for (int i = 0; i < CHARSET; ++i) _root->nxt[i] = nullptr;
}

template <const size_t CHARSET>
Trie<CHARSET>::~Trie() {
    clear(_root);
}

template <const size_t CHARSET>
void Trie<CHARSET>::clear() {
    for (int i = 0; i < CHARSET; ++i)
        if (_root->nxt[i] != nullptr) clear(_root->nxt[i]);
}

template <const size_t CHARSET>
void Trie<CHARSET>::clear(Node* ptr) const {
    for (int i = 0; i < CHARSET; ++i)
        if (ptr->nxt[i] != nullptr) clear(ptr->nxt[i]);

    delete ptr;
    ptr = nullptr;
}

template <const size_t CHARSET>
std::vector<std::string> Trie<CHARSET>::keys() {
    return prefix("");
}

template <const size_t CHARSET>
void Trie<CHARSET>::insert(std::string key) {
    insert(key, _root, 0);
}

template <const size_t CHARSET>
void Trie<CHARSET>::insert(std::string key, Node*& ptr, int d) {
    if (ptr == nullptr) {
        ptr = new Node();
        ptr->col = Black;

        for (int i = 0; i < CHARSET; ++i) ptr->nxt[i] = nullptr;
    }

    if (d == key.size()) {
        ptr->col = Red;
        return;
    }

    int c = key[d];
    insert(key, ptr->nxt[c], d + 1);
}

template <const size_t CHARSET>
void Trie<CHARSET>::remove(std::string key) {
    remove(key, _root, 0);
}

template <const size_t CHARSET>
void Trie<CHARSET>::remove(std::string key, Node*& ptr, int d) {
    if (ptr == nullptr) return;

    if (d == key.size())
        ptr->col = Black;
    else {
        int c = key[d];
        remove(key, ptr->nxt[c], d + 1);
    }

    if (ptr->col == Red) return;

    for (int i = 0; i < CHARSET; ++i)
        if (ptr->nxt[i] != nullptr) return;

    delete ptr;
    ptr = nullptr;
}

template <const size_t CHARSET>
bool Trie<CHARSET>::empty() {
    for (int i = 0; i < CHARSET; ++i)
        if (_root->nxt[i] != nullptr) return false;

    return true;
}

template <const size_t CHARSET>
bool Trie<CHARSET>::find(std::string key) const {
    Node* p = find(key, _root, 0);

    if (p == nullptr) return false;

    if (p->col == Red)
        return true;
    else
        return false;
}

template <const size_t CHARSET>
typename Trie<CHARSET>::Node* Trie<CHARSET>::find(std::string key, Node* ptr,
                                                  int d) const {
    if (ptr == nullptr) return nullptr;

    if (d == key.size()) return ptr;

    int c = key[d];

    return find(key, ptr->nxt[c], d + 1);
}

template <const size_t CHARSET>
int Trie<CHARSET>::search(std::string key, Node* ptr, int d, int len) const {
    if (ptr == nullptr) return len;

    if (ptr->col == Red) len = d;

    if (d == key.size()) return len;

    int c = key[d];

    return search(key, ptr->nxt[c], d + 1, len);
}

template <const size_t CHARSET>
std::string Trie<CHARSET>::longest_prefix(std::string key) const {
    int len = search(key, _root, 0, 0);
    return key.substr(0, len);
}

template <const size_t CHARSET>
void Trie<CHARSET>::collect(std::string key, Node* ptr,
                            std::vector<std::string>& v) const {
    if (ptr == nullptr) return;

    if (ptr->col == Red) v.push_back(key);

    for (int i = 0; i < CHARSET; ++i) collect(key + (char)i, ptr->nxt[i], v);
}

template <const size_t CHARSET>
std::vector<std::string> Trie<CHARSET>::prefix(std::string key) const {
    std::vector<std::string> v;
    collect(key, find(key, _root, 0), v);
    return v;
}

template <const size_t CHARSET>
void Trie<CHARSET>::collect(std::string pre, std::string pat, Node* ptr,
                            std::vector<std::string>& v) const {
    if (ptr == nullptr) return;

    int d = pre.size();

    if (d == pat.size() && ptr->col == Red) v.push_back(pre);

    if (d == pat.size()) return;

    char nxt = pat[d];

    for (int i = 0; i < CHARSET; ++i)
        if (nxt == '.' || nxt == i) collect(pre + (char)i, pat, ptr->nxt[i], v);
}

template <const size_t CHARSET>
std::vector<std::string> Trie<CHARSET>::match(std::string pat) const {
    std::vector<std::string> v;
    collect("", pat, _root, v);
    return v;
}
}  // namespace Octinc

#endif
