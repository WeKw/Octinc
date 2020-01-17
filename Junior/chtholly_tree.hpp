/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_CHTHOLLY_TREE__
#define OCTINC_CHTHOLLY_TREE__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <algorithm>
#include <map>
#include <set>
#include <vector>

namespace Octinc {
namespace detail {
template <typename __Tp>
struct chtholly_node {
    mutable int l, r;
    mutable __Tp v;
    chtholly_node(int l = 0, int r = 0, __Tp v = __Tp()) : l(l), r(r), v(v) {}
    bool operator<(const chtholly_node& x) const { return l < x.l; }
};
}  // namespace detail

template <typename __Tp, const __Tp Mod = 0,
          typename __Base = std::set<detail::chtholly_node<__Tp> > >
class chtholly_tree : public __Base {
    typedef detail::chtholly_node<__Tp> Node;
    typedef typename __Base::iterator iterator;

   private:
    inline __Tp mod(__Tp x) { return Mod == 0 ? x : x % Mod; }

    iterator split(int pos) {
        iterator itl = this->lower_bound(Node(pos, -1, __Tp()));
        if (itl != this->end() && itl->l == pos) return itl;
        --itl;

        iterator itr = this->insert(Node(pos, itl->r, itl->v)).first;
        itl->r = pos - 1;
        return itr;
    }

   public:
    chtholly_tree() {}

    void create(int l, int r, __Tp v)
    /*
    Create new interval [l,r] with all the values in assigned v to the tree.
    */
    {
        this->insert(Node(l, r, v));
    }

    void assign(int l, int r, __Tp x)
    /*
    Let all values in [l,r] be assigned x.
    */
    {
        iterator itl = this->split(l), itr = this->split(r + 1);
        this->erase(itl, itr);
        this->insert(Node(l, r, x));
    }

    void update(int l, int r, __Tp x)
    /*
    Add x to every value in the [l,r].
    */
    {
        iterator itl = this->split(l), itr = this->split(r + 1);

        for (; itl != itr; ++itl) itl->v += x;
    }

    const __Tp query_kth(int l, int r, int k)
    /*
    Query the kth value in [l,r];
    */
    {
        iterator itl = this->split(l), itr = this->split(r + 1);
        std::vector<std::pair<__Tp, int> > vec;

        for (; itl != itr; ++itl)
            vec.push_back(std::make_pair(itl->v, itl->r - itl->l + 1));

        std::sort(vec.begin(), vec.end());

        for (auto it = vec.begin(); it != vec.end(); ++it)
            if ((k -= it->second) <= 0) return it->first;

        return __Tp();
    }

    __Tp operator[](int i) { return query_sum(i, i); }

    __Tp query_sum(int l, int r)
    /*
    Query the sum of values in [l,r];
    */
    {
        iterator itl = this->split(l), itr = this->split(r + 1);
        __Tp res = __Tp(NULL);

        for (; itl != itr; ++itl)
            res = mod(res + itl->v * (itl->r - itl->l + 1));

        return res;
    }

    template <typename __Functor>
    __Tp query_sum(int l, int r, __Tp p, __Functor _pow)
    /*
    Query the sum of power of values in [l,r].
    _pow is the function to calculate power.
    */
    {
        iterator itl = this->split(l), itr = this->split(r + 1);
        __Tp res = __Tp(NULL);

        for (; itl != itr; ++itl)
            res = mod(res + _pow(itl->v, p) * (itl->r - itl->l + 1));

        return res;
    }
};
}  // namespace Octinc

#endif
