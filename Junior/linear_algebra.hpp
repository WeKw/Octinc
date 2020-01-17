/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_LINEAR_ALGEBRA__
#define OCTINC_LINEAR_ALGEBRA__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <algorithm>
#include <climits>
#include <cmath>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <vector>

namespace Octinc {
template <typename __Tp>
class matrix {
   private:
    typedef matrix<__Tp> matrix_t;
    typedef std::vector<__Tp> vec_t;

#define rcm_t const matrix_t&
   protected:
    size_t height, width;
    static constexpr __Tp __base = (__Tp)(1.0);
    static constexpr __Tp zero = __base - __base, one = __base / __base;
    std::vector<vec_t> vec;

   public:
    explicit matrix(size_t h = 2, size_t w = 2, __Tp init = zero)
        : height(h), width(w) {
        vec.resize(height);

        for (size_t i = 0; i < height; ++i)
            for (size_t j = 0; j < width; ++j) vec[i].push_back(init);
    }

    matrix(std::vector<vec_t> newvec) : height(newvec.size()) {
        vec = newvec;
        width = vec[0].size();
    }

    matrix(std::initializer_list<std::initializer_list<__Tp> > ini)
        : height(ini.size()) {
        vec.resize(height);

        for (auto it = ini.begin(); it < ini.end(); ++it)
            for (auto jt = it->begin(); jt < it->end(); ++jt)
                vec[it].push_bavk(*jt);
    }

    matrix(rcm_t rhs) { *this = rhs; }

    friend matrix_t matrix_cast(__Tp num, size_t s = 2) {
        matrix_t ans(s, s);

        for (size_t i = 0; i < s; ++i) ans[i][i] = num;

        return ans;
    }

    vec_t& operator[](size_t index) { return vec[index]; }

    matrix_t operator+(rcm_t rhs) {
        matrix_t ans;

        if (height != rhs.height || width != rhs.width)
            throw "cannot do the calc";

        ans.vec.resize(height);

        for (size_t i = 0; i < height; ++i)
            for (size_t j = 0; j < width; ++j)
                ans[i].push_back(vec[i][j] + rhs.vec[i][j]);

        return ans;
    }

    matrix_t operator+=(rcm_t rhs) {
        *this = *this + rhs;
        return *this;
    }

    matrix_t operator-(rcm_t rhs) {
        matrix_t ans;

        if (height != rhs.heigh || width != rhs.width)
            throw "cannot do the calc";

        ans.vec.resize(height);

        for (size_t i = 0; i < height; ++i)
            for (size_t j = 0; j < width; ++j)
                ans[i].push_back(vec[i][j] - rhs.vec[i][j]);

        return ans;
    }

    matrix_t operator-=(rcm_t rhs) {
        *this = *this - rhs;
        return *this;
    }

    matrix_t operator*(rcm_t rhs) {
        matrix_t ans(height, rhs.width);

        if (width != rhs.height) throw "cannot do the calc";

        for (size_t i = 0; i < height; ++i)
            for (size_t j = 0; j < rhs.width; ++j)
                for (size_t k = 0; k < width; ++k)
                    ans[i][j] += vec[i][k] * rhs.vec[k][j];

        return ans;
    }

    matrix_t operator*=(rcm_t rhs) {
        *this = *this * rhs;
        return *this;
    }

    matrix_t operator/(rcm_t rhs) {
        if (rhs.det() == zero || width != rhs.inv().height)
            throw "cannot do the calc";

        return (*this) * (rhs.inv());
    }

    matrix_t operator/=(rcm_t rhs) {
        *this = *this / rhs;
        return *this;
    }

    matrix_t operator*(__Tp n) {
        matrix_t ans(*this);

        for (size_t i = 0; i < ans.height; ++i)
            for (size_t j = 0; j < ans.width; ++j) ans[i][j] *= n;

        return ans;
    }

    matrix_t operator*=(__Tp n) {
        *this = *this * n;
        return *this;
    }

    matrix_t operator/(__Tp n) {
        matrix_t ans(*this);

        for (size_t i = 0; i < ans.height; ++i)
            for (size_t j = 0; j < ans.width; ++j) ans[i][j] /= n;

        return ans;
    }

    matrix_t operator/=(__Tp n) {
        *this = *this / n;
        return *this;
    }

