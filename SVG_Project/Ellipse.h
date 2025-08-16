/* Defining an Eclipse class and its derived class (Circle) */
#ifndef _ELLIPSE_
#define _ELLIPSE_

#include <objidl.h>
#include <windows.h>
#include <gdiplus.h>
#include <sstream>
#include "tinyxml2.h"
#include "SVGElement.h"


using namespace std;
using namespace Gdiplus;

#pragma comment (lib,"Gdiplus.lib")

class ellipse : public SVGElement {
protected:
	Point origin;
	double height, width;
public:
	ellipse();
	void setValue(tinyxml2::XMLElement* element) override;
	void draw(HDC hdc, gradientDatabase& database) override;
	void draw(Graphics* g, gradientDatabase& database) override;
};

class circle : public ellipse {
public:
	circle();
	void setValue(tinyxml2::XMLElement* element) override;
};



#endif // !_ELLIPSE_

