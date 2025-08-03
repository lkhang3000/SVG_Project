#include "HLineTo.h"
using namespace Gdiplus;

void HLineTo::draw(Graphics* graphics, GraphicsPath* path) {
    PointF last;
    path->GetLastPoint(&last);
    path->AddLine(PointF(last.X, last.Y), PointF(x, last.Y));
}
