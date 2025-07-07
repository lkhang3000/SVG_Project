#ifndef _SVGCONTAINER_
#define _SVGCONTAINER_


#include "SVGElement.h"
#include "Shape.h"
#include "Rectangle.h"
#include "Polygon.h"
#include "Text.h"
#include "Polyline.h"
#include "Line.h"
#include "Ellipse.h"

class SVGContainer {
	vector<SVGElement*> content; //A vector containing all SVGElement
public:
	void addElements(const char* fileName);
	void addElement(tinyxml2::XMLElement* element);
	void drawAllElement(HDC hdc);
	~SVGContainer();
};


#endif // !_SVGCONTAINER_