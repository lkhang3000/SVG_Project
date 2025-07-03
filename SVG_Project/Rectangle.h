#ifndef _RECTANGLE_
#define _RECTANGLE_


#include <objidl.h>
#include <windows.h>
#include <gdiplus.h>
#include "tinyxml2.h"
#include <sstream>
#include <vector>


using namespace std;
using namespace Gdiplus;

#pragma comment (lib,"Gdiplus.lib")

class shape {
protected:
	double fillOpacity;
	double strokeOpacity;
	Color fill;
	Color stroke;
	int strokeWidth;
public:
	shape();
	virtual void setValue(tinyxml2::XMLElement* element);
	virtual void draw(HDC hdc) = 0;
};

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



class rectangle : public shape{
	Point origin;
	int width;
	int height;

public:
	rectangle();
	void draw(HDC hdc) override;
	void setValue(tinyxml2::XMLElement* element) override;
	
};

class polygon : public shape {
	vector<Point> vertices;
public:
	polygon();
	void draw(HDC hdc) override;
	void setValue(tinyxml2::XMLElement* element) override;
};

#endif // !_RECTANGLE_