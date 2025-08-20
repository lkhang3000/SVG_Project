#ifndef _ARCTO_
#define _ARCTO_

#include "PathCommand.h"
#include <gdiplus.h>

// Implements the SVG Arc command (A / a)
class Arcto : public PathCommand {
    float rx, ry;
    float xAxisRotation;
    int largeArcFlag;
    int sweepFlag;
    Gdiplus::PointF destination;

public:
    Arcto(float rx, float ry, float xAxisRotation,
        int largeArcFlag, int sweepFlag,
        float x, float y);

    void draw(Gdiplus::Graphics* graphics, Gdiplus::GraphicsPath* path) override;
};

#endif
