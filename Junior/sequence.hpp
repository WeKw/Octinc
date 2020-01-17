/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_SEQUENCE__
#define OCTINC_SEQUENCE__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <algorithm>
#include <cstring>
#include <iostream>
#include <set>
#include <vector>

namespace Octinc {
namespace detail {
template <typename __Tp>
void __select_kth(__Tp* a, const int l, const int r, const int k) {
    int i = l, j = r, mid = a[(l + r) >> 1];

    do {
        while (a[i] < mid) ++i;
        while (a[j] > mid) --j;

        if (i <= j) {
            std::swap(a[i], a[j]);
            ++i;
            --j;
        }
    } while (i <= j);

    if (l <= j && k <= j - l + 1) __select_kth(a, l, j, k);
    if (i <= r && k >= i - l + 1) __select_kth(a, i, r, k - (i - l));
}
}  // namespace detail
namespace sequence {
template <typename __Tp>
void radix_sort(__Tp* a, const int n)
/*
Sort the integer sequence in ascending order.
The array must be stored from subscript 1.
*/
{
    __Tp* b = new __Tp[2 * n + 1];
    size_t size_of_type = sizeof(__Tp);
    size_t num_of_buc = size_of_type >> 1;
    unsigned** r = new unsigned*[num_of_buc];
    int i, k;

    for (i = 0; i < num_of_buc; ++i)
        r[i] = new unsigned[0x10000],
        memset(r[i], 0, 0x10000 * sizeof(unsigned));

    unsigned short tmp_us;
    __Tp *j, *tar;

    for (k = 0; k < num_of_buc; ++k)
        for (j = a + 1, tar = a + n + 1; j != tar; ++j) {
            tmp_us = *(((unsigned short*)j) + k);
            ++r[k][tmp_us];
        }

    for (k = 0; k < num_of_buc; ++k)
        for (i = 1; i <= 0xffff; ++i) r[k][i] += r[k][i - 1];

    for (k = 0; k < num_of_buc; k += 0x2) {
        i = k;

        for (j = a + n; j != a; --j) {
            tmp_us = *(((unsigned short*)j) + i);
            b[r[i][tmp_us]--] = *j;
        }

        i |= 1;

        if (i == num_of_buc) break;

        for (j = b + n; j != b; --j) {
            tmp_us = *(((unsigned short*)j) + i);
            a[r[i][tmp_us]--] = *j;
        }
    }

    delete[] b;

    for (int i = 0; i < num_of_buc; ++i) delete[] r[i];

    delete[] r;
}

template <typename __Tp>
void radix_sort_float(__Tp* a, const int n)
/*
Sort the float sequence in ascending order.
(Contain float,double,long double)
The array must be stored from subscript 1.
*/
{
    radix_sort(a, n);
    std::reverse(a + 1, a + n + 1);
    std::reverse(std::upper_bound(a + 1, a + n + 1, (__Tp)(-0.0)), a + n + 1);
}

template <typename __Tp>
__Tp kth_min(__Tp* a, const int n, const int k)
/*
Find the kth smallest number in array a.
*/
{
    detail::__select_kth(a, 0, n - 1, k);
    return a[k - 1];
}

template <typename __Tp>
__Tp kth_max(__Tp* a, const int n, const int k)
/*
Find the kth biggest number in array a.
*/
{
    for (int i = 0; i < n; ++i) a[i] = -a[i];

    detail::__select_kth(a, 0, n - 1, k);

    for (int i = 0; i < n; ++i) a[i] = -a[i];

    return a[k - 1];
}

template <typename __Tp>
long long inversed_pair(__Tp* a, const int n)
/*
Find the number of inversed pairs in array a.
*/
{
    if (n == 1) return 0;

    long long sum = 0;
    int mid = n >> 1;
    sum += inversed_pair(a, mid);
    sum += inversed_pair(a + mid, n - mid);
    __Tp* b = new __Tp[2 * n + 1];
    memcpy(b, a, n * sizeof(__Tp));

    for (int i1 = 0, i2 = mid, i = 0; i1 < mid || i2 < n; ++i) {
        if (i2 == n) {
            a[i] = b[i1++];
            sum += i2 - mid;
        } else if (i1 == mid)
            a[i] = b[i2++];
        else if (b[i1] < b[i2]) {
            a[i] = b[i1++];
            sum += i2 - mid;
        } else
            a[i] = b[i2++];
    }

    delete[] b;
    return sum;
}

template <typename __Tp>
int LCS(__Tp* A, int n1, __Tp* B, int n2)
/*
Find the length of the longest common subsequence of arrays a and b.
*/
{
    int** f = new int*[2 * n1 + 1];

    for (int i = 0; i <= 2 * n1; ++i)
        f[i] = new int[2 * n2 + 1], memset(f[i], 0, (2 * n2 + 1) * sizeof(int));

    for (int i = 1; i <= n1; ++i)
        for (int j = 1; j <= n2; ++j) {
            f[i][j] = f[i - 1][j];

            if (f[i][j - 1] > f[i][j]) f[i][j] = f[i][j - 1];

            if (A[i - 1] == B[j - 1] && f[i - 1][j - 1] + 1 > f[i][j])
                f[i][j] = f[i - 1][j - 1] + 1;
        }

    for (int i = 0; i <= 2 * n1; ++i) delete[] f[i];

    delete[] f;

    return f[n1][n2];
}

template <int n>
class array_hash {
   private:
    long long* c;
    long long* fac;
    int m;

    int lowbit(int x) { return x & (-x); }

    void add(int u, int x) {
        while (u < m) {
            c[u] += x;
            u += lowbit(u);
        }
    }

    long long rnk(int u) {
        long long ret = 0;

        while (u) {
            ret += c[u];
            u -= lowbit(u);
        }

        return ret;
    }

    long long kth(int k) {
        long long ret = 0;

        for (int i = m >> 1; i; i >>= 1)
            if (c[ret + i] < k) {
                ret += i;
                k -= c[ret];
            }

        return ret + 1;
    }

   public:
    array_hash() {
        m = 1;

        while (m <= n) m <<= 1;

        c = new long long[2 * m + 1];
        fac = new long long[2 * n + 1];
        memset(c, 0, (2 * m + 1) * sizeof(long long));

        fac[0] = 1;

        for (int i = 1; i <= n; ++i) fac[i] = i * fac[i - 1];
    }
    ~array_hash() {
        delete[] fac;
        delete[] c;
    }

    long long operator()(int* a)
    /*
    Query the ranking of a given full permutation from 1 to N in all 1 to n full
    permutations, The array must be stored from subscript 1. (Cantor Expansion)
    */
    {
        for (int i = 0; i <= m; ++i) c[i] = 0;

        long long ret = 0;

        for (int i = n - 1; i >= 0; --i) {
            ret += rnk(a[i]) * fac[n - i - 1];
            add(a[i], 1);
        }

        return ret;
    }

    std::vector<int> operator()(long long x)
    /*
    Query the pos-th ranking permutation in all permutation from 1 to n.
    (Inverse Cantor Expansion).
    */
    {
        for (int i = 0; i <= m; ++i) c[i] = 0;

        std::vector<int> ret;

        for (int i = 1; i <= n; ++i) add(i, 1);

        for (int i = n - 1; i >= 0; --i) {
            int t = kth(x / fac[i] + 1);
            ret.push_back(t);
            add(t, -1);
            x %= fac[i];
        }

        return ret;
    }
};
}  // namespace sequence
}  // namespace Octinc

#endif