    friend bool operator==(rcm_t lhs, rcm_t rhs) { return lhs.vec == rhs.vec; }

    friend bool operator!=(rcm_t lhs, rcm_t rhs) { return !(lhs == rhs); }

    friend std::ostream& operator<<(std::ostream& os, rcm_t mat) {
        for (size_t i = 0; i < mat.height; ++i) {
            for (size_t j = 0; j < mat.width; ++j) os << mat.vec[i][j] << " ";

            os << std::endl;
        }

        return os;
    }

    friend std::istream& operator>>(std::istream& is, matrix_t& mat) {
        for (size_t i = 0; i < mat.height; ++i)
            for (size_t j = 0; j < mat.width; ++j) is >> mat.vec[i][j];

        if (!is) mat = matrix_t(mat.height, mat.width);

        return is;
    }

    std::pair<size_t, size_t> size() { return std::make_pair(height, width); }

    matrix_t T()
    /*
    Transpose.
    */
    {
        matrix_t ans = *this;

        for (size_t i = 0; i < height; ++i)
            for (size_t j = 0; j < width; ++j) ans[j][i] = vec[i][j];

        return ans;
    }

    matrix_t inv(const __Tp eps = 1e-8)
    /*
    Inverse matrix.
    */
    {
        __Tp d = det();
        if (fabs(d) <= eps) throw "cannot the calc";
        return adjugate_mat() / det(eps);
    }

    __Tp det(const __Tp eps = 1e-8)
    /*
    Determinant.
    EPS is the allowable error.
    */
    {
        if (height != width) throw "cannot do the calc";

        matrix mat = *this;
        __Tp mul, res = one;

        int b[height + 1];

        for (int i = 0; i < height; ++i) b[i] = i;

        for (int i = 0; i < height; ++i) {
            if (fabs(mat[b[i]][i]) <= eps)
                for (int j = i + 1; j < height; ++j)
                    if (fabs(mat[b[j]][i]) > eps) {
                        std::swap(b[i], b[j]);
                        res = -res;
                        break;
                    }

            res *= mat[b[i]][i];

            for (int j = i + 1; j < height; ++j)
                if (fabs(mat[b[j]][i]) > eps) {
                    mul = mat[b[j]][i] / mat[b[i]][i];

                    for (int k = i; k < height; ++k)
                        mat[b[j]][k] -= mat[b[i]][k] * mul;
                }
        }

        return res;
    }

    __Tp alcofactor(size_t i, size_t j)
    /*
    Algebra cofactor.
    */
    {
        return (i + j & 1 ? -1 : 1) * cofactor(i, j);
    }

    __Tp cofactor(size_t i, size_t j)
    /*
    Cofactor.
    */
    {
        return cofactor_mat(i, j).det();
    }

    matrix_t cofactor_mat(size_t i, size_t j)
    /*
    Matrix of cofactor.
    */
    {
        matrix_t ans(*this);

        if (i >= height || j >= width) throw "over range";

        ans.erase(i, j);
        return ans;
    }

    matrix_t adjugate_mat()
    /*
    Adjugate matrix.
    */
    {
        matrix_t ans(height, width);
        ans.vec.resize(height);

        for (size_t i = 0; i < height; ++i)
            for (size_t j = 0; j < width; ++j) ans[i][j] = alcofactor(i, j);

        ans = ans.T();
        return ans;
    }

    void resize(size_t h, size_t w) {
        height = h, width = w;
        vec.resize(height);

        for (size_t i = 0; i < height; ++i) vec[i].resize(width);
    }

    void erase(size_t i, size_t j)
    /*
    Delete a row and a column.
    */
    {
        erase_row(i), erase_col(j);
    }

    void erase_row(size_t i)
    /*
    Delete a row.
    */
    {
        vec.erase(vec.begin() + i);
        --height;
    }

    void erase_col(size_t j)
    /*
    Delete a column.
    */
    {
        for (size_t i = 0; i < height; ++i) vec[i].erase(vec[i].begin() + j);

        --width;
    }

    void each(std::function<__Tp(__Tp x)> func) {
        for (size_t i = 0; i < height; ++i)
            for (size_t j = 0; j < width; ++j) vec[i][j] = func(vec[i][j]);
    }

