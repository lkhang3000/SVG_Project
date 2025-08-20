#ifndef CURVETO_H
#define CURVETO_H
#include "PathCommand.h"

class CurveTo : public PathCommand {
public:
    Point p1, p2, p3;
    CurveTo(Point P1, Point P2, Point P3) : p1(P1), p2(P2), p3(P3) {}
    CurveTo(float x1, float y1, float x2, float y2, float x3, float y3)
        : p1(Point(x1, y1)), p2(Point(x2, y2)), p3(Point(x3, y3)) {
    }

    // Accessors for reflection in S/s command
    float x2() const { return p2.X; }
    float y2() const { return p2.Y; }

    void draw(Gdiplus::Graphics* graphics, Gdiplus::GraphicsPath* path) override;
};
#endif
