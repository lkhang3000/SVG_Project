#ifndef _HLINETO_
#define _HLINETO_

#include "PathCommand.h"

class HLineTo : public PathCommand {
    int x;
public:
    HLineTo(int xCoord) : x(xCoord) {}
    void draw(Gdiplus::Graphics* graphics, Gdiplus::GraphicsPath* path) override;
};

#endif
