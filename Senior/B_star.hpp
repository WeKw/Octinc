/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_B_STAR_HPP
#  define OCTINC_B_STAR_HPP
#  ifndef STD_BITS_STDCPP_H
#    define STD_BITS_STDCPP_H
#    include <bits/stdc++.h>
#  endif

namespace Octinc {
template <size_t SIZE>
/*
SIZE is the maximum size of the rectangle.
*/
class B_star {
   private:
    struct Point;

    struct Point {
        int x, y, step;
        short WD, D;

        Point(int x = 0, int y = 0, int step = 0, short WD = 0, short D = 0)
            : x(x), y(y), step(step), WD(WD), D(D) {}

        const bool operator==(Point other) {
            return x == other.x && y == other.y;
        }

        void Walk(short dire) {
            constexpr short direx[] = {1, 0, -1, 0};
            constexpr short direy[] = {0, 1, 0, -1};
            x += direx[dire], y += direy[dire];
        }

        Point Next(short dire) {
            constexpr short direx[] = {1, 0, -1, 0};
            constexpr short direy[] = {0, 1, 0, -1};
            return Point(x + direx[dire], y + direy[dire], step, WD);
        }
    } start, goal;

    bool mapn[SIZE + 5][SIZE + 5], vis[SIZE + 5][SIZE + 5];
    std::queue<Point> que;

    int n, m;

    short TurnLeft(short dire) { return (dire == 0 ? 3 : dire - 1); }

    short TurnRight(short dire) { return (dire == 3 ? 0 : dire + 1); }

    bool NearWall(Point x) {
        for (short i = 0; i < 4; ++i) {
            Point rear = x;
            rear.Walk(i);

            if (mapn[rear.y][rear.x]) return true;
        }

        return false;
    }

    bool Allowed(Point x) { return !mapn[x.y][x.x] && !vis[x.y][x.x]; }

    bool AtWall(Point x) { return mapn[x.y][x.x]; }

    short Straight(Point x) {
        if (abs(x.x - goal.x) >= abs(x.y - goal.y)) {
            if (x.x < goal.x) return 0;
            if (x.x < goal.x) return 2;
        }

        if (x.y < goal.y) return 1;

        return 3;
    }

   public:
    B_star(int _n = 0, int _m = 0) {
        set_range(_n, _m);
        memset(mapn, 0, sizeof(mapn));
        memset(vis, 0, sizeof(vis));
    }

    void set_range(int _n, int _m)
    /*
    Limit the range to a n*m rectangle.
    */
    {
        n = _n, m = _m;
    }

    void set_wall(int x, int y)
    /*
    Let point (x,y) be a wall.
    */
    {
        mapn[x][y] = 1;
    }

    void clear()
    /*
    Clear the map.
    */
    {
        memset(mapn, 0, sizeof(mapn));
        memset(vis, 0, sizeof(vis));
    }

