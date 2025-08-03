#include "LineTo.h"
using namespace Gdiplus;

void Lineto::draw(Graphics* graphics, GraphicsPath* path) {
    PointF last;
    path->GetLastPoint(&last);
    path->AddLine(last, PointF(destination.X, destination.Y));
}
