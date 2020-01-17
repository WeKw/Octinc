/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_BIG_INTEGER_HPP
#  define OCTINC_BIG_INTEGER_HPP
#  ifndef STD_BITS_STDCPP_H
#    define STD_BITS_STDCPP_H
#    include <bits/stdc++.h>
#  endif

namespace Octinc {
class BigInteger {
   private:
    static const int BASE = 100000000;
    static const int WIDTH = 8;
    bool sign;
    std::size_t length;
    std::vector<int> num;

    void fix();
    void resize();

   public:
    BigInteger(int);
    BigInteger(long long);
    BigInteger(const char*);
    BigInteger(const BigInteger&);

    const BigInteger& operator=(int);
    const BigInteger& operator=(long long);
    const BigInteger& operator=(const char*);
    const BigInteger& operator=(const BigInteger&);

    std::size_t size() const;
    BigInteger e(std::size_t) const;
    BigInteger abs() const;
    std::string to_string();
    int to_int() const;
    long long to_long() const;

    const BigInteger& operator+() const;
    friend BigInteger operator+(const BigInteger&, const BigInteger&);
    const BigInteger& operator+=(const BigInteger&);
    const BigInteger& operator++();
    BigInteger operator++(int);

    BigInteger operator-() const;
    friend BigInteger operator-(const BigInteger&, const BigInteger&);
    const BigInteger& operator-=(const BigInteger&);
    const BigInteger& operator--();
    BigInteger operator--(int);

    friend BigInteger operator*(const BigInteger&, const BigInteger&);
    const BigInteger& operator*=(const BigInteger&);

    friend BigInteger operator/(const BigInteger&, const BigInteger&);
    const BigInteger& operator/=(const BigInteger&);

    friend BigInteger operator%(const BigInteger&, const BigInteger&);
    int operator%(int);
    const BigInteger& operator%=(const BigInteger&);

    friend bool operator<(const BigInteger&, const BigInteger&);
    friend bool operator>(const BigInteger&, const BigInteger&);
    friend bool operator<=(const BigInteger&, const BigInteger&);
    friend bool operator>=(const BigInteger&, const BigInteger&);
    friend bool operator==(const BigInteger&, const BigInteger&);
    friend bool operator!=(const BigInteger&, const BigInteger&);

    friend bool operator||(const BigInteger&, const BigInteger&);
    friend bool operator&&(const BigInteger&, const BigInteger&);
    bool operator!();

    friend BigInteger operator&(BigInteger, BigInteger);
    const BigInteger& operator&=(const BigInteger&);

    friend BigInteger operator|(BigInteger, BigInteger);
    const BigInteger& operator|=(const BigInteger&);

    friend BigInteger operator^(BigInteger, BigInteger);
    const BigInteger& operator^=(const BigInteger&);

    friend BigInteger operator<<(const BigInteger&, const BigInteger&);
    const BigInteger& operator<<=(const BigInteger&);

    friend BigInteger operator>>(const BigInteger&, const BigInteger&);
    const BigInteger& operator>>=(const BigInteger&);

    friend BigInteger operator~(BigInteger n);

    friend std::ostream& operator<<(std::ostream&, const BigInteger&);
    friend std::istream& operator>>(std::istream&, BigInteger&);

