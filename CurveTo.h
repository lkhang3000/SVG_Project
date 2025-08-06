#ifndef CURVETO_H
#define CURVETO_H
#include "PathCommand.h"

class CurveTo : public PathCommand {
    Point p1, p2, p3;
public:
    CurveTo(Point P1, Point P2, Point P3): p1(P1), p2(P2), p3(P3) {}
    CurveTo(int x1, int y1, int x2, int y2, int x3, int y3) : p1(Point(x1, y1)), p2(Point(x2, y2)), p3(Point(x3, y3)) {
	}
    void draw(Gdiplus::Graphics* graphics, Gdiplus::GraphicsPath* path) override;
};
#endif
