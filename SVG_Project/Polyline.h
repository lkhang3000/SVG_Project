/* Defining a Polyline class and its methods */
#ifndef _POLYLINE_
#define _POLYLINE_

#include <objidl.h>
#include <windows.h>
#include <gdiplus.h>
#include <sstream>
#include <vector>
#include "tinyxml2.h"
#include "Shape.h"


using namespace std;
using namespace Gdiplus;

#pragma comment (lib,"Gdiplus.lib") 



// --------- Polylines ---------
class polyline : public shape {
    vector<Point> vertices;
public:
    void setValue(tinyxml2::XMLElement* element) override;
    void draw(HDC hdc) override;
};

#endif // !_POLYLINE_

