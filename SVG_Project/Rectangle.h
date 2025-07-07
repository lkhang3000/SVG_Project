#ifndef _RECTANGLE_
#define _RECTANGLE_


#include <objidl.h>
#include <windows.h>
#include <gdiplus.h>
#include <sstream>
#include "tinyxml2.h"
#include "Shape.h"


using namespace std;
using namespace Gdiplus;

#pragma comment (lib,"Gdiplus.lib")

class rectangle : public shape{
	Point origin;
	int width;
	int height;

public:
	rectangle();
	void setValue(tinyxml2::XMLElement* element) override;
	void draw(HDC hdc) override;
};



#endif // !_RECTANGLE_