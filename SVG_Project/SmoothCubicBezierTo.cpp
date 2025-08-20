#include "SmoothCubicBezierTo.h"

SmoothCubicBezierTo::SmoothCubicBezierTo(float px1, float py1, float x2, float y2, float x, float y)
    : px1(px1), py1(py1), x2(x2), y2(y2), x(x), y(y) {
}

void SmoothCubicBezierTo::draw(Gdiplus::Graphics* graphics, Gdiplus::GraphicsPath* path) {
    PointF last;
    path->GetLastPoint(&last);
    PointF p1(px1, py1);
    PointF p2(x2, y2);
    PointF end(x, y);
    
    // Add the smooth cubic bezier curve to the path
    path->AddBezier(last, p1, p2, end);
}
