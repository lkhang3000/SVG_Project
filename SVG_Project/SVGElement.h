/* Defining a SVGElement class and its methods. SVGElement class is the base class of Text, Line, Shape */
#ifndef _SVGELEMENT_H
#define _SVGELEMENT_H


#include <iostream>
#include <string>
#include <objidl.h>
#include <windows.h>
#include <gdiplus.h>
#include <sstream>
#include <vector>
#include "tinyxml2.h"

using namespace std;
using namespace Gdiplus;

#pragma comment (lib,"Gdiplus.lib")

class SVGElement {
protected:
	string transform;
	double fillOpacity;
	double strokeOpacity;
	Color fill;
	Color stroke;
	int strokeWidth;
public:
	SVGElement();
	void handleTransform(Graphics* graphics);
	void deleteTransform();
	virtual void setValue(tinyxml2::XMLElement* element);
	virtual void draw(HDC hdc) = 0;
	virtual void draw(Graphics* g) = 0;
	
};

#endif // !_SVGELEMENT_