    static BigInteger Gcd(BigInteger, BigInteger);
    static BigInteger Exgcd(BigInteger, BigInteger, BigInteger&, BigInteger&);
    static BigInteger Pow(BigInteger, BigInteger);
    static BigInteger ModPow(BigInteger, BigInteger, BigInteger);
    static BigInteger Inv(BigInteger, BigInteger);
};

void BigInteger::fix() {
    while (num.back() == 0 && num.size() != 1) num.pop_back();
}

void BigInteger::resize() {
    fix();
    int tmp = num.back();

    if (tmp == 0)
        length = 1;
    else {
        length = (num.size() - 1) * 8;

        while (tmp > 0) {
            ++length;
            tmp /= 10;
        }
    }
}

BigInteger::BigInteger(int n = 0) { *this = n; }

BigInteger::BigInteger(long long n) { *this = n; }

BigInteger::BigInteger(const char* n) { *this = n; }

BigInteger::BigInteger(const BigInteger& n) { *this = n; }

const BigInteger& BigInteger::operator=(int n) {
    *this = (long long)n;
    return *this;
}

const BigInteger& BigInteger::operator=(long long n) {
    num.clear();

    if (n == 0) num.push_back(0);

    if (n >= 0)
        sign = true;
    else if (n == LONG_LONG_MIN) {
        *this = "-9223372036854775808";
        return *this;
    } else if (n < 0) {
        sign = false;
        n = -n;
    }

    while (n != 0) {
        num.push_back(n % BASE);
        n /= BASE;
    }

    resize();

    return *this;
}

const BigInteger& BigInteger::operator=(const char* n) {
    int len = strlen(n);
    int tmp = 0, ten = 1, stop = 0;

    num.clear();
    sign = (n[0] != '-');

    if (!sign) stop = 1;

    for (int i = len; i > stop; --i) {
        tmp += (n[i - 1] - '0') * ten;
        ten *= 10;

        if ((len - i) % 8 == 7) {
            num.push_back(tmp);
            tmp = 0;
            ten = 1;
        }
    }

    if ((len - stop) % WIDTH != 0) num.push_back(tmp);

    resize();

    return *this;
}

const BigInteger& BigInteger::operator=(const BigInteger& n) {
    num = n.num;
    sign = n.sign;
    length = n.length;
    return *this;
}

std::size_t BigInteger::size() const { return length; }

BigInteger BigInteger::e(std::size_t n) const {
    int tmp = n % 8;
    BigInteger ans;
    ans.length = n + 1;
    n /= 8;

    while (ans.num.size() <= n) ans.num.push_back(0);

    ans.num[n] = 1;

    while (tmp--) ans.num[n] *= 10;

    return ans * (*this);
}

std::string BigInteger::to_string() {
    std::string str = "";
    std::vector<int> a = num;

    if (!sign) str = "-";

    for (int i = 0; i < a.size(); ++i) {
        if (a[i] == 0) str += "0";

        while (a[i]) {
            str += (a[i] % 10) + '0';
            a[i] /= 10;
        }
    }

    std::reverse(str.begin(), str.end());

    return str;
}

int BigInteger::to_int() const {
    int res = 0, p = 1;

    for (int i = 0; i < num.size(); ++i) {
        res += num[i] * p;
        p *= BASE;
    }

    return sign ? res : -res;
}

long long BigInteger::to_long() const {
    long long res = 0, p = 2;

    for (int i = 0; i < num.size(); ++i) {
        res += num[i] * p;
        p *= BASE;
    }

    res /= 2;

    return sign ? res : -res;
}

BigInteger BigInteger::abs() const {
    BigInteger ans(*this);
    ans.sign = true;
    return ans;
}

const BigInteger& BigInteger::operator+() const { return *this; }

BigInteger operator+(const BigInteger& a, const BigInteger& b) {
    if (!b.sign) return a - (-b);
    if (!a.sign) return b - (-a);

    BigInteger ans;
    int carry = 0;
    int aa, bb;
    std::size_t lena = a.num.size(), lenb = b.num.size(),
                len = std::max(lena, lenb);

    ans.num.clear();

    for (std::size_t i = 0; i < len; ++i) {
        if (lena <= i)
            aa = 0;
        else
            aa = a.num[i];

        if (lenb <= i)
            bb = 0;
        else
            bb = b.num[i];

        ans.num.push_back((aa + bb + carry) % BigInteger::BASE);
        carry = (aa + bb + carry) / BigInteger::BASE;
    }

    if (carry > 0) ans.num.push_back(carry);

    ans.resize();

    return ans;
}

const BigInteger& BigInteger::operator+=(const BigInteger& n) {
    *this = *this + n;
    return *this;
}

const BigInteger& BigInteger::operator++() {
    *this = *this + 1;
    return *this;
}

BigInteger BigInteger::operator++(int) {
    BigInteger ans(*this);
    *this = *this + 1;
    return ans;
}

BigInteger BigInteger::operator-() const {
    BigInteger ans(*this);

    if (ans != 0) ans.sign = !ans.sign;

    return ans;
}

BigInteger operator-(const BigInteger& a, const BigInteger& b) {
    if (!b.sign) return a + (-b);
    if (!a.sign) return -((-a) + b);
    if (a < b) return -(b - a);

    BigInteger ans;
    int carry = 0;
    int aa, bb;
    std::size_t lena = a.num.size(), lenb = b.num.size(),
                len = std::max(lena, lenb);

    ans.num.clear();

    for (std::size_t i = 0; i < len; ++i) {
        aa = a.num[i];

        if (i >= lenb)
            bb = 0;
        else
            bb = b.num[i];

        ans.num.push_back((aa - bb - carry + BigInteger::BASE) %
                          BigInteger::BASE);

        if (aa < bb + carry)
            carry = 1;
        else
            carry = 0;
    }

    ans.resize();
    return ans;
}

const BigInteger& BigInteger::operator-=(const BigInteger& n) {
    *this = *this - n;
    return *this;
}

const BigInteger& BigInteger::operator--() {
    *this = *this - 1;
    return *this;
}

BigInteger BigInteger::operator--(int) {
    BigInteger ans(*this);
    *this = *this - 1;
    return ans;
}

BigInteger operator*(const BigInteger& a, const BigInteger& b) {
    std::size_t lena = a.num.size(), lenb = b.num.size();
    std::vector<long long> ansLL;

    for (std::size_t i = 0; i < lena; ++i)
        for (std::size_t j = 0; j < lenb; ++j)
            if (i + j >= ansLL.size())
                ansLL.push_back((long long)a.num[i] * (long long)b.num[j]);
            else
                ansLL[i + j] += (long long)a.num[i] * (long long)b.num[j];

    while (ansLL.back() == 0 && ansLL.size() != 1) ansLL.pop_back();

    std::size_t len = ansLL.size();
    long long carry = 0, tmp;
    BigInteger ans;

    ans.sign = (ansLL.size() == 1 && ansLL[0] == 0) || (a.sign == b.sign);
    ans.num.clear();

    for (std::size_t i = 0; i < len; ++i) {
        tmp = ansLL[i];
        ans.num.push_back((tmp + carry) % BigInteger::BASE);
        carry = (tmp + carry) / BigInteger::BASE;
    }

    if (carry > 0) ans.num.push_back(carry);

    ans.resize();

    return ans;
}

const BigInteger& BigInteger::operator*=(const BigInteger& n) {
    *this = *this * n;
    return *this;
}

BigInteger operator/(const BigInteger& a, const BigInteger& b) {
    BigInteger aa(a.abs()), bb(b.abs());

    if (aa < bb) return 0;

    char* str = new char[aa.size() + 1];
    memset(str, 0, sizeof(char) * (aa.size() + 1));
    BigInteger tmp;
    int lena = aa.length, lenb = bb.length;

    for (int i = 0; i <= lena - lenb; ++i) {
        tmp = bb.e(lena - lenb - i);

        while (aa >= tmp) {
            ++str[i];
            aa = aa - tmp;
        }

        str[i] += '0';
    }

    BigInteger ans(str);
    delete[] str;
    ans.sign = (ans == 0 || a.sign == b.sign);

    return ans;
}

const BigInteger& BigInteger::operator/=(const BigInteger& n) {
    *this = *this / n;
    return *this;
}

BigInteger operator%(const BigInteger& a, const BigInteger& b) {
    return a - a / b * b;
}

const BigInteger& BigInteger::operator%=(const BigInteger& n) {
    *this = *this - *this / n * n;
    return *this;
}

int BigInteger::operator%(int m) {
    int s = 1, res = 0;

    for (int i = 0; i < num.size(); ++i)
        res += num[i] % m, s = 1LL * s * 10 % m;

    return s;
}

bool operator<(const BigInteger& a, const BigInteger& b) {
    if (a.sign && !b.sign)
        return false;
    else if (!a.sign && b.sign)
        return true;
    else if (a.sign && b.sign) {
        if (a.length < b.length)
            return true;
        else if (a.length > b.length)
            return false;
        else {
            std::size_t lena = a.num.size();

            for (int i = lena - 1; ~i; --i)
                if (a.num[i] < b.num[i])
                    return true;
                else if (a.num[i] > b.num[i])
                    return false;
        }
    } else
        return -b > -a;
}

bool operator<=(const BigInteger& a, const BigInteger& b) { return !(b < a); }

bool operator>(const BigInteger& a, const BigInteger& b) { return b < a; }

bool operator>=(const BigInteger& a, const BigInteger& b) { return !(a < b); }

bool operator==(const BigInteger& a, const BigInteger& b) {
    return !(a < b) && !(b < a);
}

bool operator!=(const BigInteger& a, const BigInteger& b) {
    return (a < b) || (b < a);
}

bool operator||(const BigInteger& a, const BigInteger& b) {
    return a != 0 || b != 0;
}

bool operator&&(const BigInteger& a, const BigInteger& b) {
    return a != 0 && b != 0;
}

bool BigInteger::operator!() { return *this == 0; }

BigInteger operator&(BigInteger a, BigInteger b) {
    if (!a.sign && b.sign) return ((~a) + 1) & b;
    if (a.sign && !b.sign) return a & ((~b) + 1);
    if (!a.sign && !b.sign) return ((~a) + 1) & ((~b) + 1);

    std::vector<int> faca, facb;

    while (a != 0) {
        faca.push_back(a.num[0] % 2);
        a /= 2;
    }

    while (b != 0) {
        facb.push_back(b.num[0] % 2);
        b /= 2;
    }

    std::size_t lena = faca.size(), lenb = facb.size(),
                len = std::min(lena, lenb);
    BigInteger ans = 0, p = 1;

    for (std::size_t i = 0; i < len; ++i) {
        ans += (faca[i] && facb[i]) * p;
        p *= 2;
    }

    ans.resize();

    return ans;
}

const BigInteger& BigInteger::operator&=(const BigInteger& n) {
    *this = *this & n;
    return *this;
}

BigInteger operator|(BigInteger a, BigInteger b) {
    if (!a.sign && b.sign) return ((~a) + 1) | b;
    if (a.sign && !b.sign) return a | ((~b) + 1);
    if (!a.sign && !b.sign) return ((~a) + 1) | ((~b) + 1);

    std::vector<int> faca, facb;

    while (a != 0) {
        faca.push_back(a.num[0] % 2);
        a /= 2;
    }

    while (b != 0) {
        facb.push_back(b.num[0] % 2);
        b /= 2;
    }

    std::size_t lena = faca.size(), lenb = facb.size(),
                len = std::min(lena, lenb);

    BigInteger ans = 0, p = 1;

    for (std::size_t i = 0; i < len; ++i) {
        ans += (faca[i] || facb[i]) * p;
        p *= 2;
    }

    if (lena > lenb)
        for (std::size_t i = len; i < lena; ++i) {
            ans += faca[i] * p;
            p *= 2;
        }
    else
        for (std::size_t i = len; i < lenb; ++i) {
            ans += facb[i] * p;
            p *= 2;
        }

    ans.resize();

    return ans;
}

const BigInteger& BigInteger::operator|=(const BigInteger& n) {
    *this = *this | n;
    return *this;
}

BigInteger operator^(BigInteger a, BigInteger b) {
    if (!a.sign && b.sign) return ((~a) + 1) ^ b;
    if (a.sign && !b.sign) return a ^ ((~b) + 1);
    if (!a.sign && !b.sign) return ((~a) + 1) ^ ((~b) + 1);

    std::vector<int> faca, facb;

    while (a != 0) {
        faca.push_back(a.num[0] % 2);
        a /= 2;
    }

    while (b != 0) {
        facb.push_back(b.num[0] % 2);
        b /= 2;
    }

    std::size_t lena = faca.size(), lenb = facb.size(),
                len = std::min(lena, lenb);
    BigInteger ans = 0, p = 1;

    for (std::size_t i = 0; i < len; ++i) {
        ans += (faca[i] != facb[i]) * p;
        p *= 2;
    }

    if (lena > lenb)
        for (std::size_t i = len; i < lena; ++i) {
            ans += faca[i] * p;
            p *= 2;
        }
    else
        for (std::size_t i = len; i < lenb; ++i) {
            ans += facb[i] * p;
            p *= 2;
        }

    ans.resize();

    return ans;
}

const BigInteger& BigInteger::operator^=(const BigInteger& n) {
    *this = *this ^ n;
    return *this;
}

BigInteger Pow(BigInteger, BigInteger);

BigInteger operator<<(const BigInteger& a, const BigInteger& b) {
    BigInteger ans = a * BigInteger::Pow(2, b);
    ans.resize();
    return ans;
}

const BigInteger& BigInteger::operator<<=(const BigInteger& n) {
    *this = *this << n;
    return *this;
}

BigInteger operator>>(const BigInteger& a, const BigInteger& b) {
    BigInteger ans = a / BigInteger::Pow(2, b);
    ans.resize();
    return ans;
}

const BigInteger& BigInteger::operator>>=(const BigInteger& n) {
    *this = *this >> n;
    return *this;
}

BigInteger operator~(BigInteger n) {
    BigInteger ans = -n - 1;
    ans.resize();
    return ans;
}

std::ostream& operator<<(std::ostream& os, const BigInteger& n) {
    std::size_t len = n.num.size();

    if (!n.sign) os << "-";

    os << n.num.back();

    for (int i = len - 2; ~i; --i)
        os << std::setw(BigInteger::WIDTH) << std::setfill('0') << n.num[i];

    return os;
}

std::istream& operator>>(std::istream& is, BigInteger& n) {
    std::string str;
    is >> str;
    std::size_t len = str.length();
    std::size_t i, start = 0;

    if (str[0] == '-') start = 1;

    if (str[start] == '\0') return is;

    for (i = start; i < len; ++i)
        if (str[i] < '0' || str[i] > '9') return is;

    n = str.c_str();

    return is;
}

BigInteger BigInteger::Gcd(BigInteger a, BigInteger b) {
    BigInteger stemp;

    if (a < b) {
        stemp = a;
        a = b;
        b = stemp;
    }

    if (b == 0)
        return a;
    else
        return Gcd(b, a % b);
}

BigInteger BigInteger::Pow(BigInteger a, BigInteger b) {
    BigInteger aa = a, t = 0, k = 2;
    while (b != 0) {
        if ((b % k) != 0) t = t * aa;

        aa = aa * aa;
        b = b / k;
    }

    return t;
}

BigInteger BigInteger::ModPow(BigInteger a, BigInteger b, BigInteger p) {
    BigInteger aa = a, t = 0, k = 2;

    while (b != 0) {
        if ((b % k) != 0) t = (t % p) * (aa % p) % p;

        aa = (aa % p) * (aa % p) % p;
        b = b / k;
    }

    return t % p;
}

BigInteger BigInteger::Exgcd(BigInteger a, BigInteger b, BigInteger& x,
                             BigInteger& y) {
    BigInteger d = a;

    if (b != 0) {
        d = Exgcd(b, a % b, y, x);
        y = y - (a / b) * x;
    } else {
        x = 1;
        y = 0;
    }
    return d;
}

BigInteger BigInteger::Inv(BigInteger a, BigInteger m)
/*
Multiplication inverse.
*/
{
    BigInteger x, y;
    Exgcd(a, m, x, y);

    if (x < 0) {
        x = -x;
        x = m - x;
    }

    return (m + x % m) % m;
}
}  // namespace Octinc

#endif
