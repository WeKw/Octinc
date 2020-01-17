/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#if !defined(OCTINC_MATHMETIC_HPP) && __cplusplus >= 201103L
#  define OCTINC_MATHMETIC_HPP
#  ifndef STD_BITS_STDCPP_H
#    define STD_BITS_STDCPP_H
#    include <bits/stdc++.h>
#  endif
namespace Octinc {
namespace mathmetic {
typedef long long LL;

inline LL gcd(LL a, LL b)
/*
Calculate the greatest common divisor(gcd) of a and b.
*/
{
    return b == 0 ? a : gcd(b, a % b);
}

LL multiply(LL x, LL y, LL mod)
/*
Calculate x*y%mod
*/
{
    return (x * y - (LL)((long double)x / mod * y) * mod + mod) % mod;
}

LL exgcd(LL a, LL b, LL& x, LL& y)
/*
Calculate the gcd of a and b
and return a set of solutions of ax+by=gcd(a,b)
*/
{
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    } else {
        LL r = exgcd(b, a % b, y, x);
        y -= x * (a / b);
        return r;
    }
}

inline LL pow(LL a, LL b, LL mod)
/*
Calculate a^b%mod
*/
{
    LL res = 1;

    while (b) {
        if (b & 1) res = multiply(res, a, mod) % mod;

        b >>= 1;
        a = multiply(a, a, mod) % mod;
    }

    return res;
}

LL prime_inv(LL x, LL m)
/*
Calculate the inverse element of multiplication in the sense of modulus m of x.
(m has to be a prime).
*/
{
    return pow(x, m - 2, m);
}

LL inv(LL a, LL m)
/*
Calculate the inverse element of multiplication in the sense of modululs m of x.
*/
{
    LL x, y;
    exgcd(a, m, x, y);

    if (x < 0) {
        x = -x;
        x = m - x;
    }

    return (m + x % m) % m;
}

LL discrete_log(LL a, LL b, LL p)
/*
Calculate the discrete logarithm of a and based on b in the sense of modulus m,
That is, the integer k satisfying b^k=a(mod m).
*/
{
    std::unordered_map<LL, LL> mp;
    LL m = ceil(sqrt(p));

    for (LL i = 0; i <= m; b = multiply(b, a, p), ++i) mp[b] = i;

    a = pow(a, m, p);

    for (LL i = 0, j = 1; i < m; j = multiply(j, a, p), ++j)
        if (mp.count(j) && i * m >= mp[j]) return i * m - mp[j];

    return -1;
}

LL primitive_root(LL p)
/*
Calculate the primitive root in the sense of modulus p.
(p has to be a prime).
*/
{
    std::vector<LL> fac;
    LL phi = p - 1, x = phi;

    for (LL i = 2; i * i <= x; ++i)
        if (x % i == 0) {
            fac.push_back(i);

            while (x % i == 0) x /= i;
        }

    if (x > 1) fac.push_back(x);

    for (LL i = 2; i <= phi; ++i) {
        bool flg = true;

        for (auto j : fac)
            if (pow(i, phi / j, p) == 1) {
                flg = false;
                break;
            }

        if (flg) return i;
    }

    return -1;
}

LL degree(LL a, LL k, LL p)
/*
Calculate the degree of a based on k in the sense of modulus p.
*/
{
    LL g = primitive_root(p);
    LL x = discrete_log(g, a, p);
    assert(x >= 0 && x % k == 0);
    LL r = pow(g, x / k, p);
    return std::min(r, p - r);
}

std::vector<LL> linear_mod_equation(LL a, LL b, LL n)
/*
Calculate all the solutions of linear modulus equation ax=b(mod n).
*/
{
    LL x, y;
    LL d = exgcd(a, n, x, y);
    std::vector<LL> ans;
    ans.clear();

    if (b % d == 0) {
        x = (x % n + n) % n;
        ans.push_back(multiply(x, b / d, n / d));

        for (LL i = 1; i < d; ++i)
            ans.push_back((ans[0] + multiply(i, n / d, n)) % n);
    }

    return ans;
}

LL CRT(std::vector<int> a, std::vector<int> m)
/*
Solving the Chinese Remainder Theorem(CRT).
*/
{
    int n = a.size();
    LL M = 1;

    for (LL i = 0; i < n; ++i) M *= m[i];

    LL ret = 0;

    for (LL i = 0; i < n; ++i) {
        LL x, y;
        LL tm = M / m[i];
        exgcd(tm, m[i], x, y);
        ret = (ret + multiply(multiply(tm, x, M), a[i], M)) % M;
    }

    return (ret + M) % M;
}

LL mod_sqr(LL a, LL n)
/*
Calculate the solution of a^2=1(mod n).
(n has to be a prime).
*/
{
    LL b, k, i, x;

    if (n == 2) return a % n;

    if (pow(a, (n - 1) / 2, n) == 1) {
        if (n % 4 == 3)
            x = pow(a, (n + 1) / 4, n);
        else {
            for (b = 1; pow(b, (n - 1) / 2, n) == 1; ++b)
                ;

            i = (n - 1) / 2;
            k = 0;

            do {
                i /= 2;
                k /= 2;

                if ((multiply(pow(a, i, n), pow(b, k, n), n) + 1) % n == 0)
                    k += (n - 1) / 2;
            } while (i % 2 == 0);

            x = multiply(pow(a, (i + 1) / 2, n), pow(b, k / 2, n), n);
        }

        if (x * 2 > n) x = n - x;
    }

    return x;
}

std::vector<LL> residue(LL a, LL N, LL p)
/*
Calculate all the solutions of a^n=1(mod p).
(p has to be a prime).
*/
{
    LL g = primitive_root(p);
    LL m = discrete_log(g, a, p);
    std::vector<LL> ret;

    if (a == 0) {
        ret.push_back(0);
        return ret;
    }

    if (m == -1) return ret;

    LL A = N, B = p - 1, C = m, x, y;
    LL d = exgcd(A, B, x, y);

    if (C % d != 0) return ret;

    x = x * (C / d) % B;

    LL delta = B / d;

    for (int i = 0; i < d; ++i) {
        x = ((x + delta) % B + B) % B;
        ret.push_back(pow(g, x, p));
    }

    sort(ret.begin(), ret.end());
    ret.erase(unique(ret.begin(), ret.end()), ret.end());
    return ret;
}

void sieve_prime(int n, int ans[], int& tot)
/*
Sieve out all prime in [1,n] and stores them in the array ans.
tot denotes the number of primes.
*/
{
    bool* valid = new bool[2 * n + 1];
    memset(valid, 1, (2 * n + 1) * sizeof(bool));

    for (int i = 2; i <= n; ++i) {
        if (valid[i]) {
            ++tot;
            ans[tot] = i;
        }

        for (int j = 1; ((j <= tot) && (i * ans[j] <= n)); ++j) {
            valid[i * ans[j]] = false;

            if (i % ans[j] == 0) break;
        }
    }
}

bool __prime_test(LL x, LL p) { return pow(x, p - 1, p) == 1; }

bool __prime_Test(LL x, LL p) {
    if (!__prime_test(x, p)) return 0;

    LL k = p - 1;

    while (!(k & 1)) {
        k >>= 1;
        LL t = pow(x, k, p);

        if (t != 1 && t != p - 1) return 0;
        if (t == p - 1) return 1;
    }

    return 1;
}

bool is_prime(LL p)
/*
Determine whether p is a prime.
false means that p must be a composite number,
true means that p has a high probability of being a prime number.
It must be correct in the range of long long.
*/
{
    if (p == 1 || p == 2152302898747LL) return 0;

    LL t[] = {2, 3, 5, 7, 11};

    for (int i = 0; i <= 4; ++i)
        if (p == t[i]) return 1;

    for (int i = 0; i <= 4; ++i)
        if (!__prime_Test(t[i], p)) return 0;

    return 1;
}

void __Pollard_Rho(LL x, LL& Ans) {
    if (is_prime(x)) {
        Ans = std::max(x, Ans);
        return;
    }
    LL t1 = 1LL * rand() * rand() % (x - 1) + 1;
    LL t2 = t1, b = 1LL * rand() % (x - 1) + 1;
    t2 = (multiply(t2, t2, x) + b) % x;
    LL p = 1;
    LL i = 0;
    while (t1 != t2) {
        ++i;
        p = multiply(p, abs(t1 - t2), x);
        if (p == 0) {
            int t = gcd(abs(t1 - t2), x);
            if (t != 1 && t != x) {
                __Pollard_Rho(t, Ans), __Pollard_Rho(x / t, Ans);
            }
            return;
        }
        if (i % 127 == 0) {
            p = gcd(p, x);
            if (p != 1 && p != x) {
                __Pollard_Rho(p, Ans), __Pollard_Rho(x / p, Ans);
                return;
            }
            p = 1;
        }
        t1 = (multiply(t1, t1, x) + b) % x;
        t2 = (multiply(t2, t2, x) + b) % x;
        t2 = (multiply(t2, t2, x) + b) % x;
    }
    p = gcd(p, x);
    if (p != 1 && p != x) {
        __Pollard_Rho(p, Ans), __Pollard_Rho(x / p, Ans);
        return;
    }
}

LL prime_divisor(LL x)
/*
Calculate the maximum prime factor of x and store it in Ans.
*/
{
    LL Ans = 0;

    while (!Ans) __Pollard_Rho(x, Ans);

    return Ans;
}

void factor(int n, int a[], int b[], int& tot)
/*
Factor n.
a[i] denotes different factors.
b[i] denotes the index of a[i].
tot denotes the number of factors.
*/
{
    int tmp, now;
    tmp = ceil(sqrt(n)) + 1;
    tot = 0;
    now = n;

    for (int i = 2; i <= tmp; ++i)
        if (now % i == 0) {
            a[++tot] = i;
            b[tot] = 0;

            while (now % i == 0) {
                ++b[tot];
                now /= i;
            }
        }

    if (now != 1) {
        a[++tot] = now;
        b[tot] = 1;
    }
}

void sieve_phi(int n, int phi[])
/*
Sieve out the Euler function(phi) of each integer in [1,n],
and stores them in array phi.
*/
{
    int *mindiv = new int[2 * n + 1], *sum = new int[2 * n + 1];
    memset(mindiv, 0, (2 * n + 1) * sizeof(int));
    memset(sum, 0, (2 * n + 1) * sizeof(int));

    for (int i = 1; i <= n; ++i) mindiv[i] = i;

    for (int i = 2; 1LL * i * i < (LL)n; ++i)
        if (mindiv[i] == i)
            for (LL j = i * i; j <= n; j += i) mindiv[j] = i;

    phi[1] = 1;

    for (int i = 2; i <= n; ++i) {
        phi[i] = phi[i / mindiv[i]];

        if ((i / mindiv[i]) % mindiv[i] == 0)
            phi[i] *= mindiv[i];
        else
            phi[i] *= mindiv[i] - 1;
    }
}

void sieve_mu(int n, int mu[])
/*
Sieve out the Mobius function(mu) of each integer in [1,n],
and stores them in array mu.
*/
{
    for (int i = 1; i <= n; ++i) {
        int tar = (i == 1 ? 1 : 0);
        int delta = tar - mu[i];
        mu[i] = delta;

        for (int j = 2 * i; j <= n; j += i) mu[j] += delta;
    }
}

void sieve_inv(int n, int p, int iv[])
/*
Sieve out the inverse elements of each integer in [1,n],
and stores them in array iv.
(p has to be a prime).
*/
{
    iv[1] = 1;

    for (int i = 2; i <= n; ++i) iv[i] = (p - p / i) * iv[p % i] % p;
}

template <class __Func>
double calc_integral(const __Func& f, double a, double b, double eps = 1e-8)
/*
Calculate the integral of function f in interval [a,b].
*/
{
    std::vector<double> t;
    double h = b - a, last, cur;
    int k = 1, i = 1;
    t.push_back(h * (f(a) + f(b)) / 2.0);

    do {
        last = t.back();
        cur = 0;
        double x = a + h / 2.0;

        for (int j = 0; j < k; ++j) {
            cur += f(x);
            x += h;
        }

        cur = (t[0] + h * cur) / 2.0;
        double k1 = 4.0 / 3.0, k2 = 1.0 / 3.0;

        for (int j = 0; j < i; ++j) {
            double tmp = k1 * cur - k2 * t[j];
            t[j] = cur;
            cur = tmp;
            k2 /= 4.0 * k1 - k2;
            k1 = k2 + 1.0;
        }

        t.push_back(cur);
        k *= 2;
        h /= 2.0;
        ++i;
    } while (fabs(last - cur) > eps);

    return t.back();
}

#define sign(x) (x < eps ? -1 : x > eps)
#define INF (1e12)

double interpolation(const std::vector<double>& coef, double x)
/*
Calculate the value of the polynomial coef(stores each coefficient) at the point
x.
*/
{
    double e = 1.0, s = 0.0;

    for (int i = 0; i < coef.size(); ++i) {
        s += coef[i] * e;
        e *= x;
    }

    return s;
}

double __equation_find_root(const std::vector<double>& coef, int n, double l,
                            double r, double eps = 1e-8) {
    double signl, signr;

    if ((signl = sign(interpolation(coef, l))) == 0) return l;
    if ((signr = sign(interpolation(coef, r))) == 0) return r;

    if (signl * signr > 0) return INF;

    for (int step = 0; step < 100 && r - l > eps; ++step) {
        double mid = (l + r) * 0.5;
        int signm = sign(interpolation(coef, mid));

        if (signm == 0) return mid;

        if (signl * signm < 0)
            r = mid;
        else
            l = mid;
    }

    return (l + r) * 0.5;
}

std::vector<double> solve_equation(std::vector<double> coef, double eps = 1e-8)
/*
Solve the polynomial equation, (coef denotes each coefficient),
and stores all the real solutions in a STL vector.
*/
{
    int n = coef.size() - 1;
    std::vector<double> ret;

    if (n == 1) {
        if (sign(coef[1])) ret.push_back(-coef[0] / coef[1]);

        return ret;
    }

    std::vector<double> dcoef(n);

    for (int i = 0; i < n; ++i) dcoef[i] = coef[i + 1] * (i + 1);

    std::vector<double> droot = solve_equation(dcoef);
    droot.insert(droot.begin(), -INF);
    droot.push_back(INF);

    for (int i = 0; i + 1 < droot.size(); ++i) {
        double tmp = __equation_find_root(coef, n, droot[i], droot[i + 1], eps);

        if (tmp < INF) ret.push_back(tmp);
    }

    return ret;
}

#undef INF
#undef sign

std::string conversion(int x, int y, std::string s)
/*
Converts an x-ary number to a y-ary number.
Numbers are stored as STL strings.
*/
{
    std::string res = "";
    LL sum = 0;

    for (int i = 0; i < s.size(); ++i) {
        if (s[i] == '-') continue;

        if (s[i] >= '0' && s[i] <= '9')
            sum = sum * x + s[i] - '0';
        else
            sum = sum * x + s[i] - 'A' + 10;
    }

    while (sum) {
        char tmp = sum % y;
        sum /= y;

        if (tmp <= 9)
            tmp += '0';
        else
            tmp = tmp - 10 + 'A';

        res = tmp + res;
    }

    if (res.size() == 0) res = "0";

    if (s[0] == '-') res = '-' + res;

    return res;
}

std::vector<int> gray_code(int n)
/*
Calculate the gray code of n.
*/
{
    std::vector<int> res;
    res.clear();

    for (int i = 0; i < (1 << n); ++i) res.push_back(i ^ (i >> 1));

    return res;
}

int what_day(int y, int m, int d)
/*
Calculate what day today is.
*/
{
    int ans;

    if (m == 1 || m == 2) {
        m += 12;
        --y;
    }

    if ((y < 1752) || (y == 1752 && m < 9) || (y == 1752 && m == 9 && d < 3))
        ans = (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 + 5) % 7;
    else
        ans = (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400) % 7;

    return ans;
}

#define days (365)
#define is_leap(y) (y % 400 == 0 || y % 100 && y % 4 == 0)
#define leap(y) (y / 4 - y / 100 + y / 400)

LL __days_calc(int y, int m, int d) {
    static int s[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    LL res = (y - 1) * days + leap(y - 1);

    for (int i = 1; i < m; ++i) res += s[i];

    if (is_leap(y) && m > 2) ++res;

    res += d;

    return res;
}

LL days_between(int ya, int ma, int da, int yb, int mb, int db)
/*
Calculate the number of days between the two days.
*/
{
    return abs(__days_calc(ya, ma, da) - __days_calc(yb, mb, db));
}
#undef leap
#undef is_leap
#undef days

std::vector<int> fibonacci_conversion(int n)
/*
Calculate the Fibonacci binary of n.
That is, let n be Sigma(b[i]*fib[i]), b is the result.
*/
{
    int* fib = new int[2 * n + 1];
    int lim, i = 2;
    fib[0] = fib[1] = 1;

    while (true) {
        fib[i] = fib[i - 1] + fib[i - 2];

        if (fib[i] > n) {
            lim = i;
            break;
        }

        ++i;
    }

    std::vector<int> ret;

    for (int i = lim - 1; i > 0; --i)
        if (fib[i] <= n) {
            ret.push_back(1);
            n -= fib[i];
        } else
            ret.push_back(0);

    return ret;
}

std::string rome_conversion(int a)
/*
Converts s to Roman numerals.
*/
{
    std::string s;
    int i, j;

    if (a >= 1000) {
        i /= 1000;

        for (j = 0; j < i; ++j) s = s + "M";

        a -= 1000 * i;
    }

    if (a >= 900) {
        s = s + "CM";
        a -= 900;
    }
    if (a >= 500) {
        s = s + "D";
        a -= 500;
    }
    if (a >= 400) {
        s = s + "CD";
        a -= 400;
    }
    if (a >= 100) {
        i = a / 100;

        for (j = 0; j < i; ++j) s = s + "C";

        a -= 100 * i;
    }
    if (a >= 90) {
        s = s + "XC";
        a -= 900;
    }
    if (a >= 50) {
        s = s + "L";
        a -= 50;
    }
    if (a >= 40) {
        s = s + "XL";
        a -= 40;
    }
    if (a >= 10) {
        i = a / 10;

        for (j = 0; j < i; ++j) s = s + "X";

        a -= 10 * i;
    }
    if (a >= 9) {
        s = s + "IX";
        a -= 9;
    }
    if (a >= 5) {
        s = s + "V";
        a -= 5;
    }
    if (a >= 4) {
        s = s + "IV";
        a -= 4;
    }

    for (j = 0; j < a; ++j) s = s + "I";

    return s;
}

double __calculate_operator(double a, double b, char op) {
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;
    if (op == '/') return a / b;
    if (op == '^') return exp(log(a) * b);
    fprintf(stderr, "MATHMETIC:Operation Error.\n");
}

double calculate(std::string str, double val[])
/*
Calculate the expression.
The expression allows four operation symbols and power operation (^),
as well as variables and real numbers represented by a single letter.
The values of variables are stored in array val.
*/
{
    std::stack<double> den;
    std::stack<char> oper;
    static int priv[300];
    priv['+'] = priv['-'] = 3;
    priv['*'] = priv['/'] = 2;
    priv['^'] = 1;
    priv['('] = 10;
    double x, y, t = 0.0;
    char last = 0;

    for (int i = 0; i < str.size(); ++i) {
        if (std::isalpha(str[i]))
            den.push(val[str[i]]);
        else if (std::isdigit(str[i])) {
            den.push(atof(str.c_str() + i));

            while (i + 1 < str.size() && std::isdigit(str[i + 1])) ++i;

            if (i + 1 < str.size() && str[i + 1] == '.')
                for (++i; i + 1 < str.size() && std::isdigit(str[i + 1]); ++i)
                    ;
        } else if (str[i] == '(')
            oper.push(str[i]);
        else if (str[i] == ')') {
            while (oper.top() != '(') {
                y = den.top();
                den.pop();
                x = den.top();
                den.pop();
                char op = oper.top();
                oper.pop();
                den.push(__calculate_operator(x, y, op));
            }

            oper.pop();
        } else if (str[i] == '-' && (last == 0 || last == '(')) {
            den.push(0.0);
            oper.push('-');
        } else if (priv[str[i]] > 0) {
            while (oper.size() > 0 && priv[str[i]] >= priv[oper.top()]) {
                y = den.top();
                den.pop();
                x = den.top();
                den.pop();
                char op = oper.top();
                oper.pop();
                den.push(__calculate_operator(x, y, op));
            }

            oper.push(str[i]);
        } else
            continue;

        last = str[i];
    }

    while (!oper.empty()) {
        x = den.top();
        den.pop();
        y = den.top();
        den.pop();
        char op = oper.top();
        oper.pop();
        den.push(__calculate_operator(x, y, op));
    }

    return den.top();
}

class Fraction
/*
Fraction.
*/
{
   private:
    LL num, den;

   public:
    Fraction() : num(0), den(1) {}
    Fraction(const Fraction& other) { *this = other; }
    Fraction(LL _num, LL _den) : num(_num), den(_den) {
        if (den == 0) throw("FRACTION:Divided by zero.");

        if (den < 0) {
            den = -den;
            num = -num;
        }

        LL d = gcd(abs(num), abs(den));
        num /= d;
        den /= d;
    }

    LL numerator() { return num; }

    LL denominator() { return den; }

    Fraction operator=(int x) {
        num = x;
        den = 1;
    }

    Fraction operator-() { return Fraction(-num, den); }

    Fraction operator+(Fraction x) {
        return Fraction(num * x.den + den * x.num, den * x.den);
    }

    Fraction operator+=(Fraction x) {
        *this = *this + x;
        return *this;
    }

    Fraction operator-(Fraction x) {
        return Fraction(num * x.den - den * x.num, den * x.den);
    }

    Fraction operator-=(Fraction x) {
        *this = *this - x;
        return *this;
    }

    Fraction operator*(Fraction x) {
        return Fraction(num * x.num, den * x.den);
    }

    Fraction operator*=(Fraction x) {
        *this = *this * x;
        return *this;
    }

    Fraction operator/(Fraction x) {
        return Fraction(num * x.den, den * x.num);
    }

    Fraction operator/=(Fraction x) {
        *this = *this / x;
        return *this;
    }

    Fraction inv() { return Fraction(den, num); }

    Fraction operator+(int x) { return Fraction(num + x * den, den); }

    Fraction operator+=(int x) {
        *this = *this + x;
        return *this;
    }

    Fraction operator-(int x) { return Fraction(num - x * den, den); }

    Fraction operator-=(int x) {
        *this = *this - x;
        return *this;
    }

    Fraction operator*(int x) { return Fraction(x * num, den); }

    Fraction operator*=(int x) {
        *this = *this * x;
        return *this;
    }

    Fraction operator/(int x) { return Fraction(num, x * den); }

    Fraction operator/=(int x) {
        *this = *this / x;
        return *this;
    }

    int operator%(int x) { return num * mathmetic::inv(den, x) % x; }

    friend Fraction operator+(int x, Fraction f) {
        return Fraction(x * f.den + f.num, f.den);
    }

    friend Fraction operator-(int x, Fraction f) {
        return Fraction(x * f.den - f.num, f.den);
    }

    friend Fraction operator*(int x, Fraction f) {
        return Fraction(x * f.num, x * f.den);
    }

    friend Fraction operator/(int x, Fraction f) {
        return Fraction(x * f.den, x * f.num);
    }
};
}  // namespace mathmetic
}  // namespace Octinc

#endif
