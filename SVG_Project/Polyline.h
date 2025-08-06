/* Defining a Polyline class and its methods */
#ifndef _POLYLINE_
#define _POLYLINE_

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



// --------- Polylines ---------
class polyline : public SVGElement {
    vector<Point> vertices;
public:
    polyline();
    void setValue(tinyxml2::XMLElement* element) override;
    void draw(HDC hdc) override;
    void draw(Graphics* g) override;
};

#endif // !_POLYLINE_


