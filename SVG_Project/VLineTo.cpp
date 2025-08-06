#include "VLineTo.h"
using namespace Gdiplus;

void VLineTo::draw(Graphics* graphics, GraphicsPath* path) {
    PointF last;
    path->GetLastPoint(&last);
    path->AddLine(PointF(last.X, last.Y), PointF(last.X, y));
}
