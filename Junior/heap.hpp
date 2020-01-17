/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_HEAP__
#define OCTINC_HEAP__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <algorithm>
#include <functional>
#include <vector>

namespace Octinc {
template <typename __Tp, typename __Cmp = std::less<__Tp> >
class heap {
   private:
    std::vector<__Tp> _data;

   protected:
    size_t left(size_t node) const { return node * 2 + 1; }

    size_t right(size_t node) const { return (node + 1) * 2; }

    size_t parent(size_t node) const { return (node - 1) / 2; }

    void adjustdown(size_t node) {
        __Cmp cmp;
        size_t siz = _data.size();

        while (true) {
            size_t l = left(node), r = right(node), nxt = node;

            if (l < siz && cmp(_data[l], _data[node])) nxt = l;
            if (r < siz && cmp(_data[r], _data[nxt])) nxt = r;

            if (nxt != node) {
                std::swap(_data[node], _data[nxt]);
                node = nxt;
            } else
                break;
        }
    }

    void adjustup(size_t node) {
        __Cmp cmp;

        while (node > 0) {
            size_t p = parent(node);

            if (cmp(_data[node], _data[p])) {
                std::swap(_data[node], _data[p]);
                node = p;
            } else
                break;
        }
    }

   public:
    heap() {}
    heap(const heap& other) { *this = other; }

    __Tp pop()
    /*
    Delete and return the top of the heap.
    */
    {
        if (_data.empty()) return __Tp();

        __Tp _top = _data[0];
        _data[0] = std::move(_data.back());
        _data.pop_back();
        adjustdown(0);
        return _top;
    }

    __Tp& top()
    /*
    Return the top of the heap.
    */
    {
        if (_data.empty()) return *(_data.end());

        return _data[0];
    }

    void push(__Tp v)
    /*
    Insert a value into heap.
    */
    {
        _data.push_back(v);
        adjustup(_data.size() - 1);
    }

    size_t size() const { return _data.size(); }

    bool empty() const { return _data.empty(); }

    void clear() { _data.clear(); }

    void remove_if(std::function<bool(const __Tp& value)> func)
    /*
    Remove all the elments in heap satisflying func.
    */
    {
        if (_data.empty()) return;

        size_t fix_node = _data.size() - 1;

        while (true) {
            auto it = std::find_if(_data.begin(), _data.end(), func);

            if (it != _data.end()) {
                _data.erase(it);
                fix_node = std::min(fix_node,
                                    (size_t)std::distance(_data.begin(), it));
            } else
                break;
        }

        size_t max_node = _data.size();

        for (size_t node = fix_node; node < max_node; ++node) adjustup(node);
    }
};
}  // namespace Octinc

#endif
