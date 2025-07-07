#ifndef _TEXT_
#define _TEXT_

#include <objidl.h>
#include <windows.h>
#include <gdiplus.h>
#include <sstream>
#include "tinyxml2.h"
#include "SVGElement.h"


using namespace std;
using namespace Gdiplus;

#pragma comment (lib,"Gdiplus.lib") 

class text : public SVGElement{
    Point origin;
    string content;
    double fontSize;
    Color fill;
    double fillOpacity;
    wstring Utf8ToWstring(const string& str);
public:
    text();
    void setValue(tinyxml2::XMLElement* element) override;
    void draw(HDC hdc) override;
    
};


#endif // !_TEXT_

