/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_PAIRING_HEAP__
#define OCTINC_PAIRING_HEAP__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <algorithm>
#include <cstdint>
#include <functional>
#include <queue>

namespace Octinc {
template <typename __Tp, typename __Cmp = std::less<__Tp>,
          typename __Abs = std::less<__Tp> >
class pairing_heap : private __Cmp
/*
__Cmp is the way to sort,
__Abs is the absolute sorting way.
*/
{
   private:
    struct Node;
    Node* _root;
    size_t s;
    Node* merge(Node*, Node*);
    Node* __pop();
    bool increase(Node*, const __Tp&);
    bool decrease(Node*, const __Tp&);

   public:
    struct iterator;

    pairing_heap() {}
    pairing_heap(const pairing_heap& other) { *this = other; }

    iterator push(const __Tp&);
    /*
    Insert a node with key,
    and return the iterator of the node.
    */
    iterator top() const;
    /*
    Return the iterator of the top.
    */
    iterator pop();
    /*
    Delete the top of the heap,
    and return the iterator of the current top.
    */
    pairing_heap combine(pairing_heap*);
    /*
    Combine two heaps,
    and clear the other one.
    */
    bool modify(const iterator&, const __Tp&);
    /*
    Update the key of the node.
    */
    size_t size();
    bool empty();
};

template <typename __Tp, typename __Cmp, typename __Abs>
struct pairing_heap<__Tp, __Cmp, __Abs>::Node {
    __Tp value;
    Node *left, *child, *sibling;
    Node(const __Tp& val = __Tp())
        : value(val), left(nullptr), child(nullptr), sibling(nullptr) {}
};

template <typename __Tp, typename __Cmp, typename __Abs>
struct pairing_heap<__Tp, __Cmp, __Abs>::iterator {
   private:
    Node* _real_node;
    friend class pairing_heap;

   public:
    iterator(Node* ptr = nullptr) : _real_node(ptr) {}
    iterator(const iterator& iter) : _real_node(iter._real_node) {}

    __Tp operator*() const {
        if (_real_node == nullptr) return __Tp();
        return _real_node->value;
    }

    bool operator==(void* ptr) { return _real_node == ptr; }

    bool operator!=(void* ptr) { return _real_node != ptr; }
};

template <typename __Tp, typename __Cmp, typename __Abs>
typename pairing_heap<__Tp, __Cmp, __Abs>::Node*
pairing_heap<__Tp, __Cmp, __Abs>::merge(Node* ptr1, Node* ptr2) {
    if (ptr1 == nullptr) return ptr2;
    if (ptr2 == nullptr) return ptr1;

    if (__Cmp::operator()(ptr1->value, ptr2->value)) std::swap(ptr1, ptr2);

    ptr2->left = ptr1;
    ptr2->sibling = ptr1->child;

    if (ptr1->child != nullptr) ptr1->child->left = ptr2;

    ptr1->child = ptr2;
    return ptr1;
}

template <typename __Tp, typename __Cmp, typename __Abs>
typename pairing_heap<__Tp, __Cmp, __Abs>::Node*
pairing_heap<__Tp, __Cmp, __Abs>::__pop() {
    if (_root == nullptr) return nullptr;

    Node *son1 = _root->child, *son2;

    if (son1 == nullptr) return _root = nullptr;

    std::queue<Node*> que;

    for (Node* ptr = son1; ptr != nullptr; ptr = ptr->sibling) que.push(ptr);

    while (que.size() > 1) {
        son1 = que.front();
        que.pop();
        son2 = que.front();
        que.pop();
        son1->left = son1->sibling = nullptr;
        son2->left = son2->sibling = nullptr;
        que.push(merge(son1, son2));
    }

    delete _root;

    if (que.empty()) return _root = nullptr;
    return _root = que.front();
}

template <typename __Tp, typename __Cmp, typename __Abs>
typename pairing_heap<__Tp, __Cmp, __Abs>::iterator
pairing_heap<__Tp, __Cmp, __Abs>::push(const __Tp& val) {
    ++s;

    if (_root == nullptr) {
        _root = new Node(val);
        return iterator(_root);
    }

    Node* ptr = new Node(val);
    _root = merge(_root, ptr);
    return iterator(ptr);
}

template <typename __Tp, typename __Cmp, typename __Abs>
typename pairing_heap<__Tp, __Cmp, __Abs>::iterator
pairing_heap<__Tp, __Cmp, __Abs>::pop() {
    if (_root == nullptr) return iterator(nullptr);

    --s;
    return iterator(__pop());
}

template <typename __Tp, typename __Cmp, typename __Abs>
typename pairing_heap<__Tp, __Cmp, __Abs>::iterator
pairing_heap<__Tp, __Cmp, __Abs>::top() const {
    if (_root == nullptr) return iterator(nullptr);
    return iterator(_root);
}

template <typename __Tp, typename __Cmp, typename __Abs>
pairing_heap<__Tp, __Cmp, __Abs> pairing_heap<__Tp, __Cmp, __Abs>::combine(
    pairing_heap* h) {
    _root = merge(_root, h->_root);
    s += h->s;
    h->s = 0;
    h->_root = nullptr;
    return *this;
}

template <typename __Tp, typename __Cmp, typename __Abs>
bool pairing_heap<__Tp, __Cmp, __Abs>::decrease(Node* ptr, const __Tp& val) {
    if (ptr == nullptr || __Cmp::operator()(val, ptr->value)) return false;

    ptr->value = val;

    if (_root == ptr) return true;

    if (ptr->left->child == ptr)
        ptr->left->child = ptr->sibling;
    else
        ptr->left->sibling = ptr->sibling;

    if (ptr->sibling != nullptr) ptr->sibling->left = ptr->left;

    ptr->left = ptr->sibling = nullptr;
    _root = merge(_root, ptr);
    return true;
}

template <typename __Tp, typename __Cmp, typename __Abs>
bool pairing_heap<__Tp, __Cmp, __Abs>::increase(Node* ptr, const __Tp& val) {
    if (ptr == nullptr) return false;

    if (__Cmp::operator()(val, ptr->value)) {
        if (ptr->left->child == ptr)
            ptr->left->child = ptr->sibling;
        else
            ptr->left->sibling = ptr->sibling;

        if (ptr->sibling != nullptr) ptr->sibling->left = ptr->left;

        ptr->left = ptr->sibling = nullptr;
        Node* changed = ptr;
        std::swap(ptr, _root);
        _root = merge(ptr, __pop());
        changed->value = val;
        changed->child = nullptr;
        _root = merge(changed, _root);
        return true;
    }

    ptr->value = val;

    if (_root == ptr) return true;

    if (ptr->left->child == ptr)
        ptr->left->child = ptr->sibling;
    else
        ptr->left->sibling = ptr->sibling;

    ptr->left = ptr->sibling = nullptr;
    _root = merge(_root, ptr);
    return true;
}

template <typename __Tp, typename __Cmp, typename __Abs>
bool pairing_heap<__Tp, __Cmp, __Abs>::modify(const iterator& iter,
                                              const __Tp& val) {
    __Abs _absc;
    Node* node = iter._real_node;

    if (_absc(val, node->val))
        return decrease(node, val);
    else if (_absc(node->val, val))
        return increase(node, val);
    else
        return false;
}

template <typename __Tp, typename __Cmp, typename __Abs>
size_t pairing_heap<__Tp, __Cmp, __Abs>::size() {
    return s;
}

template <typename __Tp, typename __Cmp, typename __Abs>
bool pairing_heap<__Tp, __Cmp, __Abs>::empty() {
    return _root == nullptr;
}
}  // namespace Octinc

#endif
