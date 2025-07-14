#ifndef _SVGELEMENT_
#define _SVGELEMENT_


#include <iostream>
#include <objidl.h>
#include <windows.h>
#include <gdiplus.h>
#include <sstream>
#include <vector>
#include "tinyxml2.h"


class SVGElement {
public:
	virtual void setValue(tinyxml2::XMLElement* element) = 0;
	virtual void draw(HDC hdc) = 0;
};




#endif // !_SVGELEMENT_

