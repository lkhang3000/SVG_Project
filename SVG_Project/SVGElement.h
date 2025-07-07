#ifndef _SVGELEMENT_
#define _SVGELEMENT_


#include <iostream>
#include <objidl.h>
#include <windows.h>
#include <gdiplus.h>
#include <sstream>
#include <vector>
#include "tinyxml2.h"


/*
class shapeListNode {
	shape* data;
	shapeListNode* pNext;
public:
	shapeListNode();
	void addNextShape(tinyxml2::XMLElement* data);
	void addNextShape(shapeListNode* data);
	void drawShape(HDC hdc);
	shapeListNode* getNextNode();
	~shapeListNode();
};

class shapeList {
	shapeListNode* pHead;
	shapeListNode* pTail;

	void addFirst(tinyxml2::XMLElement* data);
public:
	shapeList();
	void addTail(tinyxml2::XMLElement* data);
	void drawAllShape(HDC hdc);
	~shapeList();
};
*/


class SVGElement {
public:
	virtual void setValue(tinyxml2::XMLElement* element) = 0;
	virtual void draw(HDC hdc) = 0;
};




#endif // !_SVGELEMENT_

