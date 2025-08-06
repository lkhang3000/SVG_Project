#ifndef MOVETO_H
#define MOVETO_H
#include "PathCommand.h"

class MoveTo : public PathCommand {
    Point origin;
public:
    MoveTo(Point p) : origin(p) {}
	MoveTo(int x, int y) : origin(Point(x, y)) {}
    void draw(Gdiplus::Graphics* graphics, Gdiplus::GraphicsPath* path) override;
};
#endif 
