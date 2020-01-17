/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_GAME_THEORY__
#define OCTINC_GANE_THEORY__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <iostream>

namespace Octinc {
template <typename __Tp>
/*
Solve Nim Game, where the rule is:
There are n piles of stones on the ground.
Every time two positions can take out any number of stones from any pile of
stones and throw them away. They can take them and can only take from a pile at
a time. In the end, the two positions who can't take stones will lose.
*/
class Nim_solver {
   private:
    __Tp res;

   public:
    Nim_solver() : res(0) {}

    void insert(__Tp x)
    /*
    Insert a number of stones.
    */
    {
        res ^= x;
    }

    bool certain_win()
    /*
    Check if the offensive position has the winning strategy.
    */
    {
        return x != 0;
    }
};

template <typename __Tp>
/*
Solve Euclidean Game, where the rule is:
Positive integers M and N, starting from the man on the offensive, subtract the
positive integer multiple of the smaller number from the larger one. Of course,
the number obtained can not be less than 0. Then it is the defensive position,
and then do the same operation for the number just obtained, and the smaller one
in M and N... Until a man gets 0, he wins.
*/
class Euclidean_solver {
   private:
    bool solve(__Tp n, __Tp m) {
        if (!m) return false;
        if (n / m == 1)
            return !solve(m, n % m);
        else
            return true;
    }

   public:
    bool certain_win(__Tp n, __Tp m)
    /*
    Check if the offensive position has the winning strategy.
    */
    {
        return solve(std::max(n, m), std::min(n, m));
    }
};
}  // namespace Octinc

#endif
