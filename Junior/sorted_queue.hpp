/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_SORTED_QUEUE__
#define OCTINC_SORTED_QUEUE__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <algorithm>
#include <cstring>
#include <deque>

namespace Octinc {
template <typename __Tp>
class sorted_queue {
   public:
    typedef typename std::deque<__Tp>::value_type value_type;
    typedef typename std::deque<__Tp>::size_type size_type;
    typedef typename std::deque<__Tp>::reference reference;
    typedef typename std::deque<__Tp>::const_reference const_reference;
    typedef typename std::deque<__Tp>::iterator iterator;
    typedef typename std::deque<__Tp>::const_iterator const_iterator;

   protected:
    std::deque<__Tp> __Main_Q;

   public:
    sorted_queue() {}
    sorted_queue(const sorted_queue& other) { *this = other; }

    iterator begin() { return __Main_Q.begin(); }

    const_iterator begin() const { return __Main_Q.begin(); }

    iterator end() { return __Main_Q.end(); }

    const_iterator end() const { return __Main_Q.end(); }

    reference operator[](size_type __Pos_) { return __Main_Q[__Pos_]; }

    const_reference operator[](size_type __Pos_) const {
        return __Main_Q[__Pos_];
    }

    inline bool empty() const { return __Main_Q.empty(); }

    inline size_type size() const { return __Main_Q.size(); }

    reference front() { return __Main_Q.front(); }

    const_reference front() const { return __Main_Q.front(); }

    reference back() { return __Main_Q.back(); }

    const_reference back() const { return __Main_Q.back(); }

    void push(const __Tp& __Val_) {
        __Main_Q.insert(
            std::lower_bound(__Main_Q.begin(), __Main_Q.end(), __Val_), __Val_);
    }

    void pop_front() { __Main_Q.pop_front(); }

    void pop_back() { __Main_Q.pop_back(); }

    iterator erase(const_iterator __Pos_) { return __Main_Q.erase(__Pos_); }

    iterator erase(iterator __First_, iterator __Last_) {
        return __Main_Q.erase(__First_, __Last_);
    }

    void remove(const __Tp& __Val_) {
        auto __interval_ = equal_range(this->begin(), this->end(), __Val_);
        this->erase(__interval_.first, __interval_.second);
    }

    inline void clear() { __Main_Q.clear(); }
};
}  // namespace Octinc

#endif
