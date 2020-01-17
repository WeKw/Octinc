/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_GEOMETRY_HPP
#  define OCTINC_GEOMETRY_HPP
#  ifndef STD_BITS_STDCPP_H
#    define STD_BITS_STDCPP_H
#    include <bits/stdc++.h>
#  endif

namespace Octinc {
namespace geometry {
const int MAX_N = 100;     // The maximum number of points in a polygon.
const double INF = 1e10;   // Infinity
const double EPS = 1e-15;  // Computational accuracy (machine infinitesimal)
const double PI = acos(-1.0);
const int LEFT = 0;         // The point is on the left side of the line.
const int RIGHT = 1;        // The point is on the right side of the line.
const int ONLINE = 2;       // The point is on the line.
const int CROSS = 0;        // Two straight lines intersect.
const int COLINE = 1;       // Two straight lines are collinear.
const int PARALLEL = 2;     // Two straight lines are parallel.
const int NOTCOPLANAR = 3;  // Two straight lines are not coplanar.
const int INSIDE = 1;       // Points are inside the graph.
const int OUTSIDE = 2;      // Points are outside the graph.
const int BORDER = 3;       // Points are on the graph boundary.
const int CONTAIN = 1;      // CONTAIN=1:Big circle contains small circle.
const int INTERTAN = 2;     // Internally tangent.
const int INTERSECT = 3;    // Intersect.
const int EXTERTAN = 4;     // Externally tangent.
const int DISJOINT = 5;     // Disjoints.

struct Point
/*
Two-dimensional vector.
*/
{
    double x, y;
    double angle, dis;
    Point() {}
    Point(double x0, double y0) : x(x0), y(y0) {}
};
struct Point3D
/*
Three-dimensional vector.
*/
{
    double x, y, z;
    Point3D() {}
    Point3D(double x0, double y0, double z0) : x(x0), y(y0), z(z0) {}
};
struct Line
/*
Two-dimensional line or segment.
*/
{
    Point p1, p2;
    Line() {}
    Line(Point p10, Point p20) : p1(p10), p2(p20) {}
};
struct Line3D
/*
Three-dimensional line or segment.
*/
{
    Point3D p1, p2;
    Line3D() {}
    Line3D(Point3D p10, Point3D p20) : p1(p10), p2(p20) {}
};
struct Rect
/*
The length and width are used to represent the rectangle,
and the width and height are respectively represented by W and H.
*/
{
    double w, h;
    Rect() {}
    Rect(double _w, double _h) : w(_w), h(_h) {}
};
struct Rect_2
/*
Represents a rectangle.
The lower left coordinate is (xl, yl)
and the upper right coordinate is (xh, yh).
*/
{
    double xl, yl, xh, yh;
    Rect_2() {}
    Rect_2(double _xl, double _yl, double _xh, double _yh)
        : xl(_xl), yl(_yl), xh(_xh), yh(_yh) {}
};
struct Circle  // Circle
{
    Point c;
    double r;
    Circle() {}
    Circle(Point _c, double _r) : c(_c), r(_r) {}
};
typedef std::vector<Point> Polygon;     // Two-dimensional polygon
typedef std::vector<Point> Points;      // Two-dimensional point set
typedef std::vector<Point3D> Points3D;  // Three-dimensional point set

inline bool ZERO(double x)  // x == 0
{
    return (fabs(x) < EPS);
}
inline bool ZERO(Point p)  // p == 0
{
    return (ZERO(p.x) && ZERO(p.y));
}
inline bool ZERO(Point3D p)  // p == 0
{
    return (ZERO(p.x) && ZERO(p.y) && ZERO(p.z));
}
inline bool Equal(double x, double y)
/*
Equal, x == y
*/
{
    return (fabs(x - y) < EPS);
}
inline bool NotEqual(double x, double y)
/*
Not equal, x != y
*/
{
    return (fabs(x - y) >= EPS);
}
inline bool LessThan(double x, double y)
/*
Less than, x < y
*/
{
    return (NotEqual(x, y) && (x < y));
}
inline bool GreaterThan(double x, double y)
/*
Greater than, x > y
*/
{
    return (NotEqual(x, y) && (x > y));
}
inline bool LessEqual(double x, double y)
/*
Less equal, x <= y
*/
{
    return (Equal(x, y) || (x < y));
}
inline bool GreaterEqual(double x, double y)
/*
Greater equal, x >= y
*/
{
    return (Equal(x, y) || (x > y));
}

inline double FIX(double x)
/*
Float fix.
*/
{
    return (fabs(x) < EPS) ? 0 : x;
}

bool operator==(Point p1, Point p2) {
    return (Equal(p1.x, p2.x) && Equal(p1.y, p2.y));
}
bool operator!=(Point p1, Point p2) {
    return (NotEqual(p1.x, p2.x) || NotEqual(p1.y, p2.y));
}
bool operator<(Point p1, Point p2) {
    if (NotEqual(p1.x, p2.x)) {
        return (p1.x < p2.x);
    } else {
        return (p1.y < p2.y);
    }
}
Point operator+(Point p1, Point p2) { return Point(p1.x + p2.x, p1.y + p2.y); }
Point operator-(Point p1, Point p2) { return Point(p1.x - p2.x, p1.y - p2.y); }
double operator*(Point p1, Point p2)
/*
Calculate the cross product p1��p2.
*/
{
    return (p1.x * p2.y - p2.x * p1.y);
}
double operator&(Point p1, Point p2)
/*
Calculate the dot product p1��p2.
*/
{
    return (p1.x * p2.x + p1.y * p2.y);
}
double Norm(Point p)
/*
Length of the vector.
*/
{
    return sqrt(p.x * p.x + p.y * p.y);
}

Point Rotate(Point p, double angle)
/*
Let the vector p rotate at an angle(radian representation).
angle>0 means counterclockwise rotation.
angle<0 means clockwise rotation.
*/
{
    Point result;
    result.x = p.x * cos(angle) - p.y * sin(angle);
    result.y = p.x * sin(angle) + p.y * cos(angle);
    return result;
}

bool operator==(Point3D p1, Point3D p2) {
    return (Equal(p1.x, p2.x) && Equal(p1.y, p2.y) && Equal(p1.z, p2.z));
}
bool operator<(Point3D p1, Point3D p2) {
    if (NotEqual(p1.x, p2.x)) {
        return (p1.x < p2.x);
    } else if (NotEqual(p1.y, p2.y)) {
        return (p1.y < p2.y);
    } else {
        return (p1.z < p2.z);
    }
}
Point3D operator+(Point3D p1, Point3D p2) {
    return Point3D(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
}
Point3D operator-(Point3D p1, Point3D p2) {
    return Point3D(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}
Point3D operator*(Point3D p1, Point3D p2)
/*Calculate the cross product p1 x p2.*/
{
    return Point3D(p1.y * p2.z - p1.z * p2.y, p1.z * p2.x - p1.x * p2.z,
                   p1.x * p2.y - p1.y * p2.x);
}
double operator&(Point3D p1, Point3D p2)
/*Calculate the dot product p1��p2.*/
{
    return (p1.x * p2.x + p1.y * p2.y + p1.z * p2.z);
}
double Norm(Point3D p)
/*
Normal of three-dimensional vector.
*/
{
    return sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
}

double Area(Point A, Point B, Point C)
/*
Calculate the area of the triangle.
The symbol of area is determined according to the right-handed rotation rule.
*/
{
    return ((B - A) * (C - A) / 2.0);
}

double Area(double a, double b, double c)
/*
Calculate the area of the triangle by the side lengths of the triangle.
*/
{
    double s = (a + b + c) / 2.0;
    return sqrt(s * (s - a) * (s - b) * (s - c));
}

double Area(const Circle& C)
/*
Calculate the area of the circle.
*/
{
    return PI * C.r * C.r;
}

double Area(const Polygon& poly)
/*
Calculate the area of the polygon.
The symbol of area is determined according to the right-handed rotation rule.
*/
{
    double res = 0;
    int n = poly.size();
    if (n < 3) return 0;
    for (int i = 0; i < n; i++) {
        res += poly[i].x * poly[(i + 1) % n].y;
        res -= poly[i].y * poly[(i + 1) % n].x;
    }
    return (res / 2.0);
}

double Distance(Point p1, Point p2)
/*
Query the distance from the point to the straight line in two-dimensional space.
*/
{
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}
double Distance(Point3D p1, Point3D p2)
/*
Query the distance from the point to the straight line in three-dimensional
space.
*/
{
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) +
                (p1.z - p2.z) * (p1.z - p2.z));
}
double Distance(Point p, Line L)
/*
Query the distance from the point to the straight line in two-dimensional plane.
*/
{
    return (fabs((p - L.p1) * (L.p2 - L.p1)) / Norm(L.p2 - L.p1));
}
double Distance(Point3D p, Line3D L)
/*
Query the distance from the point to the straight line in three-dimensional
space.
*/
{
    return (Norm((p - L.p1) * (L.p2 - L.p1)) / Norm(L.p2 - L.p1));
}
bool OnLine(Point p, Line L)
/*
Determine whether the point P in two-dimensional plane is on the straight line
L.
*/
{
    return ZERO((p - L.p1) * (L.p2 - L.p1));
}
bool OnLine(Point3D p, Line3D L)
/*
Determine whether the point P in three-dimensional space is on the straight line
L.
*/
{
    return ZERO((p - L.p1) * (L.p2 - L.p1));
}

int Relation(Point p, Line L)
/*
Calculate the relative relationship between point P and line L,
return ONLINE, LEFT, RIGHT.
*/
{
    double res = (L.p2 - L.p1) * (p - L.p1);
    if (Equal(res, 0)) {
        return ONLINE;
    } else if (res > 0) {
        return LEFT;
    } else {
        return RIGHT;
    }
}

bool SameSide(Point p1, Point p2, Line L)
/*
Determine whether point P1 and P2 are on the same side of line L.
*/
{
    double m1 = (p1 - L.p1) * (L.p2 - L.p1);
    double m2 = (p2 - L.p1) * (L.p2 - L.p1);
    return GreaterThan(m1 * m2, 0);
}

bool OnLineSeg(Point p, Line L)
/*
Determine whether point P in two-dimensional space is on line segment L.
*/
{
    return (ZERO((L.p1 - p) * (L.p2 - p)) &&
            LessEqual((p.x - L.p1.x) * (p.x - L.p2.x), 0) &&
            LessEqual((p.y - L.p1.y) * (p.y - L.p2.y), 0));
}
bool OnLineSeg(Point3D p, Line3D L)
/*
Determine whether point P in three-dimensional space is on line segment L.
*/
{
    return (ZERO((L.p1 - p) * (L.p2 - p)) &&
            Equal(Norm(p - L.p1) + Norm(p - L.p2), Norm(L.p2 - L.p1)));
}
Point SymPoint(Point p, Line L)
/*
Query the symmetric point of point P on two-dimensional plane with respect to
line L.
*/
{
    Point result;
    double a = L.p2.x - L.p1.x;
    double b = L.p2.y - L.p1.y;
    double t = ((p.x - L.p1.x) * a + (p.y - L.p1.y) * b) / (a * a + b * b);
    result.x = 2 * L.p1.x + 2 * a * t - p.x;
    result.y = 2 * L.p1.y + 2 * b * t - p.y;
    return result;
}

bool Coplanar(Points3D points)
/*
Determine whether all points in a point set are coplanar.
*/
{
    int i;
    Point3D p;

    if (points.size() < 4) return true;
    p = (points[2] - points[0]) * (points[1] - points[0]);
    for (i = 3; i < points.size(); i++) {
        if (!ZERO(p & points[i])) return false;
    }
    return true;
}

bool LineIntersect(Line L1, Line L2)
/*
Determine whether two two-dimensional straight lines intersect.
*/
{
    return (!ZERO((L1.p1 - L1.p2) * (L2.p1 - L2.p2)));
}
bool LineIntersect(Line3D L1, Line3D L2)
/*
Determine whether two three-dimensional straight lines intersect.
*/
{
    Point3D p1 = L1.p1 - L1.p2;
    Point3D p2 = L2.p1 - L2.p2;
    Point3D p = p1 * p2;
    if (ZERO(p)) return false;
    p = (L2.p1 - L1.p2) * (L1.p1 - L1.p2);
    return ZERO(p & L2.p2);
}
bool LineSegIntersect(Line L1, Line L2)
/*
Judge whether two lines intersect in two dimensions.
*/
{
    return (
        GreaterEqual(std::max(L1.p1.x, L1.p2.x), std::min(L2.p1.x, L2.p2.x)) &&
        GreaterEqual(std::max(L2.p1.x, L2.p2.x), std::min(L1.p1.x, L1.p2.x)) &&
        GreaterEqual(std::max(L1.p1.y, L1.p2.y), std::min(L2.p1.y, L2.p2.y)) &&
        GreaterEqual(std::max(L2.p1.y, L2.p2.y), std::min(L1.p1.y, L1.p2.y)) &&
        LessEqual(((L2.p1 - L1.p1) * (L1.p2 - L1.p1)) *
                      ((L2.p2 - L1.p1) * (L1.p2 - L1.p1)),
                  0) &&
        LessEqual(((L1.p1 - L2.p1) * (L2.p2 - L2.p1)) *
                      ((L1.p2 - L2.p1) * (L2.p2 - L2.p1)),
                  0));
}
int CalCrossPoint(Line L1, Line L2, Point& P)
/*
The intersection of two two two-dimensional straight lines is calculated and the
result is returned in parameter P. The return value indicates the position
relationship between the two lines: COLINE=1:Collinear; PARALLEL=2:Parallel;
CROSS=3:Intersection.
*/
{
    double A1, B1, C1, A2, B2, C2;

    A1 = L1.p2.y - L1.p1.y;
    B1 = L1.p1.x - L1.p2.x;
    C1 = L1.p2.x * L1.p1.y - L1.p1.x * L1.p2.y;

    A2 = L2.p2.y - L2.p1.y;
    B2 = L2.p1.x - L2.p2.x;
    C2 = L2.p2.x * L2.p1.y - L2.p1.x * L2.p2.y;

    if (Equal(A1 * B2, B1 * A2)) {
        if (Equal((A1 + B1) * C2, (A2 + B2) * C1)) {
            return COLINE;
        } else {
            return PARALLEL;
        }
    } else {
        P.x = (B2 * C1 - B1 * C2) / (A2 * B1 - A1 * B2);
        P.y = (A1 * C2 - A2 * C1) / (A2 * B1 - A1 * B2);
        return CROSS;
    }
}

Point NearestPointToLine(Point P, Line L)
/*
Calculate the nearest point from point P to line L.
*/
{
    Point result;
    double a, b, t;

    a = L.p2.x - L.p1.x;
    b = L.p2.y - L.p1.y;
    t = ((P.x - L.p1.x) * a + (P.y - L.p1.y) * b) / (a * a + b * b);

    result.x = L.p1.x + a * t;
    result.y = L.p1.y + b * t;
    return result;
}

Point NearestPointToLineSeg(Point P, Line L)
/*
Calculate the nearest point from point P to line segment L.
*/
{
    Point result;
    double a, b, t;

    a = L.p2.x - L.p1.x;
    b = L.p2.y - L.p1.y;
    t = ((P.x - L.p1.x) * a + (P.y - L.p1.y) * b) / (a * a + b * b);

    if (GreaterEqual(t, 0) && LessEqual(t, 1)) {
        result.x = L.p1.x + a * t;
        result.y = L.p1.y + b * t;
    } else {
        if (Norm(P - L.p1) < Norm(P - L.p2)) {
            result = L.p1;
        } else {
            result = L.p2;
        }
    }
    return result;
}

double MinDistance(Line L1, Line L2)
/*
Calculate the shortest distance between segment L1 and segment L2.
*/
{
    double d1, d2, d3, d4;

    if (LineSegIntersect(L1, L2)) {
        return 0;
    } else {
        d1 = Norm(NearestPointToLineSeg(L1.p1, L2) - L1.p1);
        d2 = Norm(NearestPointToLineSeg(L1.p2, L2) - L1.p2);
        d3 = Norm(NearestPointToLineSeg(L2.p1, L1) - L2.p1);
        d4 = Norm(NearestPointToLineSeg(L2.p2, L1) - L2.p2);

        return std::min(std::min(d1, d2), std::min(d3, d4));
    }
}

double Inclination(Line L1, Line L2)
/*
Find the angle between two straight lines in two dimensions.
The return value is the radian between 0 and Pi.
*/
{
    Point u = L1.p2 - L1.p1;
    Point v = L2.p2 - L2.p1;
    return acos((u & v) / (Norm(u) * Norm(v)));
}

double Inclination(Line3D L1, Line3D L2)
/*
Find the angle between three-dimensional two straight lines.
The return value is the radian between 0 and Pi.
*/
{
    Point3D u = L1.p2 - L1.p1;
    Point3D v = L2.p2 - L2.p1;
    return acos((u & v) / (Norm(u) * Norm(v)));
}

int InsideConvex(Point p, const Polygon& poly)
/*
Determine whether point P is in a convex polygon.
INSIDE=1:The point is in the polygon;
BORDER=2:The point is on the polygon boundary;
OUTSIDE=3:The point is outside polygon.
*/
{
    Point q(0, 0);
    Line side;
    int i, n = poly.size();

    for (i = 0; i < n; i++) {
        q.x += poly[i].x;
        q.y += poly[i].y;
    }
    q.x /= n;
    q.y /= n;
    for (i = 0; i < n; i++) {
        side.p1 = poly[i];
        side.p2 = poly[(i + 1) % n];
        if (OnLineSeg(p, side)) {
            return BORDER;
        } else if (!SameSide(p, q, side)) {
            return OUTSIDE;
        }
    }
    return INSIDE;
}

bool IsConvex(const Polygon& poly)
/*
Determine whether the polygon is convex.
*/
{
    int i, n, rel;
    Line side;

    n = poly.size();
    if (n < 3) return false;
    side.p1 = poly[0];
    side.p2 = poly[1];
    rel = Relation(poly[2], side);
    for (i = 1; i < n; i++) {
        side.p1 = poly[i];
        side.p2 = poly[(i + 1) % n];
        if (Relation(poly[(i + 2) % n], side) != rel) return false;
    }
    return true;
}

int InsidePolygon(const Polygon& poly, Point p)
/*
To determine whether point P is in a simple polygon.
(the polygon can be convex or concave)
INSIDE=1:The point is in the polygon;
BORDER=2:The point is on the polygon boundary;
OUTSIDE=3:The point is outside polygon.
*/
{
    int i, n, count;
    Line ray, side;

    n = poly.size();
    count = 0;
    ray.p1 = p;
    ray.p2.y = p.y;
    ray.p2.x = -INF;

    for (i = 0; i < n; i++) {
        side.p1 = poly[i];
        side.p2 = poly[(i + 1) % n];

        if (OnLineSeg(p, side)) {
            return BORDER;
        }
        if (Equal(side.p1.y, side.p2.y)) {
            continue;
        }
        if (OnLineSeg(side.p1, ray)) {
            if (GreaterThan(side.p1.y, side.p2.y)) count++;
        } else if (OnLineSeg(side.p2, ray)) {
            if (GreaterThan(side.p2.y, side.p1.y)) count++;
        } else if (LineSegIntersect(ray, side)) {
            count++;
        }
    }
    return ((count % 2 == 1) ? INSIDE : OUTSIDE);
}

bool InsidePolygon(const Polygon& poly, Line L)
/*
Determine whether a line segment is in a polygon.
(the point of the line segment may be on the polygon)
*/
{
    bool result;
    int n, i;
    Points points;
    Point p;
    Line side;

    result = ((InsidePolygon(poly, L.p1) != OUTSIDE) &&
              (InsidePolygon(poly, L.p2) != OUTSIDE));

    if (!result) return false;

    n = poly.size();
    for (i = 0; i < n; i++) {
        side.p1 = poly[i];
        side.p2 = poly[(i + 1) % n];

        if (OnLineSeg(L.p1, side)) {
            points.push_back(L.p1);
        } else if (OnLineSeg(L.p2, side)) {
            points.push_back(L.p2);
        } else if (OnLineSeg(side.p1, L)) {
            points.push_back(side.p1);
        } else if (OnLineSeg(side.p2, L)) {
            points.push_back(side.p2);
        } else if (LineSegIntersect(side, L)) {
            return false;
        }
    }
    sort(points.begin(), points.end());

    for (i = 1; i < points.size(); i++) {
        if (points[i - 1] != points[i]) {
            p.x = (points[i - 1].x + points[i].x) / 2.0;
            p.y = (points[i - 1].y + points[i].y) / 2.0;
            if (InsidePolygon(poly, p) == OUTSIDE) {
                return false;
            }
        }
    }
    return true;
}

bool ConvexHullCmp(const Point& left, const Point& right)
/*
Comparison function for convex hull.
*/
{
    if (Equal(left.angle, right.angle)) {
        return (left.dis < right.dis);
    } else {
        return (left.angle < right.angle);
    }
}

void ConvexHull(Points& points, Polygon& result)
/*
Query convex hull.
*/
{
    int i, k, n;
    Point p;

    n = points.size();
    result.clear();

    if (n < 3) return;

    k = 0;
    for (i = 1; i < n; i++) {
        if (Equal(points[i].y, points[k].y)) {
            if (points[i].x <= points[k].x) k = i;
        } else if (points[i].y < points[k].y) {
            k = i;
        }
    }
    std::swap(points[0], points[k]);

    for (i = 1; i < n; i++) {
        points[i].angle =
            atan2(points[i].y - points[0].y, points[i].x - points[0].x);
        points[i].dis = Norm(points[i] - points[0]);
    }

    sort(points.begin() + 1, points.end(), ConvexHullCmp);

    result.push_back(points[0]);
    for (i = 1; i < n; i++) {
        if ((i + 1 < n) && Equal(points[i].angle, points[i + 1].angle))
            continue;
        if (result.size() >= 3) {
            p = result[result.size() - 2];
            while (GreaterEqual((points[i] - p) * (result.back() - p), 0)) {
                result.pop_back();
                p = result[result.size() - 2];
            }
        }
        result.push_back(points[i]);
    }
}

double CutConvex(const Polygon& poly, const Line& line, Polygon result[3])
/*
Cutting convex polygon with directed straight line.
Result [LEFT] and result [RIGHT] save the left and right parts of the cut
line,respectively.
*/
{
    std::vector<Point> points;
    Line side;
    Point p;
    int i, n, cur, pre;

    result[LEFT].clear();
    result[RIGHT].clear();
    result[ONLINE].clear();
    n = poly.size();
    if (n == 0) return 0;
    pre = cur = Relation(poly[0], line);

    for (i = 0; i < n; i++) {
        cur = Relation(poly[(i + 1) % n], line);
        if (cur == pre) {
            result[cur].push_back(poly[(i + 1) % n]);
        } else {
            side.p1 = poly[i];
            side.p2 = poly[(i + 1) % n];
            CalCrossPoint(side, line, p);
            points.push_back(p);
            result[pre].push_back(p);
            result[cur].push_back(p);
            result[cur].push_back(poly[(i + 1) % n]);
            pre = cur;
        }
    }

    sort(points.begin(), points.end());

    if (points.size() < 2) {
        return 0;
    } else {
        return Norm(points.front() - points.back());
    }
}

Point CenterOfPolygon(const Polygon& poly)
/*
Query the center of a polygon.
*/
{
    Point p, p0, p1, p2, p3;
    double m, m0;

    p1 = poly[0];
    p2 = poly[1];
    p.x = p.y = m = 0;
    for (int i = 2; i < poly.size(); i++) {
        p3 = poly[i];
        p0.x = (p1.x + p2.x + p3.x) / 3.0;
        p0.y = (p1.y + p2.y + p3.y) / 3.0;
        m0 = p1.x * p2.y + p2.x * p3.y + p3.x * p1.y - p1.y * p2.x -
             p2.y * p3.x - p3.y * p1.x;
        if (ZERO(m + m0)) {
            m0 += EPS;
        }
        p.x = (m * p.x + m0 * p0.x) / (m + m0);
        p.y = (m * p.y + m0 * p0.y) / (m + m0);
        m = m + m0;
        p2 = p3;
    }
    return p;
}

bool Intersect(Rect_2 r1, Rect_2 r2)
/*
Judging whether two rectangles intersect,
adjacent is not intersecting
*/
{
    return (std::max(r1.xl, r2.xl) < std::min(r1.xh, r2.xh) &&
            std::max(r1.yl, r2.yl) < std::min(r1.yh, r2.yh));
}

bool IsContain(Rect r1, Rect r2)
/*
Determine whether rectangular r2 can be placed in rectangular r1.
*/
{
    if (r1.w > r2.w && r1.h > r2.h)
        return true;
    else {
        double r = sqrt(r2.w * r2.w + r2.h * r2.h) / 2.0;
        double alpha = atan2(r2.h, r2.w);
        double sita = asin((r1.h / 2.0) / r);
        double x = r * cos(sita - 2 * alpha);
        double y = r * sin(sita - 2 * alpha);
        if (x < r1.w / 2.0 && y < r1.h / 2.0 && x > 0 && y > -r1.h / 2.0)
            return true;
        else
            return false;
    }
}

Point Center(const Circle& C)
/*
Return the center of the cicle.
*/
{
    return C.c;
}

double CommonArea(const Circle& A, const Circle& B)
/*
Public area of two circles.
*/
{
    double area = 0.0;
    const Circle& M = (A.r > B.r) ? A : B;
    const Circle& N = (A.r > B.r) ? B : A;
    double D = Distance(Center(M), Center(N));
    if ((D < M.r + N.r) && (D > M.r - N.r)) {
        double cosM = (M.r * M.r + D * D - N.r * N.r) / (2.0 * M.r * D);
        double cosN = (N.r * N.r + D * D - M.r * M.r) / (2.0 * N.r * D);
        double alpha = 2.0 * acos(cosM);
        double beta = 2.0 * acos(cosN);
        double TM = 0.5 * M.r * M.r * sin(alpha);
        double TN = 0.5 * N.r * N.r * sin(beta);
        double FM = (alpha / 360.0) * Area(M);
        double FN = (beta / 360.0) * Area(N);
        area = FM + FN - TM - TN;
    } else if (D <= M.r - N.r) {
        area = Area(N);
    }
    return area;
}

bool IsInCircle(const Circle& C, const Rect_2& rect)
/*
Determine if the circle is inside the rectangle
(tangent is not allowed)
*/
{
    return (
        GreaterThan(C.c.x - C.r, rect.xl) && LessThan(C.c.x + C.r, rect.xh) &&
        GreaterThan(C.c.y - C.r, rect.yl) && LessThan(C.c.y + C.r, rect.yh));
}

int CircleRelation(const Circle& c1, const Circle& c2)
/*
Judging the positional relationship between the two circles.
CONTAIN=1:Big circle contains small circle;
INTERTAN=2:Internally tangent;
INTERSECT=3:Intersect;
EXTERTAN=4:Externally tangent;
DISJOINT=5:Disjoints;
*/
{
    double dis = Distance(c1.c, c2.c);
    if (LessThan(dis, fabs(c1.r - c2.r))) return CONTAIN;
    if (Equal(dis, fabs(c1.r - c2.r))) return INTERTAN;
    if (LessThan(dis, c1.r + c2.r) && GreaterThan(dis, fabs(c1.r - c2.r)))
        return INTERSECT;
    if (Equal(dis, c1.r + c2.r)) return EXTERTAN;
    return DISJOINT;
}

void CircleDeter(Point p1, Point p2, Point p3, Circle& C) {
    double a, b, c, d, e, f;
    a = p2.y - p1.y;
    b = p3.y - p1.y;
    c = p2.x - p1.x;
    d = p3.x - p1.x;
    f = p3.x * p3.x + p3.y * p3.y - p1.x * p1.x - p1.y * p1.y;
    e = p2.x * p2.x + p2.y * p2.y - p1.x * p1.x - p1.y * p1.y;
    C.c.x = (a * f - b * e) / (2 * a * d - 2 * b * c);
    C.c.y = (d * e - c * f) / (2 * a * d - 2 * b * c);
    C.r = Distance(C.c, p1);
}

void MinCircleCover(Points a, Circle& result)
/*
Find the minimum circle cover of the point set
and store the result in result.
*/
{
    random_shuffle(a.begin(), a.end());
    int n = a.size();
    result.c = a[0];
    result.r = 0;

    for (int i = 1; i < n; ++i) {
        if (GreaterThan(Distance(a[i], result.c), result.r)) {
            result.c = a[i];
            result.r = 0;

            for (int j = 0; j < i; ++j) {
                if (GreaterThan(Distance(a[j], result.c), result.r)) {
                    result.c.x = (a[i].x + a[j].x) / 2.0;
                    result.c.y = (a[i].y + a[j].y) / 2.0;
                    result.r = Distance(result.c, a[j]);

                    for (int k = 0; k < j; ++k)
                        if (GreaterThan(Distance(result.c, a[k]), result.r))
                            CircleDeter(a[i], a[j], a[k], result);
                }
            }
        }
    }
}

template <const size_t Maxn>
class scan_line {
   private:
    int n;
    long long *dic, *val, tot, INF = 1 << 31;

