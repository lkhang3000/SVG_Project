#ifndef _POLYGON_
#define _POLYGON_

#include <objidl.h>
#include <windows.h>
#include <gdiplus.h>
#include <sstream>
#include <vector>
#include "tinyxml2.h"
#include "Shape.h"


using namespace std;
using namespace Gdiplus;

#pragma comment (lib,"Gdiplus.lib")

class polygon : public shape {
	vector<Point> vertices;
public:
	polygon();
	void draw(HDC hdc) override;
	void setValue(tinyxml2::XMLElement* element) override;
};


#endif // !_POLYGON_
