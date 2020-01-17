/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_PEAK__
#define OCTINC_PEAK__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <functional>
#include <utility>
#include <valarray>
#include <vector>

namespace Octinc {
template <typename __Tp, typename __Cmp = std::less<__Tp> >
/*
__Cmp is the way to check peaks.
*/
class peak {
   private:
    void filter(const __Tp* const freq, size_t siz, bool opt = 0) {
        if (siz != m_freq.size()) m_freq.resize(siz);

        for (size_t i = 1; i < siz; ++i) m_freq[i] = freq[i];

        if (!opt) return;

        __Tp b[] = {(__Tp)0.15, (__Tp)0.2f, (__Tp)0.3, (__Tp)0.2, (__Tp)0.15};
        const static size_t corr_len = sizeof(b) / sizeof(b[0]);

        for (size_t i = 0; i < corr_len - 1; ++i) {
            m_freq[i] += freq[i - 0] * b[corr_len - 1];

            if (i == 0) continue;

            m_freq[i] += freq[i - 1] * b[corr_len - 2];

            if (i == 1) continue;

            m_freq[i] += freq[i - 2] * b[corr_len - 3];

            if (i == 2) continue;

            m_freq[i] += freq[i - 3] * b[corr_len - 4];
        }

        for (size_t i = corr_len - 1; i < siz; ++i) {
            m_freq[i] += freq[i - 0] * b[corr_len - 1];
            m_freq[i] += freq[i - 1] * b[corr_len - 2];
            m_freq[i] += freq[i - 2] * b[corr_len - 3];
            m_freq[i] += freq[i - 3] * b[corr_len - 4];
            m_freq[i] += freq[i - 4] * b[corr_len - 5];
        }
    }

    void find_peaks() {
        size_t siz = m_freq.size();
        m_ipks.clear();
        __Cmp _cmp;

        for (size_t i = 1; i < siz - 1; ++i)
            if ((_cmp(m_freq[i - 1], m_freq[i]) ||
                 m_freq[i] == m_freq[i - 1]) &&
                (_cmp(m_freq[i + 1], m_freq[i]) || m_freq[i] == m_freq[i + 1]))
                m_ipks.push_back(i);
    }

    void remove_below(__Tp minT, __Tp minH, size_t minW) {
        for (auto it = m_ipks.begin(); it != m_ipks.end();)
            if (m_freq[*it] < minH) {
                it = m_ipks.erase(it);
                continue;
            } else
                ++it;

        size_t siz = m_ipks.size();
        std::vector<size_t> res;

        for (size_t i = 0; i < siz; ++i) {
            auto idx = m_ipks[i];
            __Cmp _cmp;
            auto v = _cmp(m_freq[idx - 1], m_freq[idx + 1]) ? m_freq[idx + 1]
                                                            : m_freq[idx - 1];

            if (m_freq[idx] - v > minT) res.push_back(m_ipks[i]);
        }

        result_t pks;
        pks.resize(res.size());

        for (size_t i = 0; i < res.size(); ++i) {
            pks[i].first = m_freq[res[i]];
            pks[i].second = res[i];
        }

        std::sort(pks.begin(), pks.end(),
                  [](const std::pair<__Tp, size_t>& x,
                     const std::pair<__Tp, size_t>& y) {
                      __Cmp _cmp;
                      return _cmp(x.first, x.second);
                  });

        siz = pks.size();
        std::vector<int> idel(siz, 0);

        for (size_t i = 0; i < siz; ++i)
            if (idel[i] == 0) {
                for (size_t j = 0; j < idel.size(); ++j)
                    idel[j] =
                        idel[j] || (pks[j].second + minW >= pks[i].second) &&
                                       (pks[j].second <= pks[i].second + minW);

                idel[i] = 0;
            }

        m_ipks.clear();
        siz = idel.size();

        for (size_t i = 0; i < siz; ++i)
            if (!idel[i]) m_ipks.push_back(pks[i].second);

        std::sort(m_ipks.begin(), m_ipks.end());
    }

    std::valarray<__Tp> m_freq;
    std::vector<size_t> m_ipks;
    int m_dis = 5;
    __Tp m_thre = (__Tp)0.0;
    __Tp m_ht = (__Tp)0.05;

   public:
    typedef std::vector<std::pair<__Tp, size_t> > result_t;

    peak(int dis = 0, __Tp ht = __Tp(), __Tp thre = __Tp())
        : m_dis(dis), m_ht(ht), m_thre(thre) {}

    result_t process(__Tp* freq, size_t siz, bool opt = 0)
    /*
    Process the array and return the peaks.
    opt means whether use the smooth filter or not.
    */
    {
        result_t res;
        assert(freq && siz > 1);

        filter(freq, siz, opt);
        find_peaks();
        remove_below(m_thre, m_ht, m_dis);

        for (size_t i = 0; i < m_ipks.size(); ++i)
            res.push_back(std::make_pair(m_freq[m_ipks[i]], m_ipks[i]));

        return res;
    }

    void set_distance(int dis)
    /*
    Set the minimum distance.
    */
    {
        m_dis = dis;
    }

    void set_height(__Tp ht)
    /*
    Set the minimum height difference.
    */
    {
        m_ht = ht;
    }

    void set_threshold(__Tp thre)
    /*
    Set the minimum threshold.
    */
    {
        m_thre = thre;
    }
};
}  // namespace Octinc

#endif
