/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_RANDOM_ACCESS_SET__
#define OCTINC_RANDOM_ACCESS_SET__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <algorithm>
#include <cstddef>
#include <functional>
#include <utility>
#include <vector>

namespace Octinc {
template <typename __Key, typename __Cmp = std::less<__Key>,
          typename __Alloc = std::allocator<__Key> >
class raccess_set {
   private:
    typedef std::vector<__Key, __Alloc> Vector;

   public:
    typedef __Key key_type;
    typedef __Key value_type;
    typedef __Cmp key_compare;
    typedef __Cmp value_compare;
    typedef __Alloc allocator_type;

    typedef typename __Alloc::const_reference const_reference;
    typedef typename Vector::iterator iterator;
    typedef typename Vector::const_iterator const_iterator;
    typedef typename Vector::reverse_iterator reverse_iterator;
    typedef typename Vector::const_reverse_iterator const_reverse_iterator;
    typedef typename Vector::size_type size_type;
    typedef typename Vector::difference_type difference_type;
    typedef typename Vector::const_pointer const_pointer;

    raccess_set(const std::size_t, const __Cmp& = __Cmp(),
                const __Alloc& = __Alloc());
    raccess_set(const __Cmp& = __Cmp(), const __Alloc& = __Alloc());
    template <typename Iterator>
    raccess_set(Iterator, Iterator, const __Cmp& = __Cmp(),
                const __Alloc& = __Alloc());

    const value_type& operator[](const size_type) const;

    const_iterator begin() const;
    const_iterator end() const;
    iterator begin();
    iterator end();

    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;
    reverse_iterator rbegin();
    reverse_iterator rend();

    const_iterator find(const key_type&) const;
    iterator find(const key_type&);

    bool empty() const;
    size_type size() const;
    size_type max_size() const;

    std::pair<const_iterator, bool> insert(const value_type&);
    template <typename Iterator>
    void insert(Iterator, Iterator);
    const_iterator insert(iterator, const value_type&);

    void erase(iterator position);
    size_type erase(const key_type&);
    void erase(iterator, iterator);
    void clear();

    size_type count(const key_type&) const;
    key_compare key_comp() const;
    value_compare value_comp() const;
    const_iterator lower_bound(const key_type&) const;
    const_iterator upper_bound(const key_type&) const;
    iterator lower_bound(const key_type&);
    iterator upper_bound(const key_type&);
    std::pair<const_iterator, const_iterator> equal_range(
        const key_type&) const;
    std::pair<iterator, iterator> equal_range(const key_type&);

    allocator_type get_allocator() const;

