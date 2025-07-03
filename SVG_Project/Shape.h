#pragma once
#ifndef  Shape_h
#define  Shape_h
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <regex>
#include <iostream>
#include "Helper.h"
#include "tinyxml2.h"
 
 
using namespace std;
using namespace Gdiplus;
using namespace tinyxml2;

#pragma comment (lib,"Gdiplus.lib")

class SVGElement {
public:
	virtual void Render(Graphics& graphics) = 0;
	static bool ParseColor(string colType, SVGColor& color);
};

class Circle : public SVGElement {
private:
	float x, y, r;
	SVGColor fillCol, strokeCol;
	float strokeWid, fillOpa, strokeOpa;
	bool checkFill, checkStroke;
public:
	Circle(XMLElement* elem);
	void Render(Graphics& graphic) override;
};

class Eclipse : public SVGElement {
private:
	float cx, cy, rx, ry;
	SVGColor fillCol, strokeCol;
	float strokeWid, fillOpa, strokeOpa;
	bool checkFill, checkStroke;
public:
	Eclipse(XMLElement* elem);
	void Render(Graphics& graphic) override;
};

#endif // ! Shape_h
