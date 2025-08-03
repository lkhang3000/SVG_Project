#include "MoveTo.h"
using namespace Gdiplus;

void MoveTo::draw(Graphics* graphics, GraphicsPath* path) {
	// MoveTo command sets the starting point of the path
	path->StartFigure();
	path->AddLine(origin, origin); // Add a line to itself to set the point
}
