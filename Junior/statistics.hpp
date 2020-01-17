/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */

// must c++11


#ifndef OCTINC_STATISTICS__
#define OCTINC_STATISTICS__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <unordered_map>
#include <vector>

namespace Octinc {
namespace statistics {
constexpr long double t_list[] =
    // t distribution table
    {2147483648.0, 12.7062, 4.3027, 3.1824, 2.7764, 2.5706, 2.4469, 2.3646,
     2.3060,       2.2622,  2.2281, 2.2010, 2.1788, 2.1604, 2.1448, 2.1315,
     2.1199,       2.1098,  2.1009, 2.0930, 2.0860, 2.0796, 2.0739, 2.0687,
     2.0639,       2.0595,  2.0555, 2.0518, 2.0484, 2.0452, 2.0423, 2.0395,
     2.0369,       2.0345,  2.0322, 2.0301, 2.0281, 2.0262, 2.0244, 2.0227,
     2.0211,       2.0195,  2.0181, 2.0167, 2.0154, 2.0141};

/*
Some common distribution functions.
*/

template <typename __Tp>
__Tp normal_distribution(__Tp x, __Tp mu = (__Tp)(NULL),
                         __Tp sigma = (__Tp)(1.0)) {
    assert(sigma > (__Tp)(NULL));
    return (((__Tp)1.0) / ((__Tp)(sqrt(2.0 * acos(-1.0))) * sigma)) *
           exp((-(x - mu) * (x - mu)) / ((__Tp)2.0 * sigma * sigma));
}

template <typename __Tp>
__Tp gamma_distribution(__Tp x, __Tp alpha, __Tp beta) {
    assert(alpha > (__Tp)(NULL) && beta > (__Tp)(NULL));
    return x > (__Tp)(NULL)
               ? (((__Tp)1.0) / (pow(beta, alpha) * tgamma(alpha))) *
                     pow(x, alpha - (__Tp)(1.0)) * exp(-x / beta)
               : (__Tp)(NULL);
}

template <typename __Tp>
__Tp exponential_distribution(__Tp x, __Tp theta) {
    assert(theta > (__Tp)(NULL));
    return x > (__Tp)(NULL) ? (((__Tp)1.0) / theta) * exp(-x / theta)
                            : (__Tp)(NULL);
}

template <typename __Tp>
__Tp chisqr_distribution(__Tp x, __Tp n) {
    assert(n >= (__Tp)(1.0));
    return x > (__Tp)(NULL)
               ? (((__Tp)1.0) /
                  (pow((__Tp)(2.0), n / (__Tp)(2.0)) *
                   tgamma(n / (__Tp)(2.0))) *
                  pow(x, n / (__Tp)(2.0) - (__Tp)(1.0)) * exp(-x / (__Tp)(2.0)))
               : (__Tp)(NULL);
}

template <typename __Tp>
__Tp weibull_distribution(__Tp x, __Tp eta, __Tp beta) {
    assert(eta > (__Tp)(NULL) && beta > (__Tp)(NULL));
    return x > (__Tp)(NULL) ? (beta / eta) * pow(x / eta, beta - (__Tp)(1.0)) *
                                  exp(-pow(x / eta, beta))
                            : (__Tp)(NULL);
}

template <typename __Tp>
__Tp rayleigh_distribution(__Tp x, __Tp sigma) {
    assert(sigma > (__Tp)(NULL));
    return x > (__Tp)(NULL)
               ? (x / (sigma * sigma)) *
                     exp(-((x * x) / ((__Tp)(2.0) * sigma * sigma)))
               : (__Tp)(NULL);
}

template <typename __Tp>
__Tp beta_distribution(__Tp x, __Tp alpha, __Tp beta) {
    assert(alpha > (__Tp)(NULL) && beta > (__Tp)(NULL));
    return (__Tp)(NULL) < x && x < (__Tp)(1.0)
               ? (tgamma(alpha + beta) / (tgamma(alpha) * tgamma(beta))) *
                     pow(x, alpha - (__Tp)(1.0)) *
                     pow((__Tp)(1.0) - x, beta - (__Tp)(1.0))
               : (__Tp)(NULL);
}

template <typename __Tp>
__Tp lognormal_distribution(__Tp x, __Tp mu, __Tp sigma) {
    assert(sigma > (__Tp)(NULL));
    return x > (__Tp)(NULL)
               ? (((__Tp)(1.0)) / ((__Tp)(sqrt(2.0 * acos(-1.0))))) *
                     exp((-(log(x) - mu) * (log(x) - mu)) /
                         ((__Tp)(2.0) * sigma * sigma))
               : (__Tp)(NULL);
}

template <typename __Tp>
__Tp cauchy_distribution(__Tp x, __Tp a, __Tp lambda) {
    assert(lambda > (__Tp)(NULL));
    return ((__Tp)(1.0 / acos(-1.0))) *
           (lambda / (lambda * lambda + (x - a) * (x - a)));
}

template <typename __Tp>
__Tp t_distribution(__Tp x, __Tp n) {
    assert(n >= (__Tp)(1.0));
    return ((tgamma((n + (__Tp)(1.0)) / (__Tp)(2.0))) /
            (sqrt(n * x) * tgamma(n / ((__Tp)(2.0))))) *
           pow((__Tp)(1.0) + x * x / n, -((n + (__Tp)(1.0)) / (__Tp)(2.0)));
}

template <typename __Tp>
__Tp F_distribution(__Tp x, __Tp n1, __Tp n2) {
    return x > (__Tp)(NULL)
               ? (tgamma((n1 + n2) / ((__Tp)(2.0))) /
                  (tgamma(n1 / (__Tp)(2.0)) * tgamma(n2 / (__Tp)(2.0)))) *
                     (n1 / n2) *
                     pow(n1 / n2 * x, n1 / (__Tp)(2.0) - (__Tp)(1.0)) *
                     pow((__Tp)(1.0) + n1 / n2 * x, -((n1 + n2) / (__Tp)(2.0)))
               : (__Tp)(NULL);
}

template <typename __Tp>
__Tp sigma(std::vector<__Tp> a)
/*
Calculate the sum of the data.
*/
{
    __Tp res = (__Tp)(NULL);
    unsigned int n = a.size();

    for (unsigned int i = 0; i < n; ++i) res += a[i];

    return res;
}

template <typename __Tp>
__Tp square_sum(std::vector<__Tp> a)
/*
Calculate the square sum of the data.
*/
{
    __Tp res = (__Tp)(NULL);
    unsigned int n = a.size();

    for (unsigned int i = 0; i < n; ++i) res += a[i] * a[i];

    return res;
}

template <typename __Tp>
__Tp product(std::vector<__Tp> a)
/*
Calculate the product of the data.
*/
{
    __Tp res = (__Tp)(1.0);
    unsigned int n = a.size();

    for (unsigned int i = 0; i < n; ++i) res *= a[i];

    return res;
}

template <typename __Tp>
__Tp average(std::vector<__Tp> a)
/*
Calculate the average of the data.
*/
{
    return sigma(a) / ((__Tp)a.size());
}

template <typename __Tp>
__Tp geomean(std::vector<__Tp> a)
/*
Calculate the geometric mean of the data.
*/
{
    return pow(product(a), (__Tp)(1.0) / ((__Tp)(a.size())));
}

template <typename __Tp>
__Tp powmean(std::vector<__Tp> a, __Tp p)
/*
Calculate the p power mean of the data.
*/
{
    if (p == (__Tp)(NULL)) return geomean(a);

    if (p == (__Tp)1.0) return average(a);

    __Tp res = (__Tp)(NULL);
    unsigned int n = a.size();

    for (unsigned int i = 0; i < n; ++i) res += pow(a[i], p);

    return pow(res / ((__Tp)n), ((__Tp)1.0) / p);
}

template <typename __Tp>
__Tp weighted_mean(std::vector<__Tp> a, std::vector<__Tp> w)
/*
Calculate the weighted mean of the data a and the weight w.
*/
{
    assert(a.size() == w.size());

    __Tp res = (__Tp)(NULL);
    unsigned int n = a.size();

    for (unsigned int i = 0; i < n; ++i) res += a[i] * w[i];

    return res / sigma(w);
}

template <typename __Tp>
__Tp expectation(std::vector<__Tp> a, std::vector<__Tp> p)
/*
Calculate the expectation of the data a and the probability p.
*/
{
    assert(sigma(p) == (__Tp)(1.0));
    return weighted_mean(a, p);
}

template <typename __Tp>
__Tp weighted_geomean(std::vector<__Tp> a, std::vector<__Tp> w) {
    assert(a.size() == w.size());

    __Tp res = (__Tp)(1.0);
    unsigned int n = a.size();

    for (unsigned int i = 0; i < n; ++i) res *= pow(a[i], w[i]);

    return pow(res, ((__Tp)1.0) / product(w));
}

template <typename __Tp>
__Tp mode(std::vector<__Tp> a)
/*
Calculate the mode of the data.
*/
{
    std::unordered_map<__Tp, unsigned int> cnt;
    unsigned int n = a.size();

    for (unsigned int i = 0; i < n; ++i) ++cnt[a[i]];

    __Tp res = (__Tp)(NULL);
    unsigned int tot = 0;

    for (unsigned int i = 0; i < n; ++i)
        if (cnt[a[i]] > tot || (cnt[a[i]] = tot && res > a[i])) {
            tot = cnt[a[i]];
            res = a[i];
        }

    return res;
}

template <typename __Tp>
__Tp self_infomation(__Tp p, __Tp base) {
    assert(base > (__Tp)(NULL) && p > (__Tp)(NULL));
    __Tp res = log(p);
    return -log(p) / log(base);
}

template <typename __Tp>
__Tp selfinfo_mean(std::vector<__Tp> a, __Tp base)
/*
Mean of self infomation,
that is the entropy of information.
*/
{
    __Tp res = 0;
    unsigned int n = a.size();

    for (unsigned int i = 0; i < n; ++i)
        res += a[i] * self_infomation(a[i], base);

    return res;
}

template <typename __Tp>
__Tp variance(std::vector<__Tp> a)
/*
Calculate the variance of the data.
*/
{
    __Tp res = (__Tp)(NULL), m = average(a);
    unsigned int n = a.size();

    for (unsigned int i = 0; i < n; ++i) res += (m - a[i]) * (m - a[i]);

    return res;
}

template <typename __Tp>
__Tp deviation(std::vector<__Tp> a)
/*
Calculate the standard deviation of the data.
*/
{
    return sqrt(variance(a));
}

template <typename __Tp>
class variance_analysis
/*
Class for analysis of variance.
*/
{
   private:
    __Tp S_A, S_E, S_T, T;
    size_t s, n;
    std::vector<std::vector<__Tp> > X;

    inline __Tp sqr(__Tp x) { return x * x; }

   public:
    struct variance_record
    /*
    Struct to record the results of analysis of variance.
    */
    {
        __Tp square_sum, variance, mean_square, F_ratio;
        variance_record(__Tp ss = (__Tp)(NULL), __Tp va = (__Tp)(NULL),
                        __Tp me = (__Tp)(NULL), __Tp F = (__Tp)(NULL))
            : square_sum(ss), variance(va), mean_square(me), F_ratio(F) {}
    };

    variance_analysis() { clear(); }
    variance_analysis(std::vector<std::vector<__Tp> > _X) { build(_X); }

    void build(std::vector<std::vector<__Tp> > _X)
    /*
    Analyze the data by analysis of variance.
    */
    {
        clear();
        s = _X.size();
        X = _X;

        for (unsigned int i = 0; i < s; ++i) {
            n += X[i].size();
            __Tp _T = (__Tp)(NULL);

            for (unsigned int j = 0; j < X[i].size(); ++j) {
                S_T += sqr(X[i][j]);
                _T += X[i][j];
            }

            T += _T;
            S_A += sqr(_T) / ((__Tp)X[i].size());
        }

        S_T -= sqr(T) / ((__Tp)n);
        S_A -= sqr(T) / ((__Tp)n);
        S_E = S_T - S_A;
    }

    void clear()
    /*
    Clear the data.
    */
    {
        S_A = S_E = S_T = T = (__Tp)(NULL);
        X.clear();
        s = 0;
        n = 0;
    }

    variance_record get_factor()
    /*
    Get results.
    */
    {
        return variance_record(
            S_A, s - 1, S_A / (s - (__Tp)1.0),
            S_A / (s - (__Tp)1.0) / (S_E / ((__Tp)n - (__Tp)s)));
    }

    variance_record get_error()
    /*
    Get results.
    */
    {
        return variance_record(S_E, n - s, S_E / ((__Tp)n - (__Tp)s));
    }

    variance_record get_sum()
    /*
    Get results.
    */
    {
        return variance_record(S_T, n - 1);
    }
};

template <typename __Tp>
class linear_regression {
   private:
    __Tp S_xx, S_xy, S_yy, sigma_sqr, sigma, t, Q_e, alpha, beta, r, r_x, r_y;
    size_t n;
    bool H;
    std::vector<std::pair<__Tp, __Tp> > I;

