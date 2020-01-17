/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_TUPLE__
#define OCTINC_TUPLE__

#include <algorithm>
#include <cstdint>

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

namespace Octinc {
template <typename... __TList>
class tuple;
template <>
class tuple<> {};

typedef tuple<> nulltuple;

template <typename __Tp, typename... __TList>
class tuple<__Tp, __TList...> : public tuple<__TList...> {
   private:
    typedef tuple<__TList...> base_type;
    typedef tuple<__Tp, __TList...> this_type;

   public:
    tuple(const __Tp& v, const __TList&... tails)
        : base_type(tails...), _value(v) {}
    tuple(const this_type& other)
        : base_type(static_cast<const base_type&>(other)),
          _value(other._value) {}

    __Tp& head() { return this->_value; }

    this_type& operator=(const this_type& other) {
        base_type::operator()(static_cast<const base_type&>(other));
        _value = other._value;
        return *this;
    }

   protected:
    __Tp _value;
};

template <typename __Tp>
class tuple<__Tp> : public nulltuple {
   private:
    typedef nulltuple base_type;
    typedef tuple<__Tp> this_type;

   public:
    tuple(const __Tp& v) : _value(v) {}
    tuple(const this_type& other) : _value(other._value) {}

    __Tp& head() { return this->_value; }

    this_type& operator=(const this_type& other) {
        _value = other._value;
        return *this;
    }

   protected:
    __Tp _value;
};

template <size_t N, typename... _TList>
struct tuple_at;

template <size_t N, typename __Tp, typename... __TList>
struct tuple_at<N, tuple<__Tp, __TList...> > {
    typedef typename tuple_at<N - 1, tuple<__TList...> >::value_type value_type;
    typedef typename tuple_at<N - 1, tuple<__TList...> >::tuple_type tuple_type;
};

template <typename __Tp, typename... __TList>
struct tuple_at<0, tuple<__Tp, __TList...> > {
    typedef __Tp value_type;
    typedef tuple<__Tp, __TList...> tuple_type;
};

template <>
struct tuple_at<0, nulltuple> {
    typedef nulltuple value_type;
    typedef nulltuple tuple_type;
};

template <size_t N, typename... __TList>
constexpr const typename tuple_at<N, tuple<__TList...> >::value_type& tuple_get(
    const tuple<__TList...>& _tuple)
/*
Get the Nth element of tuple.(Constant value)
*/
{
    typedef tuple<__TList...> tuple_type;
    typedef typename tuple_at<N, tuple_type>::tuple_type base_type;

    return static_cast<const base_type&>(_tuple).head();
}

template <size_t N, typename... __TList>
typename tuple_at<N, tuple<__TList...> >::value_type& tuple_get(
    tuple<__TList...>& _tuple)
/*
Get the Nth element of tuple.
*/
{
    typedef tuple<__TList...> tuple_type;
    typedef typename tuple_at<N, tuple_type>::tuple_type base_type;

    return static_cast<base_type&>(_tuple).head();
}

template <typename... __TList>
tuple<__TList...> make_tuple(const __TList&... args)
/*
Make a tuple.
*/
{
    return tuple<__TList...>(args...);
}
}  // namespace Octinc

#endif
