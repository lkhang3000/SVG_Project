#ifndef _GRADIENT_H
#define _GRADIENT_H
#include <iostream>
#include <string>
#include <objidl.h>
#include <windows.h>
#include <gdiplus.h>
#include <sstream>
#include <vector>
#include "tinyxml2.h"

using namespace std;
using namespace Gdiplus;

#pragma comment (lib,"Gdiplus.lib")

class gradient {
    string id;
public:
    void setValue(tinyxml2::XMLElement* element);
    virtual Brush* getBrush() = 0;
    string getID();
};

class linearGradient : public gradient {
public:
    Brush* getBrush() override;
};

class radialGradient : public gradient {
public:
    Brush* getBrush() override;
};

class gradientDatabase {
    vector <gradient*> content;
public:
    void extractData(tinyxml2::XMLElement* element);
    Brush* getBrush(string id);
    ~gradientDatabase();
};
#endif // !_GRADIENT_H

/**/