    struct Node {
        int l, r;
        long long val, len;

        Node() : l(0), r(0), val(0), len(0) {}
    } * tree;

    struct Line {
        long long x, y1, y2, flag;

        Line() : x(0), y1(0), y2(0), flag(0) {}

        bool operator<(const Line& b) const {
            if (x != b.x) return x < b.x;
            return flag > b.flag;
        }
    } * plane;

    void pushup(int p) {
        if ((tree[p].l == INF && tree[p].r == INF)) return;

        if (tree[p].val)
            tree[p].len = val[tree[p].r + 1] - val[tree[p].l];
        else
            tree[p].len = tree[p << 1].len + tree[p << 1 | 1].len;
    }

    void build(int p, int l, int r) {
        tree[p].l = l, tree[p].r = r;

        if (l == r) return;

        int mid = (l + r) >> 1;

        build(p << 1, l, mid), build(p << 1 | 1, mid + 1, r);
    }

    void update(int p, int l, int r, long long v) {
        if (l <= tree[p].l && tree[p].r <= r) {
            tree[p].val += v;
            pushup(p);
            return;
        }

        int mid = (tree[p].l + tree[p].r) >> 1;

        if (l <= mid) update(p << 1, l, r, v);
        if (mid < r) update(p << 1 | 1, l, r, v);

        pushup(p);
    }

