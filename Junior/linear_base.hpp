/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_LINEAR_BASE__
#define OCTINC_LINEAR_BASE__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <cstdint>
#include <iostream>

namespace Octinc {
template <typename __Tp, size_t MaxLog = 64>
/*
MaxLog is log(MaxValue)
*/
class linear_base {
   private:
    __Tp dig[MaxLog + 1], tmp[MaxLog + 1];
    bool flag;

   public:
    linear_base() {}

    void insert(__Tp x)
    /*
    Insert a value into  the linear base.
    */
    {
        for (int i = MaxLog; i >= 0; --i) {
            if (x & (1LL << i))
                if (!dig[i]) {
                    dig[i] = x;
                    return;
                } else
                    x ^= dig[i];
        }

        flag = true;
    }

    bool contain(__Tp x) const
    /*
    Determine whether the linear base contains the value.
    */
    {
        for (int i = MaxLog; i >= 0; --i)
            if (x & (1LL << i))
                if (!dig[i])
                    return false;
                else
                    x ^= dig[i];

        return true;
    }

    __Tp query_max() const
    /*
    Query the maximum xor sum.
    */
    {
        __Tp res = 0;

        for (int i = MaxLog; i >= 0; --i) res = std::max(res, res ^ dig[i]);

        return res;
    }

    __Tp query_min() const
    /*
    Query the minimum xor sum
    */
    {
        if (flag) return 0;
        for (int i = 0; i <= MaxLog; ++i)
            if (dig[i]) return dig[i];
    }

    __Tp query_kth(__Tp k)
    /*
    Query kth minimum xor sum
    */
    {
        __Tp res = 0;
        int cnt = 0;

        k -= flag;

        if (!k) return 0;

        for (int i = 0; i <= MaxLog; ++i) {
            for (int j = i - 1; j >= 0; --j)
                if (dig[i] & (1LL << j)) dig[i] ^= dig[j];

            if (dig[i]) tmp[cnt++] = dig[i];
        }

        if (k >= (1LL << cnt)) return (__Tp)-1;

        for (int i = 0; i < cnt; ++i)
            if (k & (1LL << i)) res ^= tmp[i];

        return res;
    }
};
}  // namespace Octinc

#endif
