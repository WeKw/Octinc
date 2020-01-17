/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_EX_STRING_HPP
#  define OCTINC_EX_STRING_HPP
#  ifndef STD_BITS_STDCPP_H
#    define STD_BITS_STDCPP_H
#    include <bits/stdc++.h>
#  endif

namespace Octinc {
namespace detail {
void __suffix_array_radix(std::string str, int *a, int *b, int m) {
    int n = str.size();
    int *count = new int[2 * m + 1];
    memset(count, 0, (2 * m + 1) * sizeof(int));

    for (int i = 0; i < n; ++i) ++count[str[a[i]]];

    for (int i = 1; i <= m; ++i) count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; --i) b[--count[str[a[i]]]] = a[i];

    delete[] count;
}

void __suffix_array_radix(int *str, int *a, int *b, int n, int m) {
    int *count = new int[2 * m + 1];
    memset(count, 0, (2 * m + 1) * sizeof(int));

    for (int i = 0; i < n; ++i) ++count[str[a[i]]];

    for (int i = 1; i <= m; ++i) count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; --i) b[--count[str[a[i]]]] = a[i];

    delete[] count;
}
}  // namespace detail

namespace ex_string {
std::vector<int> match(std::string pattern, std::string text)
/*
Find the matching position of one string in another.
*/
{
    int n = pattern.size();
    std::vector<int> next(n + 1, 0);

    for (int i = 1; i < n; ++i) {
        int j = i;

        if (pattern[j] == pattern[i]) {
            next[i + 1] = j + 1;
            break;
        }
    }

    std::vector<int> positions;

    int m = text.size();

    for (int i = 0, j = 0; i < m; ++i) {
        if (j < n && text[i] == pattern[j])
            ++j;
        else {
            while (j > 0) {
                j = next[j];

                if (text[i] == pattern[j]) {
                    ++j;
                    break;
                }
            }
        }

        if (j == n) positions.push_back(j);
    }

    return positions;
}

void exKMP(std::string a, std::string b, int *ret)
/*
Find the longest common prefix of a for each suffix of B.
The results are stored in array Next.
*/
{
    int n = a.size(), m = b.size();
    std::vector<int> Next(std::max(n, m) + 1, 0);
    int i, j = 0, k;

    while (j + 1 < m && a[j] == a[j + 1]) ++j;

    Next[1] = j;
    k = 1;

    for (i = 2; i < m; ++i) {
        int len = k + Next[k], L = Next[i - k];

        if (L < len - i)
            Next[i] = L;
        else {
            j = std::max(0, len - i);

            while (i + j < m && a[i] == a[i + j]) ++j;

            Next[i] = j;
            k = i;
        }
    }

    j = 0;

    while (j < n && j < m && a[i] == b[j]) ++j;

    ret[0] = j;
    k = 0;

    for (i = 1; i < n; ++i) {
        int len = k + ret[k], L = Next[i - k];

        if (L < len - i)
            ret[i] = L;
        else {
            j = std::max(0, len - i);

            while (j < m && i + j < n && a[j] == b[i + j]) ++j;

            ret[i] = j;
            k = i;
        }
    }
}

std::string smallestRepresation(std::string s)
/*
Find the smallest representation of the loop string.
*/
{
    int i = 0, j = 1, k, l;
    int n = s.size();

    s += s;

    while (j < n) {
        k = 0;

        while (k < n && s[i + j] == s[j + k]) ++k;

        if (k >= n)
            break;
        else {
            l = i + k;
            i = j;
            j = std::max(l, j) + 1;
        }
    }

    return s.substr(i, n);
}

void suffix_array(std::string str, int *sa)
/*
Find the suffix array of the string.
The result is stored in array suffix.
*/
{
    int m, n = str.size();

    for (int i = 0; i < n; ++i) m = std::max(m, (int)str[i]);

    int *rank = new int[2 * n + 1], *a = new int[2 * n + 1],
        *b = new int[2 * n + 1];
    memset(rank, 0, (2 * n + 1) * sizeof(int));
    memset(a, 0, (2 * n + 1) * sizeof(int));
    memset(b, 0, (2 * n + 1) * sizeof(int));

    for (int i = 0; i < n; ++i) rank[i] = i;

    detail::__suffix_array_radix(str, rank, sa, m);

    rank[sa[0]] = 0;

    for (int i = 1; i < n; ++i)
        rank[sa[i]] = rank[sa[i - 1]] + (str[sa[i]] != str[sa[i - 1]]);

    for (int i = 0; (1 << i) < n; ++i) {
        for (int j = 0; j < n; ++j) {
            a[j] = rank[j] + 1;
            b[j] = j + (1 << i) >= n ? 0 : rank[j + (1 << i)] + 1;
            sa[j] = j;
        }

        detail::__suffix_array_radix(b, sa, rank, n, n);
        detail::__suffix_array_radix(a, rank, sa, n, n);
        rank[sa[0]] = 0;

        for (int j = 1; j < n; ++j) {
            rank[sa[j]] = rank[sa[j - 1]] + (a[sa[j - 1]] != a[sa[j]] ||
                                             b[sa[j - 1]] != b[sa[j]]);
        }
    }

    delete[] rank;
    delete[] a;
    delete[] b;
}

void suffix_array(int *str, int *sa, int n) {
    int m;

    for (int i = 0; i < n; ++i) m = std::max(m, str[i]);

    int *rank = new int[2 * n + 1], *a = new int[2 * n + 1],
        *b = new int[2 * n + 1];
    memset(rank, 0, (2 * n + 1) * sizeof(int));
    memset(a, 0, (2 * n + 1) * sizeof(int));
    memset(b, 0, (2 * n + 1) * sizeof(int));

    for (int i = 0; i < n; ++i) rank[i] = i;

    detail::__suffix_array_radix(str, rank, sa, n, m);

    rank[sa[0]] = 0;

    for (int i = 1; i < n; ++i)
        rank[sa[i]] = rank[sa[i - 1]] + (str[sa[i]] != str[sa[i - 1]]);

    for (int i = 0; (1 << i) < n; ++i) {
        for (int j = 0; j < n; ++j) {
            a[j] = rank[j] + 1;
            b[j] = j + (1 << i) >= n ? 0 : rank[j + (1 << i)] + 1;
            sa[j] = j;
        }

        detail::__suffix_array_radix(b, sa, rank, n, n);
        detail::__suffix_array_radix(a, rank, sa, n, n);
        rank[sa[0]] = 0;

        for (int j = 1; j < n; ++j) {
            rank[sa[j]] = rank[sa[j - 1]] + (a[sa[j - 1]] != a[sa[j]] ||
                                             b[sa[j - 1]] != b[sa[j]]);
        }
    }

    delete[] rank;
    delete[] a;
    delete[] b;
}

void calc_height(std::string str, int *sa, int *h)
/*
Find the height array of the string.
The result is stored in the array h.
Notice:sa is the suffix array that has computed of the string.
h[i] is the longest common prefix of sa[i] and sa[i-1].
*/
{
    int n = str.size();
    int *rank = new int[2 * n + 1];
    memset(rank, 0, (2 * n + 1) * sizeof(int));
    int k = 0;

    h[0] = 0;

    for (int i = 0; i < n; ++i) rank[sa[i]] = i;

    for (int i = 0; i < n; ++i) {
        k = (k == 0 ? 0 : k - 1);

        if (rank[i] != 0)
            while (str[i + k] == str[sa[rank[i] - 1] + k]) ++k;

        h[rank[i]] = k;
    }

    delete[] rank;
}

std::string duplicate_substr(std::string str, short kind)
/*
kind=1:
Find the longest repeated substring of the string.
kind=2:
Find the longest non-overlapping repeated substrings of the string.
*/
{
    std::string rev;
    int n = str.size();
    int *s = new int[2 * n + 1], *sa = new int[2 * n + 1],
        *rank = new int[2 * n + 1], *h = new int[2 * n + 1];
    memset(s, 0, (2 * n + 1) * sizeof(int));
    memset(sa, 0, (2 * n + 1) * sizeof(int));
    memset(rank, 0, (2 * n + 1) * sizeof(int));
    memset(h, 0, (2 * n + 1) * sizeof(int));

    std::copy(str.begin(), str.begin(), s);
    suffix_array(s, sa, n);

    for (int i = 0; i < n; ++i) rank[sa[i]] = i;

    int k = 0;
    int ans1 = 0, pos1 = 0;

    for (int i = 0; i < n; ++i) {
        k = (k == 0 ? 0 : k - 1);

        while (rank[i] > 0 && s[i + k] == s[sa[rank[i] - 1] + k]) ++k;

        h[rank[i]] = k;

        if (h[rank[i]] > ans1) {
            ans1 = h[rank[i]];
            pos1 = i;
        }
    }

    if (kind == 1) return str.substr(pos1, ans1);

    int low = 1, high = n;
    int ans2 = 0, pos21 = 0, pos22 = 0;

    while (low <= high) {
        int mid = (low + high) >> 1;
        bool ok = false;

        for (int i = 0; i < n;) {
            int j = i + 1, minPos = sa[i], maxPos = sa[i];

            while (j < n && h[j] >= mid) {
                minPos = std::min(minPos, sa[j]);
                maxPos = std::max(maxPos, sa[j]);
                ++j;
            }

            if (maxPos - minPos >= mid) {
                ok = true;

                if (mid > ans2) {
                    ans2 = mid;
                    pos21 = minPos;
                    pos22 = maxPos;
                }

                break;
            }

            i = j;
        }

        if (ok)
            low = mid + 1;
        else
            high = mid - 1;
    }

    if (kind == 2) return str.substr(pos21, ans2);

    delete[] s;
    delete[] sa;
    delete[] rank;
    delete[] h;
}

std::string LCS(std::string a, std::string b)
/*
Find the longest common substring of two strings.
*/
{
    std::string str;
    str = a + "#" + b;
    int n = str.size();
    int *s = new int[2 * n + 1], *sa = new int[2 * n + 1],
        *rank = new int[2 * n + 1], *h = new int[2 * n + 1];
    memset(s, 0, (2 * n + 1) * sizeof(int));
    memset(sa, 0, (2 * n + 1) * sizeof(int));
    memset(h, 0, (2 * n + 1) * sizeof(int));
    memset(rank, 0, (2 * n + 1) * sizeof(int));
    std::copy(str.begin(), str.end(), s);

    suffix_array(s, sa, n);

    for (int i = 0; i < n; ++i) rank[sa[i]] = i;

    int curH = 0;

    for (int i = 0; i < n; ++i) {
        curH = (curH == 0 ? 0 : curH - 1);

        if (rank[i] != 0)
            while (str[i + curH] == str[sa[rank[i] - 1] + curH]) ++curH;
        else
            curH = 0;

        h[rank[i]] = curH;
    }

    int ans = 0, pos;

    for (int i = 1; i < n; ++i)
        if (h[i] > ans && (sa[i - 1] < a.size()) != (sa[i] < a.size())) {
            ans = h[i];
            pos = sa[i];
        }

    if (ans == 0)
        return std::string("");
    else
        return str.substr(pos, ans);

    delete[] s;
    delete[] sa;
    delete[] rank;
    delete[] h;
}

int find_palindrome(std::string str)
/*
Find the length of the longest palindrome substring of the string.
*/
{
    int n = str.size();
    int *len = new int[2 * n + 1];
    memset(len, 0, (2 * n + 1) * sizeof(int));
    std::string s;
    s.resize(n * 2 + 2);

    s[0] = s[1] = '#';

    for (int i = 0; i < n; ++i) {
        s[i * 2 + 2] = str[i];
        s[i * 2 + 3] = '#';
    }

    n = n * 2 + 2;
    s[n] = 0;

    int maxright = 0, mid;

    for (int i = 1; i < n; ++i) {
        if (i < maxright)
            len[i] = std::min(len[(mid << 1) - i], len[mid] + mid - i);
        else
            len[i] = 1;

        while (s[i + len[i]] == s[i - len[i]]) ++len[i];

        if (len[i] + i > maxright) {
            maxright = len[i] + i;
            mid = i;
        }
    }

    int ans = 1;

    for (int i = 0; i < n; ++i) ans = std::max(ans, len[i]);

    delete[] len;

    return ans - 1;
}

template <size_t size, const unsigned long long PRIME = 137>
class string_hash
/*
Class for string hashing.
*/
{
   private:
    unsigned long long *h;
    unsigned long long *base;
    int n;

   public:
    string_hash() {
        h = new unsigned long long[size];
        base = new unsigned long long[size];
        memset(h, 0, size * sizeof(unsigned long long));
        memset(base, 0, size * sizeof(unsigned long long));
    }

    string_hash(std::string str) {
        h = new unsigned long long[size];
        base = new unsigned long long[size];
        memset(h, 0, size * sizeof(unsigned long long));
        memset(base, 0, size * sizeof(unsigned long long));
        build(str);
    }

    ~string_hash() {
        delete[] h;
        delete[] base;
    }

    void build(std::string str) {
        n = str.size();
        h[0] = 0;

        for (int i = 1; i <= n; ++i) h[i] = h[i - 1] * PRIME + str[i - 1];

        base[0] = 1;

        for (int i = 1; i <= n; ++i) base[i] = base[i - 1] * PRIME;
    }

    unsigned long long operator()() { return h[n]; }

    unsigned long long operator()(int l, int r)
    /*
    Query the hash value of substring in [l,r].
    */
    {
        return h[r] - h[l] * base[r - l];
    }
};

template <size_t size, const int CHARSET = 26, const int CHARBASE = 'a'>
class Aho_Corasick_Automaton {
   private:
    int **c, *val, *fail, cnt;