   public:
    scan_line() : n(0), tot(0) {
        dic = new long long[Maxn];
        val = new long long[Maxn];
        tree = new Node[Maxn]();
        plane = new Line[Maxn]();
        memset(dic, 0, sizeof(long long) * Maxn);
        memset(val, 0, sizeof(long long) * Maxn);
    }

    void insert(long long x1, long long y1, long long x2, long long y2) {
        ++n;
        plane[(n << 1) - 1].x = x1, plane[n << 1].x = x2;
        plane[(n << 1) - 1].y1 = plane[n << 1].y1 = y2;
        plane[(n << 1) - 1].y2 = plane[n << 1].y2 = y1;
        plane[(n << 1) - 1].flag = 1, plane[n << 1].flag = -1;
        dic[++tot] = y1;
        dic[++tot] = y2;
    }

    long long query_area() {
        long long ans = 0;

        std::sort(dic + 1, dic + (n << 1) + 1);
        tot = std::unique(dic + 1, dic + (n << 1) + 1) - dic - 1;

        for (int i = 1; i <= n << 1; ++i) {
            long long pos1 =
                std::lower_bound(dic + 1, dic + tot + 1, plane[i].y1) - dic;
            long long pos2 =
                std::lower_bound(dic + 1, dic + tot + 1, plane[i].y2) - dic;

            val[pos1] = plane[i].y1, val[pos2] = plane[i].y2;

            plane[i].y1 = pos1;
            INF = std::max(INF, pos1);
            plane[i].y2 = pos2;
        }

        std::sort(plane + 1, plane + (n << 1) + 1);
        build(1, 1, n << 1);

        for (int i = 1; i <= n << 1; ++i) {
            update(1, plane[i].y2, plane[i].y1 - 1, plane[i].flag);
            ans += tree[1].len * (plane[i + 1].x - plane[i].x);
        }

        return ans;
    }
};
}  // namespace geometry
}  // namespace Octinc

#endif
