#ifndef _SVGPATH_
#define _SVGPATH_

#include <objidl.h>
#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <string>
#include <sstream>
#include "tinyxml2.h"



#include "SVGElement.h"
#include "MoveTo.h"
#include "ArcTo.h"
#include "SmoothCubicBezierTo.h"
#include "ShortCurveTo.h"
#include "CurveTo.h"
#include "LineTo.h"
#include "HLineTo.h"
#include "VLineTo.h"
#include "ClosePath.h"


using namespace std;
using namespace Gdiplus;

#pragma comment (lib,"Gdiplus.lib")

class path : public SVGElement {
    vector<PathCommand*> commands;
public:
    path();
    ~path();
    void setValue(tinyxml2::XMLElement* element) override;
    void draw(HDC hdc, gradientDatabase& database) override;
    void draw(Graphics* g, gradientDatabase& database) override;
    bool isCommand(const string& token);
	vector<string> tokenizeSVGPath(const string& d);
};

#endif // !_SVGPATH_