   private:
    std::vector<__Key> vector_;
    __Cmp compare_;
};

template <typename __Key, typename __Cmp, typename __Alloc>
inline raccess_set<__Key, __Cmp, __Alloc>::raccess_set(const __Cmp& comparison,
                                                       const __Alloc& allocator)
    : vector_(allocator), compare_(comparison) {}

template <typename __Key, typename __Cmp, typename __Alloc>
inline raccess_set<__Key, __Cmp, __Alloc>::raccess_set(
    const std::size_t reserveSize, const __Cmp& comparison,
    const __Alloc& allocator)
    : vector_(allocator), compare_(comparison) {
    vector_.reserve(reserveSize);
}

template <typename __Key, typename __Cmp, typename __Alloc>
template <typename Iterator>
inline raccess_set<__Key, __Cmp, __Alloc>::raccess_set(Iterator beginInput,
                                                       Iterator endInput,
                                                       const __Cmp& comparison,
                                                       const __Alloc& allocator)
    : vector_(allocator), compare_(comparison) {
    while (beginInput != endInput) {
        insert(*beginInput);
        ++beginInput;
    }
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline const typename raccess_set<__Key, __Cmp, __Alloc>::value_type&
    raccess_set<__Key, __Cmp, __Alloc>::operator[](
        const typename raccess_set<__Key, __Cmp, __Alloc>::size_type index)
        const {
    return vector_[index];
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline typename raccess_set<__Key, __Cmp, __Alloc>::const_iterator
raccess_set<__Key, __Cmp, __Alloc>::begin() const {
    return vector_.begin();
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline typename raccess_set<__Key, __Cmp, __Alloc>::const_iterator
raccess_set<__Key, __Cmp, __Alloc>::end() const {
    return vector_.end();
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline typename raccess_set<__Key, __Cmp, __Alloc>::const_reverse_iterator
raccess_set<__Key, __Cmp, __Alloc>::rbegin() const {
    return vector_.rbegin();
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline typename raccess_set<__Key, __Cmp, __Alloc>::const_reverse_iterator
raccess_set<__Key, __Cmp, __Alloc>::rend() const {
    return vector_.rend();
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline typename raccess_set<__Key, __Cmp, __Alloc>::iterator
raccess_set<__Key, __Cmp, __Alloc>::begin() {
    return vector_.begin();
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline typename raccess_set<__Key, __Cmp, __Alloc>::iterator
raccess_set<__Key, __Cmp, __Alloc>::end() {
    return vector_.end();
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline typename raccess_set<__Key, __Cmp, __Alloc>::reverse_iterator
raccess_set<__Key, __Cmp, __Alloc>::rbegin() {
    return vector_.rbegin();
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline typename raccess_set<__Key, __Cmp, __Alloc>::reverse_iterator
raccess_set<__Key, __Cmp, __Alloc>::rend() {
    return vector_.rend();
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline bool raccess_set<__Key, __Cmp, __Alloc>::empty() const {
    return vector_.empty();
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline typename raccess_set<__Key, __Cmp, __Alloc>::size_type
raccess_set<__Key, __Cmp, __Alloc>::size() const {
    return vector_.size();
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline typename raccess_set<__Key, __Cmp, __Alloc>::size_type
raccess_set<__Key, __Cmp, __Alloc>::max_size() const {
    return vector_.max_size();
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline std::pair<typename raccess_set<__Key, __Cmp, __Alloc>::const_iterator,
                 bool>
raccess_set<__Key, __Cmp, __Alloc>::insert(
    const typename raccess_set<__Key, __Cmp, __Alloc>::value_type& value) {
    bool found(true);
    iterator i(lower_bound(static_cast<__Key>(value)));
    if (i == end() || compare_(static_cast<__Key>(value), *i)) {
        i = vector_.insert(i, static_cast<__Key>(value));
        found = false;
    }
    return std::make_pair(i, !found);
}

template <typename __Key, typename __Cmp, typename __Alloc>
template <typename Iterator>
inline void raccess_set<__Key, __Cmp, __Alloc>::insert(Iterator first,
                                                       Iterator last) {
    for (; first != last; ++first) {
        insert(*first);
    }
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline typename raccess_set<__Key, __Cmp, __Alloc>::const_iterator
raccess_set<__Key, __Cmp, __Alloc>::insert(
    typename raccess_set<__Key, __Cmp, __Alloc>::iterator position,
    const typename raccess_set<__Key, __Cmp, __Alloc>::value_type& value) {
    std::pair<const_iterator, bool> ret;
    ret = insert(value);
    return ret.first;
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline void raccess_set<__Key, __Cmp, __Alloc>::erase(
    typename raccess_set<__Key, __Cmp, __Alloc>::iterator position) {
    vector_.erase(position);
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline typename raccess_set<__Key, __Cmp, __Alloc>::size_type
raccess_set<__Key, __Cmp, __Alloc>::erase(const key_type& x) {
    iterator i = find(x);
    if (i != vector_.end()) {
        erase(i);
        return 1;
    }
    return 0;
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline void raccess_set<__Key, __Cmp, __Alloc>::erase(
    const typename raccess_set<__Key, __Cmp, __Alloc>::iterator first,
    const typename raccess_set<__Key, __Cmp, __Alloc>::iterator last) {
    vector_.erase(first, last);
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline void raccess_set<__Key, __Cmp, __Alloc>::clear() {
    vector_.clear();
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline typename raccess_set<__Key, __Cmp, __Alloc>::key_compare
raccess_set<__Key, __Cmp, __Alloc>::key_comp() const {
    return compare_;
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline typename raccess_set<__Key, __Cmp, __Alloc>::value_compare
raccess_set<__Key, __Cmp, __Alloc>::value_comp() const {
    return compare_;
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline typename raccess_set<__Key, __Cmp, __Alloc>::const_iterator
raccess_set<__Key, __Cmp, __Alloc>::find(const key_type& value) const {
    const_iterator i(lower_bound(value));
    if (i != end() && compare_(value, *i)) {
        i = end();
    }
    return i;
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline typename raccess_set<__Key, __Cmp, __Alloc>::iterator
raccess_set<__Key, __Cmp, __Alloc>::find(const key_type& value) {
    iterator i(lower_bound(value));
    if (i != end() && compare_(value, *i)) {
        i = end();
    }
    return i;
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline typename raccess_set<__Key, __Cmp, __Alloc>::size_type
raccess_set<__Key, __Cmp, __Alloc>::count(const key_type& value) const {
    if (find(value) == end()) {
        return 0;
    } else {
        return 1;
    }
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline typename raccess_set<__Key, __Cmp, __Alloc>::const_iterator
raccess_set<__Key, __Cmp, __Alloc>::lower_bound(const key_type& value) const {
    return std::lower_bound(vector_.begin(), vector_.end(), value, compare_);
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline typename raccess_set<__Key, __Cmp, __Alloc>::iterator
raccess_set<__Key, __Cmp, __Alloc>::lower_bound(const key_type& value) {
    return std::lower_bound(vector_.begin(), vector_.end(), value, compare_);
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline typename raccess_set<__Key, __Cmp, __Alloc>::const_iterator
raccess_set<__Key, __Cmp, __Alloc>::upper_bound(const key_type& value) const {
    return std::upper_bound(vector_.begin(), vector_.end(), value, compare_);
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline typename raccess_set<__Key, __Cmp, __Alloc>::iterator
raccess_set<__Key, __Cmp, __Alloc>::upper_bound(const key_type& value) {
    return std::upper_bound(vector_.begin(), vector_.end(), value, compare_);
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline std::pair<typename raccess_set<__Key, __Cmp, __Alloc>::const_iterator,
                 typename raccess_set<__Key, __Cmp, __Alloc>::const_iterator>
raccess_set<__Key, __Cmp, __Alloc>::equal_range(const key_type& value) const {
    return std::equal_range(vector_.begin(), vector_.end(), value, compare_);
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline std::pair<typename raccess_set<__Key, __Cmp, __Alloc>::iterator,
                 typename raccess_set<__Key, __Cmp, __Alloc>::iterator>
raccess_set<__Key, __Cmp, __Alloc>::equal_range(const key_type& value) {
    return std::equal_range(vector_.begin(), vector_.end(), value, compare_);
}

template <typename __Key, typename __Cmp, typename __Alloc>
inline typename raccess_set<__Key, __Cmp, __Alloc>::allocator_type
raccess_set<__Key, __Cmp, __Alloc>::get_allocator() const {
    return vector_.get_allocator();
}

}  // namespace Octinc

#endif