    matrix_t pow(int n) {
        if (n < 0) return pow(-n).inv();
        if (n == 0) return matrix_cast(one, height);

        --n;
        matrix_t ans(*this), tmp(*this);

        while (n) {
            if (n & 1) ans = ans * tmp;

            tmp = tmp * tmp;
            n >>= 1;
        }

        return ans;
    }
#undef rcm_t
};

template <typename __Tp>
bool Gauss_elimination(matrix<__Tp> a, std::vector<__Tp>& b,
                       const __Tp eps = 1e-8)
/*
Gauss elimination.
EPS is the allowable error of calculation.
The solution vector is stored in B,
and return whether there is a unique solution.
*/
{
    int r;
    b.clear();

    if (a.size().first != a.size().second - 1) return false;

    int n = a.size().first;
    __Tp f;

    for (int i = 0; i < n; ++i) {
        r = i;

        for (int j = i + 1; j < n; ++j)
            if (fabs(a[j][i]) > fabs(a[r][i])) r = j;

        if (fabs(a[r][i]) < eps) return false;

        if (r != i)
            for (int j = 0; j <= n; ++j) std::swap(a[r][j], a[i][j]);

        for (int k = i + 1; k < n; ++k) {
            f = a[k][i] / a[i][i];

            for (int j = i; j <= n; ++j) a[k][j] -= f * a[i][j];
        }
    }

    for (int i = n - 1; i >= 0; --i) {
        for (int j = i + 1; j < n; ++j) a[i][n] -= a[j][n] * a[i][j];

        a[i][n] /= a[i][i];
    }

    for (int i = 0; i < n; ++i) b.emplace_back(a[i][n]);

    return true;
}

template <typename __Tp>
class linear_programming {
   private:
    matrix<__Tp> mat;
    std::set<int> P;
    size_t cn, bn;

    bool Pivot(std::pair<size_t, size_t>& p, const __Tp eps = 1e-8,
               const __Tp INF = INT_MAX) {
        int x = 0, y = 0;
        __Tp cmax = -INF;
        std::vector<__Tp> C = mat[0], B;

        for (int i = 0; i < bn; ++i) B.push_back(mat[i][cn - 1]);

        for (int i = 0; i < C.size(); ++i)
            if (cmax < C[i] && P.find(i) == P.end()) {
                cmax = C[i];
                y = i;
            }

        if (cmax < 0) return false;

        __Tp bmin = INF;

        for (int i = 1; i < bn; ++i) {
            __Tp tmp = B[i] / mat[i][y];

            if (mat[i][y] != 0 && bmin > tmp) {
                bmin = tmp;
                x = i;
            }
        }

        p = std::make_pair(x, y);

        for (auto it = P.begin(); it != P.end(); ++it)
            if (fabs(mat[x][*it]) > eps) {
                P.erase(*it);
                break;
            }

        P.insert(y);

        return true;
    }

    void Gaussian(std::pair<size_t, size_t> p, const __Tp eps = 1e-8) {
        size_t x = p.first, y = p.second;
        __Tp norm = mat[x][y];

        for (int i = 0; i < cn; ++i) mat[x][i] /= norm;

        for (int i = 0; i < bn && i != x; ++i)
            if (fabs(mat[i][y]) > eps) {
                __Tp tmpnorm = mat[i][y];

                for (int j = 0; j < cn; ++j) mat[i][j] -= tmpnorm * mat[x][j];
            }
    }

   public:
    linear_programming() {}
    linear_programming(const matrix<__Tp>& mat) : mat(mat) {
        cn = mat.size().first;
        bn = mat.size().second;
    }

    linear_programming operator=(const matrix<__Tp>& mat)
    /*
    Import simplicity.
    */
    {
        this->mat = mat;
        cn = mat.size().first;
        bn = mat.size().second;
        return *this;
    }

    std::pair<__Tp, std::set<int> > simplex(const __Tp eps = 1e-8)
    /*
    Solve the linear programming.
    Return the maximized value and the solution vector.
    EPS is the allowable error.
    */
    {
        std::pair<size_t, size_t> t;
        P.clear();

        for (int i = 0; i < bn - 1; ++i) P.insert(cn - i - 2);

        while (true) {
            if (!Pivot(t, eps)) return std::make_pair(-mat[0][cn - 1], P);

            Gaussian(t, eps);
        }
    }
};
}  // namespace Octinc

#endif
