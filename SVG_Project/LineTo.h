#ifndef _LINETO_
#define _LINETO_

#include "PathCommand.h"

class Lineto : public PathCommand {
    Gdiplus::Point destination;
public:
    Lineto(Gdiplus::Point pt) : destination(pt) {}
    Lineto(int x, int y) : destination(x, y) {}
    void draw(Gdiplus::Graphics* graphics, Gdiplus::GraphicsPath* path) override;
};

#endif
