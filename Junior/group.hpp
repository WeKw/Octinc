/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_GROUP__
#define OCTINC_GROUP__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <cstdint>
#include <cstdio>
#include <vector>

namespace Octinc {
template <typename __Tp>
class group {
   private:
    std::vector<__Tp> _base;
    __Tp _addtag, _multag;

    void pushup() {
        for (typename std::vector<__Tp>::iterator iter = _base.begin();
             iter != _base.end(); ++iter)
            *iter = (*iter * _multag) + _addtag;

        _addtag = (__Tp)(NULL);
        _multag = (__Tp)(1);
    }

   public:
    group() : _addtag(__Tp(NULL)), _multag(__Tp(1)) {}
    group(const std::vector<__Tp>& vec)
        : _base(vec), _addtag(__Tp(NULL)), _multag(__Tp(1)) {}
    group(const group& other)
        : _base(other._base), _addtag(other._addtag), _multag(other._multag) {}

    void insert(const __Tp& v)
    /*
    Insert a value,
    and pushup tags.
    */
    {
        pushup();
        _base.push_back(v);
    }

    bool empty() const { return _base.empty(); }

    size_t size() const { return _base.size(); }

    void clear() {
        _base.clear();
        _addtag = __Tp(NULL);
        _multag = __Tp(1);
    }

    group operator=(const std::vector<__Tp>& vec) {
        _base = vec;
        return *this;
    }

    group operator+(const __Tp& v)
    /*
    Add v to all elements in the set.
    */
    {
        group g(*this);
        g._addtag += v;
        return g;
    }

    group operator+=(const __Tp& v) {
        _addtag += v;
        return *this;
    }

    group operator-(const __Tp& v)
    /*
    Add v to all elements in the set.
    */
    {
        group g(*this);
        g._addtag -= v;
        return g;
    }

    group operator-=(const __Tp& v) {
        _addtag -= v;
        return *this;
    }

    group operator*(const __Tp& v)
    /*
    Multiply all elements in the set by v.
    */
    {
        group g(*this);
        g._addtag *= v;
        g._multag *= v;
        return g;
    }

    group operator*=(const __Tp& v) {
        _addtag *= v;
        _multag *= v;
        return *this;
    }

    group operator/(const __Tp& v)
    /*
    Multiply all elements in the set by v.
    */
    {
        group g(*this);
        g._addtag /= v;
        g._multag /= v;
        return g;
    }

    group operator/=(const __Tp& v) {
        _addtag /= v;
        _multag /= v;
        return *this;
    }

    const __Tp operator[](size_t s)
    /*
    Get the sth value in the set.
    */
    {
        return _base[s] * _multag + _addtag;
    }
};
}  // namespace Octinc

#endif
