/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_POLYNOMIAL_HPP
#  define OCTINC_POLYNOMIAL_HPP
#  ifndef STD_BITS_STDCPP_H
#    define STD_BITS_STDCPP_H
#    include <bits/stdc++.h>
#  endif
#  include "mathmetic.hpp"

namespace Octinc {
namespace polynomial {
const int ModForm = 998244353;
}

namespace detail {
using polynomial::ModForm;

void __increase(int &x, int y) { (x += y) >= ModForm && (x -= ModForm); }
void __decrease(int &x, int y) { (x -= y) < 0 && (x += ModForm); }
int __increase(int x) { return x >= ModForm ? x - ModForm : x; }
int __decrease(int x) { return x < 0 ? x + ModForm : x; }
int __extend(int x) {
    int n = 1;
    for (; n < x; n <<= 1)
        ;
    return n;
}
}  // namespace detail

namespace polynomial {
typedef std::vector<int> polynomial;

const int GRoot = 3;

void NTT(polynomial &A, bool opt) {
    int n = A.size(), k = 0;

    for (; (1 << k) < n; ++k)
        ;

    polynomial rev(n);

    for (int i = 0; i < n; ++i) {
        rev[i] = rev[i >> 1] >> 1 | (i & 1) << (k - 1);

        if (i < rev[i]) std::swap(A[i], A[rev[i]]);
    }
    for (int l = 2; l <= n; l <<= 1) {
        int m = l >> 1, w = mathmetic::pow(GRoot, (ModForm - 1) / l, ModForm);

        if (opt) w = mathmetic::prime_inv(w, ModForm);

        for (int i = 0; i < n; i += l) {
            int wk = 1;

            for (int j = 0; j < m; ++j, wk = 1LL * wk * w % ModForm) {
                int p = A[i + j], q = 1LL * wk * A[i + j + m] % ModForm;
                A[i + j] = (p + q) % ModForm;
                A[i + j + m] = (p - q + ModForm) % ModForm;
            }
        }
    }
}

void N_DFT(polynomial &A) { NTT(A, false); }

void N_IDFT(polynomial &A) {
    NTT(A, true);
    int t = mathmetic::prime_inv(A.size(), ModForm);

    for (auto &x : A) x = 1LL * x * t % ModForm;
}

void FWTand(polynomial &A, bool opt) {
    int n = A.size();

    for (int l = 2; l <= n; l <<= 1) {
        int m = l >> 1;

        for (int i = 0; i < n; i += l)
            for (int j = 0; j < m; ++j)
                if (opt)
                    detail::__decrease(A[i + j], A[i + j + m]);
                else
                    detail::__increase(A[i + j], A[i + j + m]);
    }
}
void FWTor(polynomial &A, bool opt) {
    int n = A.size();

    for (int l = 2; l <= n; l <<= 1) {
        int m = l >> 1;

        for (int i = 0; i < n; i += l)
            for (int j = 0; j < m; ++j) {
                if (opt)
                    detail::__decrease(A[i + j + m], A[i + j]);
                else
                    detail::__increase(A[i + j + m], A[i + j]);
            }
    }
}
void FWTxor(polynomial &A, bool opt) {
    static const int I2 = (ModForm + 1) >> 1;
    int n = A.size();

    for (int l = 2; l <= n; l <<= 1) {
        int m = l >> 1;

        for (int i = 0; i < n; i += l)
            for (int j = 0; j < m; ++j) {
                int x = A[i + j], y = A[i + j + m];

                if (opt) {
                    A[i + j] = 1LL * detail::__increase(x + y) * I2 % ModForm;
                    A[i + j + m] =
                        1LL * detail::__decrease(x - y) * I2 % ModForm;
                } else {
                    A[i + j] = detail::__increase(x + y);
                    A[i + j + m] = detail::__decrease(x - y);
                }
            }
    }
}

polynomial operator+(polynomial, polynomial);
polynomial operator+(polynomial, int);
polynomial operator+(int, polynomial);
polynomial operator-(polynomial, polynomial);
polynomial operator-(polynomial, int);
polynomial operator-(int, polynomial);
polynomial operator-(polynomial);
polynomial operator*(polynomial, polynomial);
polynomial operator*(polynomial, int);
polynomial operator*(int, polynomial);
polynomial operator/(polynomial, polynomial);
polynomial operator/(polynomial, int);
polynomial operator%(polynomial, polynomial);
polynomial operator<<(polynomial, int);
polynomial operator>>(polynomial, int);
polynomial operator&(polynomial, polynomial);
polynomial operator|(polynomial, polynomial);
polynomial operator^(polynomial, polynomial);
polynomial inv(polynomial);
polynomial pow(polynomial, int);
polynomial fix(polynomial, int);
polynomial derivative(polynomial, bool);
polynomial integral(polynomial, bool);
polynomial sqrt(polynomial);
polynomial root(polynomial, int);
polynomial ln(polynomial);
polynomial exp(polynomial);
polynomial sin(polynomial);
polynomial cos(polynomial);
polynomial tan(polynomial);
polynomial asin(polynomial);
polynomial acos(polynomial);
polynomial atan(polynomial);
void print(polynomial A, char mid);

polynomial operator+(polynomial A, polynomial B) {
    int n = std::max(A.size(), B.size());
    A.resize(n), B.resize(n);

    for (int i = 0; i < n; ++i) detail::__increase(A[i], B[i]);

    return A;
}
polynomial operator+(polynomial A, int v) {
    detail::__increase(A[0], v);
    return A;
}
polynomial operator+(int v, polynomial A) {
    detail::__increase(A[0], v);
    return A;
}
polynomial operator-(polynomial A, polynomial B) {
    int n = std::max(A.size(), B.size());
    A.resize(n), B.resize(n);

    for (int i = 0; i < n; ++i) detail::__decrease(A[i], B[i]);
    return A;
}
polynomial operator-(polynomial A, int v) {
    detail::__decrease(A[0], v);
    return A;
}
polynomial operator-(int v, polynomial A) {
    A = -A, detail::__increase(A[0], v);
    return A;
}
polynomial operator-(polynomial A) {
    for (auto &x : A) x = detail::__decrease(-x);
    return A;
}
polynomial operator*(polynomial A, polynomial B) {
    int n = A.size() + B.size() - 1, N = detail::__extend(n);
    A.resize(N), N_DFT(A);
    B.resize(N), N_DFT(B);

    for (int i = 0; i < N; ++i) A[i] = 1LL * A[i] * B[i] % ModForm;

    N_IDFT(A), A.resize(n);
    return A;
}
polynomial operator*(polynomial A, int v) {
    for (auto &x : A) x = 1LL * x * v % ModForm;
    return A;
}
polynomial operator*(int v, polynomial A) {
    for (auto &x : A) x = 1LL * x * v % ModForm;
    return A;
}
polynomial operator/(polynomial A, polynomial B) {
    int n = A.size() - B.size() + 1;

    if (n <= 0) return polynomial(1, 0);

    std::reverse(A.begin(), A.end());
    std::reverse(B.begin(), B.end());
    A.resize(n), B.resize(n);
    A = fix(A * inv(B), n);
    std::reverse(A.begin(), A.end());

    return A;
}
polynomial operator/(polynomial A, int v) {
    return A * mathmetic::prime_inv(v, ModForm);
}
polynomial operator%(polynomial A, polynomial B) {
    int n = B.size() - 1;
    return fix(A - A / B * B, n);
}
polynomial inv(polynomial A) {
    int n = A.size(), N = detail::__extend(n);
    A.resize(N);
    polynomial I(N, 0);
    I[0] = mathmetic::prime_inv(A[0], ModForm);

    for (int l = 2; l <= N; l <<= 1) {
        polynomial P(l), Q(l);

        std::copy(A.begin(), A.begin() + l, P.begin());
        std::copy(I.begin(), I.begin() + l, Q.begin());

        int L = l << 1;

        P.resize(L), N_DFT(P);
        Q.resize(L), N_DFT(Q);

        for (int i = 0; i < L; ++i)
            P[i] = 1LL * Q[i] * (2 - 1LL * P[i] * Q[i] % ModForm + ModForm) %
                   ModForm;

        N_IDFT(P), P.resize(l);
        std::copy(P.begin(), P.begin() + l, I.begin());
    }

    I.resize(n);
    return I;
}
polynomial pow(polynomial A, int k) {
    int n = A.size(), x = 0;
    for (; x < n && A[x] == 0; ++x)
        ;

    if (1LL * x * k >= n) return polynomial(n, 0);

    A = A >> x;
    int v = A[0];

    return (exp(ln(A) * k) * mathmetic::pow(v, k, ModForm)) << (x * k);
}
polynomial operator<<(polynomial A, int x) {
    int n = A.size();
    polynomial B(n, 0);

    for (int i = 0; i < n - x; ++i) B[i + x] = A[i];

    return B;
}
polynomial operator>>(polynomial A, int x) {
    int n = A.size();
    polynomial B(n, 0);

    for (int i = 0; i < n - x; ++i) B[i] = A[i + x];

    return B;
}
polynomial operator&(polynomial A, polynomial B) {
    int N = detail::__extend(std::max(A.size(), B.size()));
    A.resize(N), FWTand(A, false);
    B.resize(N), FWTand(B, false);

    for (int i = 0; i < N; ++i) A[i] = 1LL * A[i] * B[i] % ModForm;

    FWTand(A, true);
    return A;
}
polynomial operator|(polynomial A, polynomial B) {
    int N = detail::__extend(std::max(A.size(), B.size()));
    A.resize(N), FWTor(A, false);
    B.resize(N), FWTor(B, false);

    for (int i = 0; i < N; ++i) A[i] = 1LL * A[i] * B[i] % ModForm;

    FWTor(A, true);
    return A;
}
polynomial operator^(polynomial A, polynomial B) {
    int N = detail::__extend(std::max(A.size(), B.size()));
    A.resize(N), FWTxor(A, false);
    B.resize(N), FWTxor(B, false);

    for (int i = 0; i < N; ++i) A[i] = 1LL * A[i] * B[i] % ModForm;

    FWTxor(A, true);
    return A;
}
polynomial fix(polynomial A, int n) {
    A.resize(n);
    return A;
}
polynomial derivative(polynomial A, bool m = true) {
    int n = A.size();

    if (n == 1) return polynomial(1, 0);

    polynomial D(n - 1, 0);

    for (int i = 1; i < n; ++i) D[i - 1] = 1LL * i * A[i] % ModForm;

    if (m) D.resize(n);

    return D;
}
polynomial integral(polynomial A, bool m = true) {
    int n = A.size();
    polynomial I(n + 1, 0);

    for (int i = 1; i <= n; ++i)
        I[i] = 1LL * mathmetic::prime_inv(i, ModForm) * A[i - 1] % ModForm;

    if (m) I.resize(n);

    return I;
}
polynomial sqrt(polynomial A) {
    int n = A.size(), N = detail::__extend(n);
    A.resize(N);
    polynomial R(N, 0);
    R[0] = mathmetic::degree(A[0], 2, ModForm);
    int i2 = mathmetic::prime_inv(2, ModForm);

    for (int l = 2; l <= N; l <<= 1) {
        polynomial P(l), Q(l);

        std::copy(A.begin(), A.begin() + l, P.begin());
        std::copy(R.begin(), R.begin() + l, Q.begin());

        polynomial I = inv(Q);
        int L = l << 1;
        P.resize(L), N_DFT(P);
        Q.resize(L), N_DFT(Q);
        I.resize(L), N_DFT(I);

        for (int i = 0; i < L; ++i) {
            Q[i] = 1LL * Q[i] * Q[i] % ModForm;
            P[i] = 1LL * (P[i] + Q[i]) * i2 % ModForm * I[i] % ModForm;
        }

        N_IDFT(P), P.resize(l);
        std::copy(P.begin(), P.begin() + l, R.begin());
    }

    R.resize(n);
    return R;
}
polynomial root(polynomial A, int k) {
    return k == 1 ? A : k == 2 ? sqrt(A) : exp(ln(A) / k);
}
polynomial ln(polynomial A) {
    assert(A[0] == 1);
    int n = A.size();
    return integral(fix(derivative(A) * inv(A), n));
}
polynomial exp(polynomial A) {
    assert(A[0] == 0);
    int n = A.size(), N = detail::__extend(n);
    A.resize(N);
    polynomial E(N, 0);
    E[0] = 1;

    for (int l = 2; l <= N; l <<= 1) {
        polynomial P = (-ln(fix(E, l)) + fix(A, l) + 1) * fix(E, l);
        std::copy(P.begin(), P.begin() + l, E.begin());
    }

    E.resize(n);
    return E;
}
polynomial sin(polynomial A) {
    assert(A[0] == 0);
    int i = mathmetic::degree(ModForm - 1, 2, ModForm);
    polynomial E = exp(i * A);
    return (E - inv(E)) / (2LL * i % ModForm);
}
polynomial cos(polynomial A) {
    assert(A[0] == 0);
    int i = mathmetic::degree(ModForm - 1, 2, ModForm);
    polynomial E = exp(i * A);
    return (E + inv(E)) / 2;
}
polynomial tan(polynomial A) {
    assert(A[0] == 0);
    int n = A.size();
    return fix(sin(A) * inv(cos(A)), n);
}
polynomial asin(polynomial A) {
    assert(A[0] == 0);
    int n = A.size();
    return integral(fix(derivative(A) * inv(sqrt(1 - fix(A * A, n))), n));
}
polynomial acos(polynomial A) {
    assert(A[0] == 0);
    return -asin(A);
}
polynomial atan(polynomial A) {
    assert(A[0] == 0);
    int n = A.size();
    return integral(fix(derivative(A) * inv(1 + fix(A * A, n)), n));
}
void print(polynomial A, char mid = ' ') {
    int n = A.size();

    for (int i = 0; i < n; ++i) printf("%d%c", A[i], i == n - 1 ? '\n' : mid);
}
}  // namespace polynomial
}  // namespace Octinc

#endif
