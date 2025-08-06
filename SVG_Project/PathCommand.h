#ifndef PATH_COMMAND_H
#define PATH_COMMAND_H
#include <windows.h>
#include <gdiplus.h>
#include "tinyxml2.h"
using namespace std;
using namespace Gdiplus;

class PathCommand {
public:
    virtual ~PathCommand() = default;
    virtual void draw(Gdiplus::Graphics* graphics, Gdiplus::GraphicsPath* path) = 0;
};
#endif

