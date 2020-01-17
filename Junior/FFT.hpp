/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_FFT__
#define OCTINC_FFT__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <cmath>
#include <iostream>
#include "complex.h"

namespace Octinc {
namespace detail {
template <typename __Tp>
static void __FFT_Rearrange(const complex<__Tp>* const Input,
                            const complex<__Tp>* Output, const unsigned int N) {
    unsigned int tar = 0;

    for (unsigned int i = 0; i < N; ++i) {
        Output[tar] = Input[i];
        unsigned int t = N;

        while (tar & (t >>= 1)) tar &= ~t;

        tar |= t;
    }
}

template <typename __Tp>
static void __FFT_Rearrange(const complex<__Tp>* Data, const unsigned int N) {
    unsigned int tar = 0;

    for (unsigned int i = 0; i < N; ++i) {
        if (tar > i) std::swap(Data[tar], Data[i]);

        unsigned int t = N;

        while (tar & (t >>= 1)) tar &= ~t;

        tar |= t;
    }
}

template <typename __Tp>
static void __FFT_Perform(const complex<__Tp>* Data, const unsigned int N,
                          const bool opt) {
    const double Pi = opt ? acos(-1.0) : -acos(1.0);

    for (unsigned int i = 1; i < N; i <<= 1) {
        const double delta = Pi / double(i);
        const double Sine = sin(delta * 0.5);
        const complex<__Tp> Mul(static_cast<__Tp>(-2.0 * Sine * Sine),
                                static_cast<__Tp>(sin(delta)));
        complex<__Tp> fac(1.0);

        for (unsigned int j = 0; j < i; ++j) {
            for (unsigned int k = j; k < N; k += (i << 1)) {
                const unsigned int m = k + i;
                const complex<__Tp> prod = complex<__Tp>(fac * Data[m]);
                Data[m] = Data[k] - prod;
                Data[k] += prod;
            }

            fac = Mul * fac + fac;
        }
    }
}

template <typename __Tp>
static void __FFT_Scale(const complex<__Tp>* Data, const unsigned int N) {
    const auto fac = static_cast<__Tp>(1.0 / (N * 1.0));

    for (unsigned int i = 0; i < N; ++i) Data[i] *= fac;
}
}  // namespace detail

namespace FFT {
template <typename __Tp>
static bool DFT(const complex<__Tp>* const Input, const complex<__Tp>* Output,
                const unsigned int N)
/*
DFT Fourier Transform.
*/
{
    if (!Input || !Output || N < 1 || N & (N - 1)) return false;

    detail::__FFT_Rearrange(Input, Output, N);
    detail::__FFT_Perform(Output, N, false);
    return true;
}

template <typename __Tp>
static bool DFT(const complex<__Tp>* Data, const unsigned int N)
/*
DFT Fourier Transform,
inplace version.
*/
{
    if (!Data || N < 1 || N & (N - 1)) return false;

    Rearange(Data, N);
    detail::__FFT_Perform(Data, N, false);
    return true;
}

template <typename __Tp>
static bool IDFT(const complex<__Tp>* const Input, const complex<__Tp>* Output,
                 const unsigned int N, const bool scale = true)
/*
IDFT Foureir Transform.
*/
{
    if (!Input || !Output || N < 1 || N & (N - 1)) return false;

    detail::__FFT_Rearrange(Input, Output, N);
    detail::__FFT_Perform(Output, N, true);

    if (scale) detail::__FFT_Scale(Output, N);

    return true;
}

template <typename __Tp>
static bool IDFT(const complex<__Tp>* Data, const unsigned int N,
                 const bool scale = true)
/*
IDFT Fourier Transform,
inplace version.
*/
{
    if (!Data || N < 1 || N & (N - 1)) return false;

    detail::__FFT_Rearrange(Data, N);
    detail::__FFT_Perform(Data, N, true);

    if (scale) detail::__FFT_Scale(Data, N);

    return true;
}
}  // namespace FFT
}  // namespace Octinc

#endif