   public:
    Aho_Corasick_Automaton() {
        cnt = 0;
        c = new int *[size];

        for (int i = 0; i < size; ++i)
            c[i] = new int[CHARSET], memset(c[i], 0, CHARSET * sizeof(int));

        val = new int[size];
        fail = new int[size];
        memset(fail, 0, size * sizeof(int));
        memset(val, 0, size * sizeof(int));
    }

    ~Aho_Corasick_Automaton() {
        delete[] val;
        delete[] fail;

        for (int i = 0; i < size; ++i) delete[] c[i];

        delete[] c;
    }

    void insert(std::string str)
    /*
    Insert a text string.
    */
    {
        int n = str.size();
        int now = 0;

        for (int i = 0; i < n; ++i) {
            int v = str[i] - CHARBASE;

            if (!c[now][v]) c[now][v] = ++cnt;

            now = c[now][v];
        }

        ++val[now];
    }

    void build() {
        std::queue<int> q;

        for (int i = 0; i < CHARSET; ++i)
            if (c[0][i]) {
                fail[c[0][i]] = 0;
                q.push(c[0][i]);
            }

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int i = 0; i < CHARSET; ++i)
                if (c[u][i]) {
                    fail[c[u][i]] = c[fail[u]][i];
                    q.push(c[u][i]);
                }

