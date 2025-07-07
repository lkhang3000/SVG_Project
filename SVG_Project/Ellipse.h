#ifndef _ELLIPSE_
#define _ELLIPSE_

#include <objidl.h>
#include <windows.h>
#include <gdiplus.h>
#include <sstream>
#include "tinyxml2.h"
#include "Shape.h"


using namespace std;
using namespace Gdiplus;

#pragma comment (lib,"Gdiplus.lib")

class ellipse : public shape {
protected:
	Point origin;
	double height, width;
public:
	ellipse();
	void setValue(tinyxml2::XMLElement* element) override;
	void draw(HDC hdc) override;
};

class circle : public ellipse {
public:
	void setValue(tinyxml2::XMLElement* element) override;
};



#endif // !_ELLIPSE_

