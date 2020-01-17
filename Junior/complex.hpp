/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_COMPLEX__
#define OCTINC_COMPLEX__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <cmath>
#include <cstdlib>

namespace Octinc {
template <typename __Tp>
class complex {
   private:
    __Tp r, i;

   public:
    complex(__Tp x = 0, __Tp y = 0) : r(x), i(y) {}
    complex(const complex& other) : r(other.r), i(other.i) {}

    __Tp& real()
    /*
    Real part.
    */
    {
        return r;
    }

    __Tp& imag()
    /*
    Imaginary part.
    */
    {
        return i;
    }

    __Tp length()
    /*
    Length.
    */
    {
        return sqrt(r * r + i * i);
    }

    __Tp arg()
    /*
    Polar angle.
    */
    {
        return atan2(i, r);
    }

    complex conj()
    /*
    Conjugate.
    */
    {
        return complex(r, -i);
    }

    complex operator-() { return complex(-r, -i); }

    complex operator+=(__Tp b) {
        r += b;
        return *this;
    }

    complex operator+=(complex b) {
        r += b.r;
        i += b.i;
        return *this;
    }

    template <typename __Tp2>
    complex operator+(__Tp2 b) {
        complex c = *this;
        c += b;
        return c;
    }

    friend complex operator+(__Tp a, complex b) { return b + a; }

    complex operator-=(__Tp b) {
        r -= b;
        return *this;
    }

    complex operator-=(complex b) {
        r -= b.r;
        i -= b.i;
        return *this;
    }

    template <typename __Tp2>
    complex operator-(__Tp2 b) {
        complex c = *this;
        c -= b;
        return c;
    }

    friend complex operator-(__Tp a, complex b) { return complex(a) - b; }

    complex operator*=(__Tp b) {
        r *= b, i *= b;
        return *this;
    }

    complex operator*=(complex b) {
        complex c = *this;
        r = b.r * c.r - b.i * c.i;
        i = b.r * c.i + b.i * c.r;
        return *this;
    }

    template <typename __Tp2>
    complex operator*(__Tp2 b) {
        complex c = *this;
        c *= b;
        return c;
    }

    friend complex operator*(__Tp a, complex b) { return b * a; }

    complex operator/=(__Tp b) {
        r /= b, i /= b;
        return *this;
    }

    complex operator/=(complex b) {
        complex c = b.conj();
        *this /= (b * c).r;
        *this *= c;
        return *this;
    }

    template <typename __Tp2>
    complex operator/(__Tp2 b) {
        complex c = *this;
        c /= b;
        return c;
    }

    friend complex operator/(__Tp a, complex b) { return complex(a) / b; }
};

namespace complex_math {
template <typename __Tp>
complex<__Tp> cexp(complex<__Tp> x)
/*
Complex exponential function.
*/
{
    return exp(x.real()) * complex<__Tp>(cos(x.imag()), sin(x.imag()));
}

template <typename __Tp>
complex<__Tp> ccos(complex<__Tp> x)
/*
Complex cosine.
*/
{
    return (cexp(complex<__Tp>((__Tp)(NULL), x)) +
            cexp(complex<__Tp>((__Tp)(NULL), -x))) /
           ((__Tp)2.0);
}

template <typename __Tp>
complex<__Tp> csin(complex<__Tp> x)
/*
Complex sine.
*/
{
    return (cexp(complex<__Tp>((__Tp)(NULL), x)) -
            cexp(complex<__Tp>((__Tp)(NULL), -x))) /
           (complex<__Tp>((__Tp)(NULL), (__Tp)2.0));
}

template <typename __Tp>
complex<__Tp> clog(complex<__Tp> x)
/*
Complex logarithm function.
*/
{
    return complex<__Tp>(log(x.length()), x.arg());
}

template <typename __Tp>
complex<__Tp> polar(__Tp _r, __Tp Theta)
/*
Construct complex by polar coordinates.
*/
{
    return complex<__Tp>(_r * cos(Theta), _r * sin(Theta));
}

template <typename __Tp>
complex<__Tp> cpow(complex<__Tp> x, __Tp p)
/*
Complex power function.
*/
{
    return polar(pow(x.length(), p), x.arg() * p);
}

template <typename __Tp>
complex<__Tp> csqrt(complex<__Tp> x)
/*
Complex square root.
*/
{
    return cpow(x, (__Tp)0.5);
}

template <typename __Tp>
complex<__Tp> carcsin(complex<__Tp> x)
/*
Complex arcsine.
*/
{
    complex<__Tp> j = complex<__Tp>((__Tp)(NULL), (__Tp)1.0);
    return -j * clog(j * x + csqrt((__Tp)(1.0) - x * x));
}

template <typename __Tp>
complex<__Tp> carccos(complex<__Tp> x)
/*
Complex arccosine.
*/
{
    complex<__Tp> j = complex<__Tp>((__Tp)(NULL), (__Tp)1.0);
    return -j * clog(x + j * csqrt((__Tp)(1.0) - x * x));
}

template <typename __Tp>
complex<__Tp> carctan(complex<__Tp> x)
/*
Complex arctangent.
*/
{
    complex<__Tp> j = complex<__Tp>((__Tp)(NULL), (__Tp)1.0);
    return ((__Tp)1.0 / ((__Tp)2.0 * j)) * clog((j - x) * (j + x));
}
}  // namespace complex_math
}  // namespace Octinc

#endif