                else
                    c[u][i] = c[fail[u]][i];
        }
    }

    int query(std::string str)
    /*
    Find the matching times of text strings in the pattern string.
    */
    {
        int n = str.size();
        int now = 0, ans = 0;

        for (int i = 0; i < n; ++i) {
            now = c[now][str[i] - CHARBASE];

            for (int t = now; t && ~val[t]; t = fail[t]) {
                ans += val[t];
                val[t] = -1;
            }
        }

        return ans;
    }
};

template <size_t size, const int CHARSET = 26, const int CHARBASE = 'a'>
class Suffix_Automaton {
   private:
    int last, cnt;
    int **ch, *fa, *l;

   public:
    Suffix_Automaton() : last(1), cnt(1) {
        ch = new int *[size];

        for (int i = 0; i < size; ++i)
            ch[i] = new int[CHARSET], memset(ch[i], 0, CHARSET * sizeof(int));

        fa = new int[size];
        l = new int[size];
        memset(fa, 0, size * sizeof(fa));
        memset(l, 0, size * sizeof(l));
    }

    ~Suffix_Automaton() {
        for (int i = 0; i < size; ++i) delete[] ch[i];

        delete[] ch;
        delete[] fa;
        delete[] l;
    }

    void extend(int c) {
        int p = last, np = ++cnt;
        last = np;
        l[np] = l[p] + 1;

        for (; p && !ch[p][c]; p = fa[p]) ch[p][c] = np;

        if (!p)
            fa[np] = 1;
        else {
            int q = ch[p][c];

            if (l[p] + 1 == l[q])
                fa[np] = q;
            else {
                int nq = ++cnt;
                l[nq] = l[p] + 1;
                memcpy(ch[nq], ch[q], sizeof(ch[q]));
                fa[nq] = fa[q];
                fa[q] = fa[np] = nq;

                for (; ch[p][c] == q; p = fa[p]) ch[p][c] = nq;
            }
        }
    }

