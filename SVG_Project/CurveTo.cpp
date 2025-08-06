#include "CurveTo.h"
using namespace Gdiplus;

void CurveTo::draw(Graphics* graphics, GraphicsPath* path) {
    PointF lastPoint;
    path->GetLastPoint(&lastPoint);
    path->AddBezier(lastPoint, PointF(p1.X, p1.Y), PointF(p2.X, p2.Y), PointF(p3.X, p3.Y));
}
