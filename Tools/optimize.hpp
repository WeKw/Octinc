/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_OPTIMIZE_HPP
#  define OCTINC_OPTIMIZE_HPP
#  pragma GCC optimize(2)
#  pragma GCC optimize(3)
#  pragma GCC optimize("Ofast")
#  pragma GCC optimize("inline")
#  pragma GCC optimize("-fgcse")
#  pragma GCC optimize("-fgcse-lm")
#  pragma GCC optimize("-fipa-sra")
#  pragma GCC optimize("-ftree-pre")
#  pragma GCC optimize("-ftree-vrp")
#  pragma GCC optimize("-fpeephole2")
#  pragma GCC optimize("-ffast-math")
#  pragma GCC optimize("-fsched-spec")
#  pragma GCC optimize("unroll-loops")
#  pragma GCC optimize("-falign-jumps")
#  pragma GCC optimize("-falign-loops")
#  pragma GCC optimize("-falign-labels")
#  pragma GCC optimize("-fdevirtualize")
#  pragma GCC optimize("-fcaller-saves")
#  pragma GCC optimize("-fcrossjumping")
#  pragma GCC optimize("-fthread-jumps")
#  pragma GCC optimize("-funroll-loops")
#  pragma GCC optimize("-fwhole-program")
#  pragma GCC optimize("-freorder-blocks")
#  pragma GCC optimize("-fschedule-insns")
#  pragma GCC optimize("inline-functions")
#  pragma GCC optimize("-ftree-tail-merge")
#  pragma GCC optimize("-fschedule-insns2")
#  pragma GCC optimize("-fstrict-aliasing")
#  pragma GCC optimize("-fstrict-overflow")
#  pragma GCC optimize("-falign-functions")
#  pragma GCC optimize("-fcse-skip-blocks")
#  pragma GCC optimize("-fcse-follow-jumps")
#  pragma GCC optimize("-fsched-interblock")
#  pragma GCC optimize("-fpartial-inlining")
#  pragma GCC optimize("no-stack-protector")
#  pragma GCC optimize("-freorder-functions")
#  pragma GCC optimize("-findirect-inlining")
#  pragma GCC optimize("-fhoist-adjacent-loads")
#  pragma GCC optimize("-frerun-cse-after-loop")
#  pragma GCC optimize("inline-small-functions")
#  pragma GCC optimize("-finline-small-functions")
#  pragma GCC optimize("-ftree-switch-conversion")
#  pragma GCC optimize("-foptimize-sibling-calls")
#  pragma GCC optimize("-fexpensive-optimizations")
#  pragma GCC optimize("-funsafe-loop-optimizations")
#  pragma GCC optimize("inline-functions-called-once")
#  pragma GCC optimize("-fdelete-null-pointer-checks")
#  define Opt2 __attribute__((optimize("-O2")))
#  define Opt3 __attribute__((optimize("-O3")))
#  define Inline __attribute__((optimize("inline Oinline")))
#  define Ofast __attribute__((optimize("Ofast")))
#  ifndef STD_BITS_STDCPP_H
#    define STD_BITS_STDCPP_H
#    include <bits/stdc++.h>
#  endif

namespace Octinc {
struct optf_control {
    int ct, val;

    optf_control(int Ct, int Val = -1) : ct(Ct), val(Val) {}

    inline optf_control operator()(int Val) { return optf_control(ct, Val); }
} ofendl(0), ofprs(1), ofsetprecision(2);

template <const size_t IOSIZE>
/*
IOSIZE is the maximum number of characters in files.
*/
class optimized_fstream {
   private:
    char in[IOSIZE], *p, *pp, out[IOSIZE], *q, *qq, ch[20], *t, b, K, prs;

   public:
    optimized_fstream()
        : p(in), pp(in), q(out), qq(out + IOSIZE), t(ch), b(1), K(6) {}
    ~optimized_fstream() { fwrite(out, 1, q - out, stdout); }

    inline void flush() { fwrite(out, 1, q - out, stdout); }

    inline char getch() {
        return p == pp && (pp = (p = in) + fread(in, 1, IOSIZE, stdin), p == pp)
               ? b = 0,
                 EOF : *p++;
    }

    inline void putch(char x) {
        q == qq && (fwrite(out, 1, q - out, stdout), q = out), *q++ = x;
    }

    inline void puts(const char str[]) {
        fwrite(out, 1, q - out, stdout), fwrite(str, 1, strlen(str), stdout),
            q = out;
    }

    inline void getline(std::string& s) {
        s = "";
        for (register char ch; (ch = getch()) != '\n' && b;) s += ch;
    }

#define indef(T)                                                               \
    inline optimized_fstream& operator>>(T& x) {                               \
        x = 0;                                                                 \
        register char f = 0, ch;                                               \
        while (!isdigit(ch = getch()) && b) f |= ch == '-';                    \
        while (isdigit(ch)) x = (x << 1) + (x << 3) + (ch ^ 48), ch = getch(); \
        return x = f ? -x : x, *this;                                          \
    }

    indef(int) indef(long long)
#undef indef

        inline optimized_fstream&
        operator>>(char& ch) {
        return ch = getch(), *this;
    }

    inline optimized_fstream& operator>>(std::string& s) {
        s = "";
        register char ch;
        while (isspace(ch = getch()) && b)
            ;

        while (!isspace(ch) && b) s += ch, ch = getch();

        return *this;
    }

    inline optimized_fstream& operator>>(double& x) {
        x = 0;

        register char f = 0, ch;
        double d = 0.1;

        while (!isdigit(ch = getch()) && b) f |= (ch == '-');
        while (isdigit(ch)) x = x * 10 + (ch ^ 48), ch = getch();

        if (ch == '.')
            while (isdigit(ch = getch())) x += d * (ch ^ 48), d *= 0.1;

        return x = f ? -x : x, *this;
    }

#define outdef(_T)                                            \
    inline optimized_fstream& operator<<(_T x) {              \
        !x && (putch('0'), 0), x < 0 && (putch('-'), x = -x); \
        while (x) *t++ = x % 10 + 48, x /= 10;                \
        while (t != ch) *q++ = *--t;                          \
        return *this;                                         \
    }

    outdef(int) outdef(long long)
#undef outdef

        inline optimized_fstream&
        operator<<(char ch) {
        return putch(ch), *this;
    }

    inline optimized_fstream& operator<<(const char str[]) {
        return puts(str), *this;
    }

    inline optimized_fstream& operator<<(const std::string& s) {
        return puts(s.c_str()), *this;
    }

    inline optimized_fstream& operator<<(double x) {
        register int k = 0;
        this->operator<<(int(x));
        putch('.');
        x -= int(x);
        prs && (x += 5 * pow(10, -K - 1));

        while (k < K) putch(int(x *= 10) ^ 48), x -= int(x), ++k;

        return *this;
    }

    inline optimized_fstream& operator<<(const optf_control& cl) {
        switch (cl.ct) {
            case 0:
                putch('\n');
                break;
            case 1:
                prs = cl.val;
                break;
            case 2:
                K = cl.val;
                break;
        }
    }

    inline operator bool() { return b; }
};
}  // namespace Octinc

#endif
