#ifndef _GROUP_
#define _GROUP_

#include "Text.h"
#include "Line.h"
#include "Ellipse.h"
#include "Polygon.h"
#include "Polyline.h"
#include "Rectangle.h"
#include "Path.h"
#include "tinyxml2.h"

using namespace std;
using namespace Gdiplus;

#pragma comment (lib,"Gdiplus.lib")

class SVGGroup : public SVGElement {
	vector <SVGElement*> content;
public:
	SVGGroup();
	void draw(HDC hdc) override;
	void draw(Graphics* g);
	void setValue(tinyxml2::XMLElement* element) override;
	void addElements(tinyxml2::XMLElement* element);
};

#endif // !_GROUP_

