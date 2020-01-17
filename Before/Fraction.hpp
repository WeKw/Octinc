/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_FRACTION_HPP
#  define OCTINC_FRACTION_HPP
#  ifndef STD_BITS_STDCPP_H
#    define STD_BITS_STDCPP_H
#    include <bits/stdc++.h>
#  endif
namespace Octinc {
    template<typename Type> class Fraction {
        private:
            Type gcd(Type, Type);
            Type lcm(Type, Type);
        public:
            Type fx, fy;
            Fraction<Type>();
            Fraction<Type>(Type);
            Fraction<Type>(Type, Type);
            void update();
    };
    template<typename Type> Fraction<Type> operator+(Fraction<Type>, Fraction<Type>);
    template<typename Type> Fraction<Type> operator-(Fraction<Type>, Fraction<Type>);
    template<typename Type> Fraction<Type> operator*(Fraction<Type>, Fraction<Type>);
    template<typename Type> Fraction<Type> operator/(Fraction<Type>, Fraction<Type>);
    template<typename Type> Fraction<Type>& operator+=(Fraction<Type>&, Fraction<Type>);
    template<typename Type> Fraction<Type>& operator-=(Fraction<Type>&, Fraction<Type>);
    template<typename Type> Fraction<Type>& operator*=(Fraction<Type>&, Fraction<Type>);
    template<typename Type> Fraction<Type>& operator/=(Fraction<Type>&, Fraction<Type>);
    template<typename Type> Type operator<(Fraction<Type>, Fraction<Type>);
    template<typename Type> Type operator<=(Fraction<Type>, Fraction<Type>);
    template<typename Type> Type operator>(Fraction<Type>, Fraction<Type>);
    template<typename Type> Type operator>=(Fraction<Type>, Fraction<Type>);
    template<typename Type> Type operator==(Fraction<Type>, Fraction<Type>);
    template<typename Type> Type operator!=(Fraction<Type>, Fraction<Type>);
}
namespace Octinc {
    template<typename Type> inline Type Fraction<Type>::gcd(Type x, Type y) {
        if (y == 0) {
            return x;
        }
        return gcd(y, x % y);
    }
    template<typename Type> inline Type Fraction<Type>::lcm(Type x, Type y) { return x * y / gcd(x, y); }
    template<typename Type> inline Fraction<Type>::Fraction<Type>() { fx = fy = 1; }
    template<typename Type> inline Fraction<Type>::Fraction<Type>(Type x) {
        fx = x;
        fy = 1;
    }
    template<typename Type> inline Fraction<Type>::Fraction<Type>(Type x, Type y) {
        fx = x;
        fy = y;
    }
    template<typename Type> inline void Fraction<Type>::update() {
        Type tmp = gcd(fx, fy);
        fx /= tmp;
        fy /= tmp;
    }
    template<typename Type> inline Fraction<Type> operator+(Fraction<Type> a, Fraction<Type> b) {
        Fraction<Type> c(a.fx * b.fy + a.fy * b.fx, a.fy * b.fy);
        c.update();
        return c;
    }
    template<typename Type> inline Fraction<Type> operator-(Fraction<Type> a, Fraction<Type> b) {
        Fraction<Type> c(a.fx * b.fy - a.fy * b.fx, a.fy * b.fy);
        c.update();
        return c;
    }
    template<typename Type> inline Fraction<Type> operator*(Fraction<Type> a, Fraction<Type> b) {
        Fraction<Type> c(a.fx * b.fx, a.fy * b.fy);
        c.update();
        return c;
    }
    template<typename Type> inline Fraction<Type> operator/(Fraction<Type> a, Fraction<Type> b) {
        Fraction<Type> c(a.fx * b.fy, a.fy * b.fx);
        c.update();
        return c;
    }
    template<typename Type> inline Fraction<Type>& operator+=(Fraction<Type>& a, Fraction<Type> b) { return a = a + b; }
    template<typename Type> inline Fraction<Type>& operator-=(Fraction<Type>& a, Fraction<Type> b) { return a = a - b; }
    template<typename Type> inline Fraction<Type>& operator*=(Fraction<Type>& a, Fraction<Type> b) { return a = a * b; }
    template<typename Type> inline Fraction<Type>& operator/=(Fraction<Type>& a, Fraction<Type> b) { return a = a / b; }
    template<typename Type> inline Type operator<(Fraction<Type> a, Fraction<Type> b) { return a.fx * b.fy < a.fy * b.fx; }
    template<typename Type> inline Type operator<=(Fraction<Type> a, Fraction<Type> b) { return a.fx * b.fy <= a.fy * b.fx; }
    template<typename Type> inline Type operator>(Fraction<Type> a, Fraction<Type> b) { return a.fx * b.fy > a.fy * b.fx; }
    template<typename Type> inline Type operator>=(Fraction<Type> a, Fraction<Type> b) { return a.fx * b.fy >= a.fy * b.fx; }
    template<typename Type> inline Type operator==(Fraction<Type> a, Fraction<Type> b) { return a.fx * b.fy == a.fy * b.fx; }
    template<typename Type> inline Type operator!=(Fraction<Type> a, Fraction<Type> b) { return a.fx * b.fy != a.fy * b.fx; }
}
#endif