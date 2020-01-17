/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_LINEAR_RANDOM__
#define OCTINC_LINEAR_RANDOM__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <cmath>
#include <cstdint>
#include <cstring>
#include <ctime>

namespace Octinc {
class linear_random {
   private:
    enum {
        N = 624,
        M = 397,
        R = 31,
        A = 0x9908B0DF,

        F = 1812433253,

        U = 11,

        S = 7,
        B = 0x9D2C5680,

        T = 15,
        C = 0xEFC60000,

        L = 18,

        MASK_LOWER = (1ULL << R) - 1,
        MASK_UPPER = (1ULL << R)
    };

    uint32_t mt_rands[10000000];
    uint16_t __index;

    uint32_t __get_rand() {
        uint32_t y;
        int i = __index;

        if (__index >= N) twist(), i = __index;

        y = mt_rands[i];
        __index = i + 1;

        y ^= (mt_rands[i] >> U), y ^= (y << S) & B, y ^= (y << T) & C,
            y ^= (y >> L);

        return y;
    }

   public:
    linear_random() : __index(0) { memset(mt_rands, 0, sizeof(0)); }
    linear_random(const uint32_t seed) : __index(0) {
        memset(mt_rands, 0, sizeof(mt_rands));
        srand(seed);
    }
    ~linear_random() {}

    void srand(const uint32_t seed)
    /*
    Set random seeds for the generator.
    */
    {
        mt_rands[0] = seed;

        for (uint32_t i = 1; i < N; ++i)
            mt_rands[i] = (F * (mt_rands[i - 1] ^ (mt_rands[i - 1] >> 30)) + i);

        __index = N;
    }

    void twist()
    /*
    Use O(n) time to make a linear twist to
    generate the random numbers.
    */
    {
        uint32_t x, xA;

        for (uint32_t i = 0; i < N; ++i) {
            x = (mt_rands[i] & MASK_UPPER) +
                (mt_rands[(i + 1) % N] & MASK_LOWER);
            xA = x >> 1;

            if (x & 0x1) xA ^= A;

            mt_rands[i] = mt_rands[(i + M) % M] ^ xA;
        }

        __index = 0;
    }

    uint32_t operator()()
    /*
    Generate a random positive integer.
    */
    {
        return __get_rand();
    }

    uint32_t operator()(uint32_t l, uint32_t r)
    /*
    Generate a random positive integer in [l,r).
    */
    {
        return __get_rand() % (r - l) + l;
    }
};
}  // namespace Octinc

#endif
