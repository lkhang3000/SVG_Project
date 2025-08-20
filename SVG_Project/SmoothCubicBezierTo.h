#ifndef SMOOTH_CUBIC_BEZIER_TO_H
#define SMOOTH_CUBIC_BEZIER_TO_H
#include "PathCommand.h"

class SmoothCubicBezierTo : public PathCommand {
public:
    float x2, y2, x, y;
    float px1, py1; // calculated first control point

    SmoothCubicBezierTo(float px1, float py1, float x2, float y2, float x, float y);
    void draw(Gdiplus::Graphics* graphics, Gdiplus::GraphicsPath* path) override;
};
#endif 
