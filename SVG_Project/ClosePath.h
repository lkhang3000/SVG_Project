#ifndef _CLOSEPATH_
#define _CLOSEPATH_

#include "PathCommand.h"

class ClosePath : public PathCommand {
public:
    void draw(Gdiplus::Graphics* graphics, Gdiplus::GraphicsPath* path) override;
};

#endif
