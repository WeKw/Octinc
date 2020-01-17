/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_DISJOINTSET_HPP
#  define OCTING_DISJOINTSET_HPP
// Head files
#  ifndef STD_BITS_STDCPP_H
#    define STD_BITS_STDCPP_H
#    include <bits/stdc++.h>
#  endif
// Class Definition
namespace Octinc {
    template<int PointNum> class DisjointSet;
}
// Class Realization
namespace Octinc {
    template<int PointNum> class DisjointSet {
        // Variable Definition
        private:
            int fi[PointNum];
        // Function Definition
        public:
            DisjointSet();
            void clear();
            int find(int);
            void merge(int, int);
    };
}
// Function Realization
namespace Octinc {
    template<int PointNum> inline DisjointSet<PointNum>::DisjointSet() {
        for (int i = 1; i <= PointNum; ++i) {
            fi[i] = i;
        }
    }
    template<int PointNum> inline void DisjointSet<PointNum>::clear() {
        for (int i = 1; i <= PointNum; ++i) {
            fi[i] = i;
        }
    }
    template<int PointNum> inline int DisjointSet<PointNum>::find(int x) {
        return this->fi[x] == x ? x : this->fi[x] = this->find(this->fi[x]);
    }
    template<int PointNum> inline void DisjointSet<PointNum>::merge(int x, int y) {
        x = this->find(x);
        y = this->find(y);
        if (x > y) {
            this->fi[y] = x;
        } else {
            this->fi[x] = y;
        }
    }
}
#endif