/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_DANCING_LINKS__
#define OCTINC_DANCING_LINKS__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <cstdio>
#include <cstring>
#include <vector>

namespace Octinc {
template <const size_t MAXSIZE>
class dancing_links {
   private:
    struct Node {
        int r, c;
        Node *U, *D, *L, *R;

        Node(int r = 0, int c = 0, Node *U = nullptr, Node *D = nullptr,
             Node *L = nullptr, Node *R = nullptr)
            : r(r), c(c), U(U), D(D), L(L), R(R) {}
    };
    Node *node;
    Node *row;
    Node *col;
    Node head;
    int *sz;
    int cnt;
    int n, m;

    inline void delete_row(Node *ptr) {
        ptr->L->R = ptr->R;
        ptr->R->L = ptr->L;
    }

    inline void delete_column(Node *ptr) {
        ptr->U->D = ptr->D;
        ptr->D->U = ptr->U;
    }

    inline void reinstate_row(Node *ptr) { ptr->L->R = ptr->R->L = ptr; }

    inline void reinstate_column(Node *ptr) { ptr->U->D = ptr->D->U = ptr; }

    inline void cover(int c) {
        if (c == n) return;

        delete_row(&col[c]);
        Node *R, *C;

        for (C = col[c].D; C != (&col[c]); C = C->D) {
            if (C->c == n) continue;

            for (R = C->L; R != C; R = R->L) {
                if (R->c == n) continue;

                --sz[R->c];
                delete_column(R);
            }

            delete_row(C);
        }
    }

    inline void reinstate(int c) {
        if (c == n) return;

        Node *R, *C;

        for (C = col[c].U; C != (&col[c]); C = C->U) {
            if (C->c == n) continue;

            reinstate_row(C);

            for (R = C->R; R != C; R = R->R) {
                if (R->c == n) continue;

                ++sz[R->c];
                reinstate_column(R);
            }
        }

        reinstate_row(&col[c]);
    }

   public:
    dancing_links() : cnt(0) {
        node = new Node[MAXSIZE * MAXSIZE];
        row = new Node[MAXSIZE];
        col = new Node[MAXSIZE];
        sz = new int[MAXSIZE];
        memset(sz, 0, sizeof(int) * MAXSIZE);
    }

    dancing_links(int r, int c) {
        node = new Node[MAXSIZE * MAXSIZE];
        row = new Node[MAXSIZE];
        col = new Node[MAXSIZE];
        sz = new int[MAXSIZE];
        memset(sz, 0, sizeof(int) * MAXSIZE);
        build(r, c);
    }

    void build(int r, int c)
    /*
    Initialize the graph with row r,column c.
    */
    {
        m = r, n = c;
        cnt = 0;
        head.r = r;
        head.c = c;
        head.L = head.R = head.U = head.D = &head;

        for (int i = 0; i < c; ++i) {
            col[i].r = r;
            col[i].c = i;
            col[i].L = &head;
            col[i].R = head.R;
            col[i].L->R = col[i].R->L = &col[i];
            col[i].U = col[i].D = &col[i];
            sz[i] = 0;
        }

        for (int i = r - 1; i >= 0; --i) {
            row[i].r = i;
            row[i].c = c;
            row[i].U = &head;
            row[i].D = head.D;
            row[i].U->D = row[i].D->U = &row[i];
            row[i].L = row[i].R = &row[i];
        }
    }

    void insert(int r, int c)
    /*
    Insert a node at (r,c) denotes the position is 1.
    */
    {
        Node *ptr = &node[cnt++];
        ptr->r = r;
        ptr->c = c;
        ptr->R = &row[r];
        ptr->L = row[r].L;
        ptr->L->R = ptr->R->L = ptr;
        ptr->U = &col[c];
        ptr->D = col[c].D;
        ptr->U->D = ptr->D->U = ptr;
        ++sz[c];
    }

    bool dance(int ans[], int k = 0)
    /*
    Solve the Exact Cover Problem and store the solution in ans.
    */
    {
        if (head.L == (&head)) return true;

        int INF = (1 << 30), c = -1;

        for (Node *ptr = head.L; ptr != (&head); ptr = ptr->L)
            if (sz[ptr->c] < INF) {
                INF = sz[ptr->c];
                c = ptr->c;
            }

        cover(c);
        Node *ptr;

        for (ptr = col[c].D; ptr != (&col[c]); ptr = ptr->D) {
            Node *rc;
            ptr->R->L = ptr;

            for (rc = ptr->L; rc != ptr; rc = rc->L) cover(rc->c);

            ptr->R->L = ptr->L;
            ans[k] = ptr->r + 1;

            if (dance(ans, k + 1)) return true;

            ptr->L->R = ptr;

            for (rc = ptr->R; rc != ptr; rc = rc->R) reinstate(rc->c);

            ptr->L->R = ptr->R;
        }

        reinstate(c);
        return false;
    }
};
}  // namespace Octinc

#endif