    void build(std::string str) {
        int n = str.size();
        last = cnt = 1;

        for (int i = 0; i < n; i++) extend(str[i] - CHARBASE);
    }

    void progress();
    /*
    Customize.
    */
};

template <size_t size, const int CHARSET = 26, const int CHARBASE = 'a'>
class Palindrome_Automaton {
   private:
    struct Node {
        int *ch;
        int fail, len, num;

        Node() {
            fail = len = num = 0;
            ch = new int[CHARSET];
            memset(ch, 0, CHARSET * sizeof(int));
        }
        ~Node() { delete[] ch; }
    } * b;

    int n, last, cnt, *s;
    char *c;

    int get_fail(int x) {
        while (s[n - b[x].len - 1] != s[n]) x = b[x].fail;

        return x;
    }

    void insert() {
        int p = get_fail(last);

        if (!b[p].ch[s[n]]) {
            b[++cnt].len = b[p].len + 2;

            int tmp = get_fail(b[p].fail);
            b[cnt].fail = b[tmp].ch[s[n]];
            b[cnt].num = b[b[cnt].fail].num + 1;
            b[p].ch[s[n]] = cnt;
        }

        last = b[p].ch[s[n]];
    }

   public:
    Palindrome_Automaton() {
        b = new Node[size]();
        c = new char[size];
        s = new int[size];
        memset(c, 0, size * sizeof(char));
        memset(s, 0, size * sizeof(int));
        b[0].len = 0;
        b[1].len = -1;
        b[0].fail = 1;
        b[1].fail = 0;
        last = 0;
        cnt = 1;
    }

    ~Palindrome_Automaton() {
        delete[] b;
        delete[] c;
        delete[] s;
    }

    std::vector<int> query(std::string str)
    /*
    For each position,
    query the number of palindrome substrings ending at that position.
    */
    {
        s[0] = CHARSET;
        std::vector<int> ans;

        for (n = 1; n <= str.size(); ++n) {
            s[n] = str[n - 1] - CHARBASE;
            insert();
            ans.push_back(b[last].num);
        }

        b[0].len = 0;
        b[1].len = -1;
        b[0].fail = 1;
        b[1].fail = 0;
        last = 0;
        cnt = 1;

        return ans;
    }
};
}  // namespace ex_string
}  // namespace Octinc

#endif
