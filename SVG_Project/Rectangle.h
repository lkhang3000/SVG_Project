/* Defining a Rectangle class and its methods */
#ifndef _RECTANGLE_
#define _RECTANGLE_


#include <objidl.h>
#include <windows.h>
#include <gdiplus.h>
#include <sstream>
#include "tinyxml2.h"
#include "SVGElement.h"


using namespace std;
using namespace Gdiplus;

#pragma comment (lib,"Gdiplus.lib")

class rectangle : public SVGElement {
	Point origin;
	int width;
	int height;

public:
	rectangle();
	void setValue(tinyxml2::XMLElement* element) override;
	void draw(HDC hdc, gradientDatabase& database) override;
	void draw(Graphics* g, gradientDatabase& database) override;
};



#endif // !_RECTANGLE_