    int find_path(int _sx, int _sy, int _ex, int _ey)
    /*
    Find the shortest path steps from the beginning point (_sx,_sy) to the end
    point (_ex,_ey). Path Finding Algorithm:B*(B star),also known as improved
    A*. Returning -1 means that there's no way between two points.
    */
    {
        start = Point(_sx, _sy);
        goal = Point(_ex, _ey);

        start.step = 0, start.WD = start.D = 4;
        que.push(start);

        while (!que.empty()) {
            Point now = que.front();
            que.pop();

            if (now == goal) return now.step;

            if (!Allowed(now)) continue;

            vis[now.y][now.x] = 1;

            if (abs(now.x - goal.x) >= abs(now.y - goal.y)) {
                if (now.x < goal.x && Allowed(now.Next(0))) {
                    Point rear;
                    rear = now.Next(0), rear.Walk(TurnLeft(0));

                    if (AtWall(rear))
                        rear = now.Next(TurnLeft(0)), rear.WD = rear.D = 4,
                        ++rear.step, que.push(rear);

                    rear = now.Next(0), rear.Walk(TurnRight(0));

                    if (AtWall(rear))
                        rear = now.Next(TurnRight(0)), rear.WD = rear.D = 4,
                        ++rear.step, que.push(rear);

                    rear = now.Next(0);
                    ++rear.step, rear.WD = rear.D = 4;
                    que.push(rear);

                    continue;
                }

                if (now.x > goal.x && Allowed(now.Next(2))) {
                    Point rear;
                    rear = now.Next(2), rear.Walk(TurnLeft(2));

                    if (AtWall(rear))
                        rear = now.Next(TurnLeft(2)), rear.WD = rear.D = 4,
                        ++rear.step, que.push(rear);

                    rear = now.Next(2), rear.Walk(TurnRight(2));

                    if (AtWall(rear))
                        rear = now.Next(TurnRight(2)), rear.WD = rear.D = 4,
                        ++rear.step, que.push(rear);

                    rear = now.Next(2);
                    ++rear.step, rear.WD = rear.D = 4;
                    que.push(rear);

                    continue;
                }
            } else {
                if (now.y < goal.y && Allowed(now.Next(1))) {
                    Point rear;
                    rear = now.Next(1), rear.Walk(TurnLeft(1));

                    if (AtWall(rear))
                        rear = now.Next(TurnLeft(1)), rear.WD = rear.D = 4,
                        ++rear.step, que.push(rear);

                    rear = now.Next(1), rear.Walk(TurnRight(1));

                    if (AtWall(rear))
                        rear = now.Next(TurnRight(1)), rear.WD = rear.D = 4,
                        ++rear.step, que.push(rear);

                    rear = now.Next(1);
                    ++rear.step, rear.WD = rear.D = 4;
                    que.push(rear);

                    continue;
                }

                if (now.y > goal.y && Allowed(now.Next(3))) {
                    Point rear;
                    rear = now.Next(3), rear.Walk(TurnLeft(3));

                    if (AtWall(rear))
                        rear = now.Next(TurnLeft(3)), rear.WD = rear.D = 4,
                        ++rear.step, que.push(rear);

                    rear = now.Next(3), rear.Walk(TurnRight(3));

                    if (AtWall(rear))
                        rear = now.Next(TurnRight(3)), rear.WD = rear.D = 4,
                        ++rear.step, que.push(rear);

                    rear = now.Next(3);
                    ++rear.step, rear.WD = rear.D = 4;
                    que.push(rear);

                    continue;
                }
            }

            if (now.WD == 4 && AtWall(now.Next(Straight(now)))) {
                if (Straight(now) == 0) {
                    Point rear;

                    rear = now.Next(1), rear.Walk(TurnLeft(1));

                    if (AtWall(rear))
                        rear = now.Next(TurnLeft(1)), rear.WD = 0, rear.D = 1,
                        ++rear.step, que.push(rear);

                    rear = now.Next(1), rear.Walk(TurnRight(1));

                    if (AtWall(rear))
                        rear = now.Next(TurnRight(1)), rear.WD = 0, rear.D = 1,
                        ++rear.step, que.push(rear);

                    rear = now.Next(1), rear.D = 1, ++rear.step, rear.WD = 0,
                    que.push(rear);

                    rear = now.Next(3), rear.Walk(TurnLeft(3));

                    if (AtWall(rear))
                        rear = now.Next(TurnLeft(3)), rear.WD = 0, rear.D = 3,
                        ++rear.step, que.push(rear);

                    rear = now.Next(3), rear.Walk(TurnRight(3));

                    if (AtWall(rear))
                        rear = now.Next(TurnRight(3)), rear.WD = 0, rear.D = 3,
                        ++rear.step, que.push(rear);

                    rear = now.Next(3), rear.D = 3, ++rear.step, rear.WD = 0,
                    que.push(rear);

                    continue;
                }

                if (Straight(now) == 1) {
                    Point rear;

                    rear = now.Next(0), rear.Walk(TurnLeft(0));

                    if (AtWall(rear))
                        rear = now.Next(TurnLeft(0)), rear.WD = 1, rear.D = 0,
                        ++rear.step, que.push(rear);

                    rear = now.Next(0), rear.Walk(TurnRight(0));

                    if (AtWall(rear))
                        rear = now.Next(TurnRight(0)), rear.WD = 1, rear.D = 0,
                        ++rear.step, que.push(rear);

                    rear = now.Next(0), rear.D = 0, ++rear.step, rear.WD = 1,
                    que.push(rear);

                    rear = now.Next(2), rear.Walk(TurnLeft(2));

                    if (AtWall(rear))
                        rear = now.Next(TurnLeft(2)), rear.WD = 1, rear.D = 2,
                        ++rear.step, que.push(rear);

                    rear = now.Next(2), rear.Walk(TurnRight(2));

                    if (AtWall(rear))
                        rear = now.Next(TurnRight(2)), rear.WD = 1, rear.D = 2,
                        ++rear.step, que.push(rear);

                    rear = now.Next(2), rear.D = 2, ++rear.step, rear.WD = 1,
                    que.push(rear);

                    continue;
                }

                if (Straight(now) == 2) {
                    Point rear;

                    rear = now.Next(1), rear.Walk(TurnLeft(1));

                    if (AtWall(rear))
                        rear = now.Next(TurnLeft(1)), rear.WD = 2, rear.D = 1,
                        ++rear.step, que.push(rear);

                    rear = now.Next(1), rear.Walk(TurnRight(1));

                    if (AtWall(rear))
                        rear = now.Next(TurnRight(1)), rear.WD = 2, rear.D = 1,
                        ++rear.step, que.push(rear);

                    rear = now.Next(1), rear.D = 1, ++rear.step, rear.WD = 2,
                    que.push(rear);

                    rear = now.Next(3), rear.Walk(TurnLeft(3));

                    if (AtWall(rear))
                        rear = now.Next(TurnLeft(3)), rear.WD = 2, rear.D = 3,
                        ++rear.step, que.push(rear);

                    rear = now.Next(3), rear.Walk(TurnRight(3));

                    if (AtWall(rear))
                        rear = now.Next(TurnRight(3)), rear.WD = 2, rear.D = 3,
                        ++rear.step, que.push(rear);

                    rear = now.Next(3), rear.D = 3, ++rear.step, rear.WD = 2,
                    que.push(rear);

                    continue;
                }

                if (Straight(now) == 3) {
                    Point rear;

                    rear = now.Next(0), rear.Walk(TurnLeft(0));

                    if (AtWall(rear))
                        rear = now.Next(TurnLeft(0)), rear.WD = 3, rear.D = 0,
                        ++rear.step, que.push(rear);

                    rear = now.Next(0), rear.Walk(TurnRight(0));

                    if (AtWall(rear))
                        rear = now.Next(TurnRight(0)), rear.WD = 3, rear.D = 0,
                        ++rear.step, que.push(rear);

                    rear = now.Next(0), rear.D = 0, ++rear.step, rear.WD = 3,
                    que.push(rear);

                    rear = now.Next(2), rear.Walk(TurnLeft(2));

                    if (AtWall(rear))
                        rear = now.Next(TurnLeft(2)), rear.WD = 3, rear.D = 2,
                        ++rear.step, que.push(rear);

                    rear = now.Next(2), rear.Walk(TurnRight(2));

                    if (AtWall(rear))
                        rear = now.Next(TurnRight(2)), rear.WD = 3, rear.D = 2,
                        ++rear.step, que.push(rear);

                    rear = now.Next(2), rear.D = 2, ++rear.step, rear.WD = 3,
                    que.push(rear);

                    continue;
                }
            } else {
                if (now.WD == 0) {
                    if (!AtWall(now.Next(0))) {
                        if (now.D == 1) {
                            Point rear;

                            rear = now.Next(0), rear.Walk(TurnLeft(0));

                            if (AtWall(rear))
                                rear = now.Next(TurnLeft(0)), rear.WD = 3,
                                rear.D = 0, ++rear.step, que.push(rear);

                            rear = now.Next(0), rear.Walk(TurnRight(0));

                            if (AtWall(rear))
                                rear = now.Next(TurnRight(0)), rear.WD = 3,
                                rear.D = 0, ++rear.step, que.push(rear);

                            rear = now.Next(0), rear.D = 0, ++rear.step,
                            rear.WD = 3, que.push(rear);

                            continue;
                        }
                        if (now.D == 3) {
                            Point rear;

                            rear = now.Next(0), rear.Walk(TurnLeft(0));

                            if (AtWall(rear))
                                rear = now.Next(TurnLeft(0)), rear.WD = 1,
                                rear.D = 0, ++rear.step, que.push(rear);

                            rear = now.Next(0), rear.Walk(TurnRight(0));

                            if (AtWall(rear))
                                rear = now.Next(TurnRight(0)), rear.WD = 1,
                                rear.D = 0, ++rear.step, que.push(rear);

                            rear = now.Next(0), rear.D = 0, ++rear.step,
                            rear.WD = 1, que.push(rear);

                            continue;
                        }
                    }

                    if (!AtWall(now.Next(now.D))) {
                        Point rear;

                        rear = now.Next(now.D), rear.Walk(TurnLeft(now.D));

                        if (AtWall(rear))
                            rear = now.Next(TurnLeft(now.D)), rear.WD = 0,
                            rear.D = now.D, ++rear.step, que.push(rear);

                        rear = now.Next(now.D), rear.Walk(TurnRight(now.D));

                        if (AtWall(rear))
                            rear = now.Next(TurnRight(now.D)), rear.WD = 0,
                            rear.D = now.D, ++rear.step, que.push(rear);

                        rear = now.Next(now.D), rear.D = now.D, ++rear.step,
                        rear.WD = 0, que.push(rear);

                        continue;
                    }

                    Point rear;

                    rear = now.Next(2), rear.Walk(TurnLeft(2));

                    if (AtWall(rear))
                        rear = now.Next(TurnLeft(2)), rear.WD = now.D,
                        rear.D = 2, ++rear.step, que.push(rear);

                    rear = now.Next(2), rear.Walk(TurnRight(2));

                    if (AtWall(rear))
                        rear = now.Next(TurnRight(2)), rear.WD = now.D,
                        rear.D = 2, ++rear.step, que.push(rear);

                    rear = now.Next(2), rear.D = 2, ++rear.step,
                    rear.WD = now.D, que.push(rear);

                    continue;
                }
                if (now.WD == 1) {
                    if (!AtWall(now.Next(1))) {
                        if (now.D == 0) {
                            Point rear;

                            rear = now.Next(1), rear.Walk(TurnLeft(1));

                            if (AtWall(rear))
                                rear = now.Next(TurnLeft(1)), rear.WD = 2,
                                rear.D = 1, ++rear.step, que.push(rear);

                            rear = now.Next(1), rear.Walk(TurnRight(1));

                            if (AtWall(rear))
                                rear = now.Next(TurnRight(1)), rear.WD = 2,
                                rear.D = 1, ++rear.step, que.push(rear);

                            rear = now.Next(1), rear.D = 1, ++rear.step,
                            rear.WD = 2, que.push(rear);

                            continue;
                        }

                        if (now.D == 2) {
                            Point rear;

                            rear = now.Next(1), rear.Walk(TurnLeft(1));

                            if (AtWall(rear))
                                rear = now.Next(TurnLeft(1)), rear.WD = 0,
                                rear.D = 1, ++rear.step, que.push(rear);

                            rear = now.Next(1), rear.Walk(TurnRight(1));

                            if (AtWall(rear))
                                rear = now.Next(TurnRight(1)), rear.WD = 0,
                                rear.D = 1, ++rear.step, que.push(rear);

                            rear = now.Next(1), rear.D = 1, ++rear.step,
                            rear.WD = 0, que.push(rear);

                            continue;
                        }
                    }

                    if (!AtWall(now.Next(now.D))) {
                        Point rear;

                        rear = now.Next(now.D), rear.Walk(TurnLeft(now.D));

                        if (AtWall(rear))
                            rear = now.Next(TurnLeft(now.D)), rear.WD = 1,
                            rear.D = now.D, ++rear.step, que.push(rear);

                        rear = now.Next(now.D), rear.Walk(TurnRight(now.D));

                        if (AtWall(rear))
                            rear = now.Next(TurnRight(now.D)), rear.WD = 1,
                            rear.D = now.D, ++rear.step, que.push(rear);

                        rear = now.Next(now.D), rear.D = now.D, ++rear.step,
                        rear.WD = 1, que.push(rear);

                        continue;
                    }

                    Point rear;

                    rear = now.Next(3), rear.Walk(TurnLeft(3));

                    if (AtWall(rear))
                        rear = now.Next(TurnLeft(3)), rear.WD = now.D,
                        rear.D = 3, ++rear.step, que.push(rear);

                    rear = now.Next(3), rear.Walk(TurnRight(3));

                    if (AtWall(rear))
                        rear = now.Next(TurnRight(3)), rear.WD = now.D,
                        rear.D = 3, ++rear.step, que.push(rear);

                    rear = now.Next(3), rear.D = 3, ++rear.step,
                    rear.WD = now.D, que.push(rear);

                    continue;
                }

                if (now.WD == 2) {
                    if (!AtWall(now.Next(2))) {
                        if (now.D == 1) {
                            Point rear;

                            rear = now.Next(2), rear.Walk(TurnLeft(2));

                            if (AtWall(rear))
                                rear = now.Next(TurnLeft(2)), rear.WD = 3,
                                rear.D = 2, ++rear.step, que.push(rear);

                            rear = now.Next(2), rear.Walk(TurnRight(2));

                            if (AtWall(rear))
                                rear = now.Next(TurnRight(2)), rear.WD = 3,
                                rear.D = 2, ++rear.step, que.push(rear);

                            rear = now.Next(2), rear.D = 2, ++rear.step,
                            rear.WD = 3, que.push(rear);

                            continue;
                        }

                        if (now.D == 3) {
                            Point rear;

                            rear = now.Next(2), rear.Walk(TurnLeft(2));

                            if (AtWall(rear))
                                rear = now.Next(TurnLeft(2)), rear.WD = 1,
                                rear.D = 2, ++rear.step, que.push(rear);

                            rear = now.Next(2), rear.Walk(TurnRight(2));

                            if (AtWall(rear))
                                rear = now.Next(TurnRight(2)), rear.WD = 1,
                                rear.D = 2, ++rear.step, que.push(rear);

                            rear = now.Next(2), rear.D = 2, ++rear.step,
                            rear.WD = 1, que.push(rear);

                            continue;
                        }
                    }

                    if (!AtWall(now.Next(now.D))) {
                        Point rear;

                        rear = now.Next(now.D), rear.Walk(TurnLeft(now.D));

                        if (AtWall(rear))
                            rear = now.Next(TurnLeft(now.D)), rear.WD = 2,
                            rear.D = now.D, ++rear.step, que.push(rear);

                        rear = now.Next(now.D), rear.Walk(TurnRight(now.D));

                        if (AtWall(rear))
                            rear = now.Next(TurnRight(now.D)), rear.WD = 2,
                            rear.D = now.D, ++rear.step, que.push(rear);

                        rear = now.Next(now.D), rear.D = now.D, ++rear.step,
                        rear.WD = 2, que.push(rear);

                        continue;
                    }

                    Point rear;

                    rear = now.Next(0), rear.Walk(TurnLeft(0));

                    if (AtWall(rear))
                        rear = now.Next(TurnLeft(0)), rear.WD = now.D,
                        rear.D = 0, ++rear.step, que.push(rear);

                    rear = now.Next(0), rear.Walk(TurnRight(0));

                    if (AtWall(rear))
                        rear = now.Next(TurnRight(0)), rear.WD = now.D,
                        rear.D = 0, ++rear.step, que.push(rear);

                    rear = now.Next(0), rear.D = 0, ++rear.step,
                    rear.WD = now.D, que.push(rear);

                    continue;
                }
                if (now.WD == 3) {
                    if (!AtWall(now.Next(3))) {
                        if (now.D == 0) {
                            Point rear;

                            rear = now.Next(3), rear.Walk(TurnLeft(3));

                            if (AtWall(rear))
                                rear = now.Next(TurnLeft(3)), rear.WD = 2,
                                rear.D = 3, ++rear.step, que.push(rear);

                            rear = now.Next(3), rear.Walk(TurnRight(3));

                            if (AtWall(rear))
                                rear = now.Next(TurnRight(3)), rear.WD = 2,
                                rear.D = 3, ++rear.step, que.push(rear);

                            rear = now.Next(3), rear.D = 3, ++rear.step,
                            rear.WD = 2, que.push(rear);

                            continue;
                        }
                        if (now.D == 2) {
                            Point rear;

                            rear = now.Next(3), rear.Walk(TurnLeft(3));

                            if (AtWall(rear))
                                rear = now.Next(TurnLeft(3)), rear.WD = 0,
                                rear.D = 3, ++rear.step, que.push(rear);

                            rear = now.Next(3), rear.Walk(TurnRight(3));

                            if (AtWall(rear))
                                rear = now.Next(TurnRight(3)), rear.WD = 0,
                                rear.D = 3, ++rear.step, que.push(rear);

                            rear = now.Next(3), rear.D = 3, ++rear.step,
                            rear.WD = 0, que.push(rear);

                            continue;
                        }
                    }

                    if (!AtWall(now.Next(now.D))) {
                        Point rear;

                        rear = now.Next(now.D), rear.Walk(TurnLeft(now.D));

                        if (AtWall(rear))
                            rear = now.Next(TurnLeft(now.D)), rear.WD = 3,
                            rear.D = now.D, ++rear.step, que.push(rear);

                        rear = now.Next(now.D), rear.Walk(TurnRight(now.D));

                        if (AtWall(rear))
                            rear = now.Next(TurnRight(now.D)), rear.WD = 3,
                            rear.D = now.D, ++rear.step, que.push(rear);

                        rear = now.Next(now.D), rear.D = now.D, ++rear.step,
                        rear.WD = 3, que.push(rear);

                        continue;
                    }

                    Point rear;

                    rear = now.Next(1), rear.Walk(TurnLeft(1));

                    if (AtWall(rear))
                        rear = now.Next(TurnLeft(1)), rear.WD = now.D,
                        rear.D = 1, ++rear.step, que.push(rear);

                    rear = now.Next(1), rear.Walk(TurnRight(1));

                    if (AtWall(rear))
                        rear = now.Next(TurnRight(1)), rear.WD = now.D,
                        rear.D = 1, ++rear.step, que.push(rear);

                    rear = now.Next(1), rear.D = 1, ++rear.step,
                    rear.WD = now.D, que.push(rear);

                    continue;
                }
            }
        }

        return -1;
    }
};
}  // namespace Octinc

#endif
