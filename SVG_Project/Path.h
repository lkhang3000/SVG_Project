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
#include "CurveTo.h"
#include "LineTo.h"
#include "HLineTo.h"
#include "VLineTo.h"
#include "ArcTo.h"
#include "SmoothCubicBezierTo.h"
#include "ClosePath.h"


using namespace std;
using namespace Gdiplus;

#pragma comment (lib,"Gdiplus.lib")

class path : public SVGElement {
    vector<PathCommand*> commands;
public:
    path();
    ~path();
    static bool isCommand(const string& token);

    void setValue(tinyxml2::XMLElement* element) override;
    void draw(HDC hdc, gradientDatabase& database) override;
    void draw(Graphics* g, gradientDatabase& database) override;
	vector<string> tokenizeSVGPath(const string& d);
    static float getNextFloat(const vector<string>& tokens, size_t& i);
	static int getNextInt(const vector<string>& tokens, size_t& i);
};

#endif // !_SVGPATH_
