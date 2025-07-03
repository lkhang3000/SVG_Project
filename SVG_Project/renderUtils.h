#pragma once
#ifndef renderUtils_h
#define renderUtils_h
#include <list>
#include "tinyxml2.h"
#include "Shape.h"
#include <iostream>

using namespace tinyxml2;
using namespace std;


class SVGRender {
private:
	list<SVGElement*> elements;
public:
	~SVGRender();
	bool loadFile(const string& fileName);
	void Parse(XMLElement* element);
	void Render(Graphics& graphic);
};

#endif