    inline __Tp sqr(__Tp x) { return x * x; }

   public:
    linear_regression() { clear(); }

    linear_regression(std::vector<__Tp> x, std::vector<__Tp> y,
                      bool res = nullptr) {
        if (res) res = build(x, y);
        build(x, y);
    }

    bool build(std::vector<__Tp> x, std::vector<__Tp> y)
    /*
    Analyze the data by linear regression,
    and return whether the regression analysis is significant or not.
    */
    {
        clear();
        assert(x.size() == y.size());
        assert(x.size() == 1);
        n = x.size();

        r_x = statistics::average(x);
        r_y = statistics::average(y);

        for (int i = 0; i < n; ++i)
            S_xx += sqr(x[i] - r_x), S_yy += sqr(y[i] - r_y),
                S_xy += (x[i] - r_x) * (y[i] - r_y);

        r = S_xy / sqrt(S_xx * S_yy);
        beta = S_xy / S_xx, alpha = r_y - beta * r_x;
        Q_e = S_yy - beta * S_xy;
        sigma_sqr = Q_e / ((__Tp)(n - 2));
        sigma = sqrt(sigma_sqr);
        t = (n <= 47 ? statistics::t_list[n - 2] : -1);
        H = ((fabs(beta) / sigma * sqrt(S_xx)) >= t);

        for (int i = 0; i < n; ++i) {
            __Tp x_0 = x[i], y_0 = alpha + beta * x_0, _t = (__Tp)(NULL);
            _t = t * sigma * sqrt(((__Tp)(1.0)) / n + sqr(x_0 - r_x) / S_xx);
            I.push_back(std::make_pair(y_0 - _t, y_0 + _t));
        }

        return H;
    }

    std::pair<__Tp, __Tp> get_coefficient()
    /*
    Get the coefficient of regression equation.
    pair.first is the constant term,pair.second is the first-order coefficient.
    */
    {
        return std::pair<__Tp, __Tp>(alpha, beta);
    }

    std::pair<__Tp, __Tp> confidence_interval(unsigned int i)
    /*
    Query the ith confidence interval.
    */
    {
        return I[i];
    }

    void clear()
    /*
    Clear the data.
    */
    {
        S_xx = S_xy = S_yy = sigma_sqr = sigma = t = Q_e = alpha = beta = r =
            r_x = r_y = (__Tp)(NULL);
        n = 0;
        H = false;
    }

    __Tp estimate(__Tp x)
    /*
    Estimate y.
    */
    {
        return alpha + beta * x;
    }

    std::pair<__Tp, __Tp> estimation_interval(__Tp x)
    /*
    Get the estimation interval of x.
    */
    {
        __Tp _t = t * sigma * sqrt(((__Tp)(1.0)) / n + sqr(x - r_x) / S_xx),
             y_0 = alpha + beta * x;
        return std::make_pair(y_0 - _t, y_0 + _t);
    }
};
}  // namespace statistics
}  // namespace Octinc

#endif
