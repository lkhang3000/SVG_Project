/* Defining SVGContainer class, which will help getting SVGElement datas from an XML files and drawing those elements. */
#ifndef _SVGCONTAINER_
#define _SVGCONTAINER_


#include "Group.h"

class SVGContainer {
	vector<SVGElement*> content; //A vector containing all SVGElement
	gradientDatabase database;
public:
	void addElements(const char* fileName);
	void addElement(tinyxml2::XMLElement* element);
	void drawAllElement(HDC hdc);
	~SVGContainer();
};


#endif // !_SVGCONTAINER_


