/* Defining a Polygon class and its methods */
#ifndef _POLYGON_
#define _POLYGON_

#include <objidl.h>
#include <windows.h>
#include <gdiplus.h>
#include <sstream>
#include <vector>
#include "tinyxml2.h"
#include "SVGElement.h"


using namespace std;
using namespace Gdiplus;

#pragma comment (lib,"Gdiplus.lib")

class polygon : public SVGElement {
	vector<Point> vertices;
public:
	polygon();
	void draw(HDC hdc) override;
	void draw(Graphics* g) override;
	void setValue(tinyxml2::XMLElement* element) override;
};


#endif // !_POLYGON_
