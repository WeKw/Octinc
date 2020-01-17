/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_PARTITION__
#define OCTINC_PARTITION__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <cstddef>
#include <map>
#include <vector>

namespace Octinc {
template <typename __Tp>
class partition_set {
   public:
    partition_set(const __Tp = 0);
    /*
    Construct a partition (with a number of sets each containing one element).
    */
    void assign(const __Tp = 0);
    /*
    Reset the partition (to a number of sets each containing one element).
    */

    __Tp const_find(const __Tp) const;  // without path compression
    /*
    Find the representative element of the set that contains the given element
    (without path compression).
    */
    __Tp find(__Tp);  // with path compression
    /*
    Find the representative element of the set that contains the given element
    (with path compression). This mutable function compresses the search path.
    */
    __Tp size() const;
    /*
    Return the number of elements.
    */
    __Tp count() const;
    /*
    Return the number of sets.
    */

    template <typename __Iter>
    void labeling(__Iter) const;
    /*
    Output a contiguous labeling of all elements.
    */

    template <typename __Iter>
    void representatives(__Iter) const;
    /*
    Output all elements which are set representatives.
    */
    void representative_labeling(std::map<__Tp, __Tp>&) const;
    /*
    Output a contiguous labeling of the representative elements.
    */

    void merge(__Tp, __Tp);
    /*
    Merge two sets.
    */
    void insert(const __Tp);
    /*
    Insert a number of new sets, each containing one element.
    */

   private:
    std::vector<__Tp> _parents;
    std::vector<__Tp> _ranks;
    __Tp _count;
};

template <typename __Tp>
inline partition_set<__Tp>::partition_set(const __Tp siz)
    : _parents(static_cast<size_t>(siz)),
      _ranks(static_cast<size_t>(siz)),
      _count(siz) {
    for (__Tp i = 0; i < siz; ++i) _parents[static_cast<size_t>(i)] = i;
}

template <typename __Tp>
inline void partition_set<__Tp>::assign(const __Tp siz) {
    _parents.resize(static_cast<size_t>(siz));
    _ranks.resize(static_cast<size_t>(siz));
    _count = siz;

    for (__Tp i = 0; i < siz; ++i) _parents[static_cast<size_t>(i)] = i;
}

template <typename __Tp>
inline __Tp partition_set<__Tp>::size() const {
    return static_cast<__Tp>(_parents.size());
}

template <typename __Tp>
inline __Tp partition_set<__Tp>::count() const {
    return _count;
}

template <typename __Tp>
inline __Tp partition_set<__Tp>::const_find(const __Tp element) const {
    __Tp root = element;

    while (_parents[static_cast<size_t>(root)] != root)
        root = _parents[static_cast<size_t>(root)];

    return root;
}

template <typename __Tp>
inline __Tp partition_set<__Tp>::find(__Tp element) {
    if (_parents[element] != element)
        return _parents[static_cast<size_t>(element)] =
                   find(_parents[static_cast<size_t>(element)]);
    return element;
}

template <typename __Tp>
inline void partition_set<__Tp>::merge(__Tp element1, __Tp element2) {
    element1 = find(element1);
    element2 = find(element2);

    if (_ranks[static_cast<size_t>(element1)] <
        _ranks[static_cast<size_t>(element2)]) {
        _parents[static_cast<size_t>(element1)] = element2;
        --_count;
    } else if (_ranks[static_cast<size_t>(element1)] >
               _ranks[static_cast<size_t>(element2)]) {
        _parents[static_cast<size_t>(element2)] = element1;
        --_count;
    } else if (element1 != element2) {
        _parents[static_cast<size_t>(element2)] = element1;
        ++_ranks[static_cast<size_t>(element1)];
        --_count;
    }
}

template <typename __Tp>
inline void partition_set<__Tp>::insert(const __Tp number) {
    const __Tp siz = static_cast<__Tp>(_parents.size());
    _ranks.insert(_ranks.end(), static_cast<size_t>(number), 0);
    _parents.insert(_parents.end(), static_cast<size_t>(number), 0);

    for (__Tp i = siz; i < siz + number; ++i)
        _parents[static_cast<size_t>(i)] = i;

    _count += number;
}

template <typename __Tp>
template <typename __Iter>
inline void partition_set<__Tp>::representatives(__Iter it) const {
    for (__Tp i = 0; i < size(); ++i)
        if (_parents[static_cast<size_t>(i)] == i) {
            *it = i;
            ++it;
        }
}

template <typename __Tp>
inline void partition_set<__Tp>::representative_labeling(
    std::map<__Tp, __Tp>& out) const {
    out.clear();
    std::vector<__Tp> r(static_cast<size_t>(count()));
    representatives(r.begin());

    for (__Tp i = 0; i < count(); ++i) out[r[static_cast<size_t>(i)]] = i;
}

template <typename __Tp>
template <typename __Iter>
inline void partition_set<__Tp>::labeling(__Iter out) const {
    std::map<__Tp, __Tp> rl;
    representative_labeling(rl);

    for (__Tp i = 0; i < size(); ++i) {
        *out = rl[find(i)];
        ++out;
    }
}
}  // namespace Octinc

#endif
