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
    Color stroke;
    double strokeOpacity;
    int strokeWidth;
public:
    line();
    void setValue(tinyxml2::XMLElement* element) override;
    void draw(HDC hdc) override;
};
#endif // !_LINE_
