#ifndef _VLINETO_
#define _VLINETO_

#include "PathCommand.h"

class VLineTo : public PathCommand {
    int y;
public:
    VLineTo(int yCoord) : y(yCoord) {}
    void draw(Gdiplus::Graphics* graphics, Gdiplus::GraphicsPath* path) override;
};

#endif
