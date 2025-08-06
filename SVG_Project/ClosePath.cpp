#include "ClosePath.h"
using namespace Gdiplus;

void ClosePath::draw(Graphics* graphics, GraphicsPath* path) {
    path->CloseFigure();
}
