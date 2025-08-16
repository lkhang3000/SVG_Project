/* Defining a Line class and its methods */
#ifndef _LINE_
#define _LINE_

#include <objidl.h>
#include <windows.h>
#include <gdiplus.h>
#include <sstream>
#include <vector>
#include "tinyxml2.h"
#include "SVGElement.h"


using namespace std;
using namespace Gdiplus;

#pragma comment (lib,"Gdiplus.lib")

class line : public SVGElement {
    vector<Point> vertices;
public:
    line();
    void setValue(tinyxml2::XMLElement* element) override;
    void draw(HDC hdc, gradientDatabase& database) override;
    void draw(Graphics* g, gradientDatabase& database) override;
};
#endif // !_LINE_
