/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_BIG_DECIMAL_HPP
#  define OCTINC_BIG_DECIMAL_HPP
#  ifndef STD_BITS_STDCPP_H
#    define STD_BITS_STDCPP_H
#    include <bits/stdc++.h>
#  endif

namespace Octinc {
class BigDecimal {
   public:
    BigDecimal();
    BigDecimal(const std::string &);
    BigDecimal(const char *);
    BigDecimal(int);
    BigDecimal(long long);
    BigDecimal(double);

    bool is_zero() const;

    std::string to_string(int) const;
    double to_double() const;

    friend BigDecimal operator+(const BigDecimal &, const BigDecimal &);
    friend BigDecimal operator+(const BigDecimal &, int);
    friend BigDecimal operator+(int, const BigDecimal &);
    friend BigDecimal operator+(const BigDecimal &, long long);
    friend BigDecimal operator+(long long, const BigDecimal &);
    friend BigDecimal operator+(const BigDecimal &, double);
    friend BigDecimal operator+(double, const BigDecimal &);

    friend BigDecimal operator-(const BigDecimal &, const BigDecimal &);
    friend BigDecimal operator-(const BigDecimal &, int);
    friend BigDecimal operator-(int, const BigDecimal &);
    friend BigDecimal operator-(const BigDecimal &, long long);
    friend BigDecimal operator-(long long, const BigDecimal &);
    friend BigDecimal operator-(const BigDecimal &, double);
    friend BigDecimal operator-(double, const BigDecimal &);

    friend BigDecimal operator*(const BigDecimal &, int);
    friend BigDecimal operator*(int, const BigDecimal &);

    friend BigDecimal operator/(const BigDecimal &, int);

    friend bool operator<(const BigDecimal &, const BigDecimal &);
    friend bool operator>(const BigDecimal &, const BigDecimal &);
    friend bool operator<=(const BigDecimal &, const BigDecimal &);
    friend bool operator>=(const BigDecimal &, const BigDecimal &);
    friend bool operator==(const BigDecimal &, const BigDecimal &);
    friend bool operator!=(const BigDecimal &, const BigDecimal &);

    BigDecimal &operator=(const std::string &);
    BigDecimal &operator=(const char *);
    BigDecimal &operator=(int);
    BigDecimal &operator=(long long);
    BigDecimal &operator=(double);

    BigDecimal &operator+=(int);
    BigDecimal &operator+=(long long);
    BigDecimal &operator+=(double);
    BigDecimal &operator+=(const BigDecimal &);

    BigDecimal &operator-=(int);
    BigDecimal &operator-=(long long);
    BigDecimal &operator-=(double);
    BigDecimal &operator-=(const BigDecimal &);

    BigDecimal &operator*=(int);

    BigDecimal &operator/=(int);

    friend BigDecimal operator-(const BigDecimal &);

    friend BigDecimal operator*(const BigDecimal &, double);
    friend BigDecimal operator*(double, const BigDecimal &);
    friend BigDecimal operator/(const BigDecimal &, double);
    BigDecimal &operator*=(double);
    BigDecimal &operator/=(double);

    friend std::istream &operator>>(std::istream &, BigDecimal &);
    friend std::ostream &operator<<(std::ostream &, const BigDecimal &);

   private:
    static const int PREC = 3000;
    static const int WIDTH = PREC / 9 + 1;
    static const int BASE = 1000000000;

    static void append_to_string(std::string &, long long);

    bool sign;
    long long integer;
    int data[WIDTH];

