/* Defining a Shape class and its methods. Shape class is the base class of Ellipse, Polyline, Rectangle, Polygon */
#ifndef _SHAPE_
#define _SHAPE_

#include <objidl.h>
#include <windows.h>
#include <gdiplus.h>
#include <sstream>
#include "tinyxml2.h"
#include "SVGElement.h"

using namespace std;
using namespace Gdiplus;

#pragma comment (lib,"Gdiplus.lib") 

class shape : public SVGElement {
protected:
	double fillOpacity;
	double strokeOpacity;
	Color fill;
	Color stroke;
	int strokeWidth;
	bool hasFill;
	bool hasStroke;
	string transform;
public:
	shape();
	void handleTransform(Graphics* graphics);
	virtual void setValue(tinyxml2::XMLElement* element) override;
	virtual void draw(HDC hdc, gradientDatabase& database) override = 0; //When Completed, try deleting this function to see if the code still runs
};


#endif // !_SHAPE_