    void init_zero();
    void init(const char *);
};

BigDecimal::BigDecimal() { this->init_zero(); }

BigDecimal::BigDecimal(const char *s) { *this = s; }

BigDecimal::BigDecimal(const std::string &s) { *this = s; }

BigDecimal::BigDecimal(int x) { *this = x; }

BigDecimal::BigDecimal(long long x) { *this = x; }

BigDecimal::BigDecimal(double x) { *this = x; }

void BigDecimal::init_zero() {
    sign = false;
    integer = 0;
    memset(data, 0, WIDTH * sizeof(int));
}

bool BigDecimal::is_zero() const {
    if (integer) return false;

    for (int i = 0; i < WIDTH; ++i)
        if (data[i]) return false;

    return true;
}

void BigDecimal::init(const char *s) {
    this->init_zero();

    sign = false;
    integer = 0;

    while (*s != 0) {
        if (*s == '-') {
            sign = true;
            ++s;
            break;
        } else if (*s >= 48 && *s <= 57)
            break;

        ++s;
    }

    while (*s >= 48 && *s <= 57) {
        integer = integer * 10 + *s - 48;
        ++s;
    }

    if (*s == '.') {
        int pos = 0;
        int x = BASE / 10;

        ++s;
        while (pos < WIDTH && *s >= 48 && *s <= 57) {
            data[pos] += (*s - 48) * x;
            ++s;
            x /= 10;

            if (x == 0) {
                ++pos;
                x = BASE / 10;
            }
        }
    }
}

BigDecimal &BigDecimal::operator=(const std::string &s) {
    this->init(s.c_str());
    return *this;
}

BigDecimal &BigDecimal::operator=(const char *s) {
    this->init(s);
    return *this;
}

BigDecimal &BigDecimal::operator=(int x) {
    this->init_zero();

    if (x < 0) {
        sign = true;
        x = -x;
    }

    integer = x;

    return *this;
}

BigDecimal &BigDecimal::operator=(long long x) {
    this->init_zero();

    if (x < 0) {
        sign = true;
        x = -x;
    }

    integer = x;

    return *this;
}

BigDecimal &BigDecimal::operator=(double x) {
    this->init_zero();

    if (x < 0) {
        sign = true;
        x = -x;
    }

    integer = (long long)x;
    x -= integer;

    for (int i = 0; i < WIDTH; ++i) {
        x *= BASE;

        if (x < 0) x = 0;

        data[i] = (int)x;
        x -= data[i];
    }

    return *this;
}

void BigDecimal::append_to_string(std::string &s, long long x) {
    if (x == 0) {
        s.append(1, 48);
        return;
    }

    char _[30];
    int cnt = 0;

    while (x) {
        _[cnt++] = x % 10;
        x /= 10;
    }

    while (cnt--) s.append(1, _[cnt] + 48);
}

std::string BigDecimal::to_string(int p) const {
    std::string ret;

    if (sign && !this->is_zero()) {
        ret = "-";
    }

    append_to_string(ret, this->integer);

    ret.append(1, '.');

    for (int i = 0; i < WIDTH; ++i) {
        int x = BASE / 10;
        int tmp = data[i];

        while (x) {
            ret.append(1, 48 + tmp / x);
            tmp %= x;
            x /= 10;

            if (--p == 0) break;
        }

        if (p == 0) break;
    }

    if (p > 0) ret.append(p, '0');

    return ret;
}

double BigDecimal::to_double() const {
    double ret = integer;

    double k = 1.0;

    for (int i = 0; i < WIDTH; ++i) {
        k /= BASE;
        ret += k * data[i];
    }

    if (sign) ret = -ret;

    return ret;
}

bool operator<(const BigDecimal &a, const BigDecimal &b) {
    if (a.sign != b.sign)
        return a.sign && (!a.is_zero() || !b.is_zero());

    else if (!a.sign) {
        if (a.integer != b.integer) return a.integer < b.integer;

        for (int i = 0; i < BigDecimal::WIDTH; ++i)
            if (a.data[i] != b.data[i]) return a.data[i] < b.data[i];

        return false;
    } else {
        if (a.integer != b.integer) return a.integer > b.integer;

        for (int i = 0; i < BigDecimal::WIDTH; ++i)
            if (a.data[i] != b.data[i]) return a.data[i] > b.data[i];

        return false;
    }
}

bool operator>(const BigDecimal &a, const BigDecimal &b) {
    if (a.sign != b.sign)
        return !a.sign && (!a.is_zero() || !b.is_zero());
    else if (!a.sign) {
        if (a.integer != b.integer) return a.integer > b.integer;

        for (int i = 0; i < BigDecimal::WIDTH; ++i)
            if (a.data[i] != b.data[i]) return a.data[i] > b.data[i];

        return false;
    } else {
        if (a.integer != b.integer) return a.integer < b.integer;

        for (int i = 0; i < BigDecimal::WIDTH; ++i)
            if (a.data[i] != b.data[i]) return a.data[i] < b.data[i];

        return false;
    }
}

bool operator<=(const BigDecimal &a, const BigDecimal &b) {
    if (a.sign != b.sign)
        return a.sign || (a.is_zero() && b.is_zero());
    else if (!a.sign) {
        if (a.integer != b.integer) return a.integer < b.integer;

        for (int i = 0; i < BigDecimal::WIDTH; ++i)
            if (a.data[i] != b.data[i]) return a.data[i] < b.data[i];

        return true;
    } else {
        if (a.integer != b.integer) return a.integer > b.integer;

        for (int i = 0; i < BigDecimal::WIDTH; ++i)
            if (a.data[i] != b.data[i]) return a.data[i] > b.data[i];

        return true;
    }
}

bool operator>=(const BigDecimal &a, const BigDecimal &b) {
    if (a.sign != b.sign)
        return !a.sign || (a.is_zero() && b.is_zero());
    else if (!a.sign) {
        if (a.integer != b.integer) return a.integer > b.integer;

        for (int i = 0; i < BigDecimal::WIDTH; ++i)
            if (a.data[i] != b.data[i]) return a.data[i] > b.data[i];

        return true;
    } else {
        if (a.integer != b.integer) return a.integer < b.integer;

        for (int i = 0; i < BigDecimal::WIDTH; ++i)
            if (a.data[i] != b.data[i]) return a.data[i] < b.data[i];

        return true;
    }
}

bool operator==(const BigDecimal &a, const BigDecimal &b) {
    if (a.is_zero() && b.is_zero()) return true;
    if (a.sign != b.sign) return false;
    if (a.integer != b.integer) return false;

    for (int i = 0; i < BigDecimal::WIDTH; ++i)
        if (a.data[i] != b.data[i]) return false;

    return true;
}

bool operator!=(const BigDecimal &a, const BigDecimal &b) { return !(a == b); }

BigDecimal &BigDecimal::operator+=(long long x) {
    if (!sign) {
        if (integer + x >= 0) {
            integer += x;
        } else {
            bool last = false;
            for (int i = WIDTH - 1; i >= 0; --i) {
                if (last || data[i]) {
                    data[i] = BASE - data[i] - last;
                    last = true;
                } else {
                    last = false;
                }
            }
            integer = -x - integer - last;
            sign = true;
        }
    } else {
        if (integer - x >= 0)
            integer -= x;
        else {
            bool last = false;

            for (int i = WIDTH - 1; i >= 0; --i) {
                if (last || data[i]) {
                    data[i] = BASE - data[i] - last;
                    last = true;
                } else
                    last = false;
            }

            integer = x - integer - last;
            sign = false;
        }
    }

    return *this;
}

BigDecimal &BigDecimal::operator+=(int x) { return *this += (long long)x; }

BigDecimal &BigDecimal::operator-=(int x) { return *this += (long long)-x; }

BigDecimal &BigDecimal::operator-=(long long x) { return *this += -x; }

BigDecimal &BigDecimal::operator/=(int x) {
    if (x < 0) {
        sign ^= 1;
        x = -x;
    }

    int last = integer % x;
    integer /= x;

    for (int i = 0; i < WIDTH; ++i) {
        long long tmp = 1LL * last * BASE + data[i];
        data[i] = tmp / x;
        last = tmp - 1LL * data[i] * x;
    }

    if (sign && integer == 0) {
        int i;

        for (i = 0; i < WIDTH; ++i)
            if (data[i] != 0) break;

        if (i == WIDTH) sign = false;
    }

    return *this;
}

BigDecimal &BigDecimal::operator*=(int x) {
    if (x < 0) {
        sign ^= 1;
        x = -x;
    } else if (x == 0) {
        init_zero();
        return *this;
    }

    int last = 0;

    for (int i = WIDTH - 1; i >= 0; --i) {
        long long tmp = 1LL * data[i] * x + last;
        last = tmp / BASE;
        data[i] = tmp - 1LL * last * BASE;
    }

    integer = integer * x + last;

    return *this;
}

BigDecimal operator-(const BigDecimal &a) {
    BigDecimal ret = a;

    if (!ret.sign && ret.integer == 0) {
        int i;

        for (i = 0; i < BigDecimal::WIDTH; ++i)
            if (ret.data[i] != 0) break;

        if (i < BigDecimal::WIDTH) ret.sign = true;
    } else
        ret.sign ^= 1;

    return ret;
}

BigDecimal operator+(const BigDecimal &a, int x) {
    BigDecimal ret = a;
    return ret += x;
}

BigDecimal operator+(int x, const BigDecimal &a) {
    BigDecimal ret = a;
    return ret += x;
}

BigDecimal operator+(const BigDecimal &a, long long x) {
    BigDecimal ret = a;
    return ret += x;
}

BigDecimal operator+(long long x, const BigDecimal &a) {
    BigDecimal ret = a;
    return ret += x;
}

BigDecimal operator-(const BigDecimal &a, int x) {
    BigDecimal ret = a;
    return ret -= x;
}

BigDecimal operator-(int x, const BigDecimal &a) { return -(a - x); }

BigDecimal operator-(const BigDecimal &a, long long x) {
    BigDecimal ret = a;
    return ret -= x;
}

BigDecimal operator-(long long x, const BigDecimal &a) { return -(a - x); }

BigDecimal operator*(const BigDecimal &a, int x) {
    BigDecimal ret = a;
    return ret *= x;
}

BigDecimal operator*(int x, const BigDecimal &a) {
    BigDecimal ret = a;
    return ret *= x;
}

BigDecimal operator/(const BigDecimal &a, int x) {
    BigDecimal ret = a;
    return ret /= x;
}

BigDecimal operator+(const BigDecimal &a, const BigDecimal &b) {
    if (a.sign == b.sign) {
        BigDecimal ret = a;
        bool last = false;

        for (int i = BigDecimal::WIDTH - 1; i >= 0; --i) {
            ret.data[i] += b.data[i] + last;

            if (ret.data[i] >= BigDecimal::BASE) {
                ret.data[i] -= BigDecimal::BASE;
                last = true;
            } else
                last = false;
        }

        ret.integer += b.integer + last;

        return ret;
    } else if (!a.sign)
        return a - (-b);
    else
        return b - (-a);
}

BigDecimal operator-(const BigDecimal &a, const BigDecimal &b) {
    if (!a.sign && !b.sign) {
        if (a >= b) {
            BigDecimal ret = a;
            bool last = false;
            for (int i = BigDecimal::WIDTH - 1; i >= 0; --i) {
                ret.data[i] -= b.data[i] + last;
                if (ret.data[i] < 0) {
                    ret.data[i] += BigDecimal::BASE;
                    last = true;
                } else
                    last = false;
            }

            ret.integer -= b.integer + last;

            return ret;
        } else {
            BigDecimal ret = b;
            bool last = false;

            for (int i = BigDecimal::WIDTH - 1; i >= 0; --i) {
                ret.data[i] -= a.data[i] + last;
                if (ret.data[i] < 0) {
                    ret.data[i] += BigDecimal::BASE;
                    last = true;
                } else
                    last = false;
            }

            ret.integer -= a.integer + last;
            ret.sign = true;
            return ret;
        }
    } else if (a.sign && b.sign)
        return -b - (-a);
    else if (a.sign)
        return -(-a + b);
    else
        return a + -b;
}

BigDecimal operator+(const BigDecimal &a, double x) {
    return a + BigDecimal(x);
}

BigDecimal operator+(double x, const BigDecimal &a) {
    return BigDecimal(x) + a;
}

BigDecimal operator-(const BigDecimal &a, double x) {
    return a - BigDecimal(x);
}

BigDecimal operator-(double x, const BigDecimal &a) {
    return BigDecimal(x) - a;
}

BigDecimal &BigDecimal::operator+=(double x) {
    *this = *this + BigDecimal(x);
    return *this;
}

BigDecimal &BigDecimal::operator-=(double x) {
    *this = *this - BigDecimal(x);
    return *this;
}

BigDecimal &BigDecimal::operator+=(const BigDecimal &b) {
    *this = *this + b;
    return *this;
}

BigDecimal &BigDecimal::operator-=(const BigDecimal &b) {
    *this = *this - b;
    return *this;
}

std::istream &operator>>(std::istream &is, BigDecimal &x) {
    std::string str;
    is >> str;
    x = str;
    return is;
}

std::ostream &operator<<(std::ostream &os, const BigDecimal &x) {
    os << x.to_string(16);
    return os;
}
}  // namespace Octinc

#endif
