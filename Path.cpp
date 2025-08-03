#include "Path.h"
#include "MoveTo.h"
#include "CurveTo.h"
#include "LineTo.h"
#include "HLineTo.h"
#include "VLineTo.h"
#include "ClosePath.h"
#include <sstream>
#include <gdiplus.h>
using namespace std;
using namespace Gdiplus;

path::~path() {
    for (auto cmd : commands) delete cmd;
}


void path::setValue(tinyxml2::XMLElement* element) {
    // Default to no fill and black stroke
    hasFill = false;
    hasStroke = false;
    stroke = Color(255, 0, 0, 0);
    strokeWidth = 1.0f;

    const char* fillAttr = element->Attribute("fill");
    if (fillAttr && std::string(fillAttr) != "none") {
        hasFill = true;
    }

    const char* strokeAttr = element->Attribute("stroke");
    if (strokeAttr && std::string(strokeAttr) != "none") {
        hasStroke = true;
    }

    float strokeWidthF = 1.0f;
    element->QueryFloatAttribute("stroke-width", &strokeWidthF);
    strokeWidth = static_cast<int>(strokeWidthF);
    this->shape::setValue(element);

    const char* dAttr = element->Attribute("d");
    if (!dAttr) return;

    std::string d(dAttr);
    std::vector<std::string> tokens;
    std::string current;
    for (char c : d) {
        if (std::isalpha(c)) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
            tokens.push_back(std::string(1, c)); // push command
        }
        else if (std::isdigit(c) || c == '.' || c == '-' || c == '+') { // handle numbers
            current += c;
        }
        else if (c == ',' || std::isspace(c)) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
        }
    }
    if (!current.empty()) tokens.push_back(current);

    // Parser
    size_t i = 0;
    while (i < tokens.size()) {
        std::string cmd = tokens[i++];
        if (cmd == "M") {
            float x = std::stof(tokens[i++]);
            float y = std::stof(tokens[i++]);
            commands.push_back(new MoveTo(x, y));
        }
        else if (cmd == "L") {
            while (i + 1 < tokens.size() && !std::isalpha(tokens[i][0])) {
                float x = std::stof(tokens[i++]);
                float y = std::stof(tokens[i++]);
                commands.push_back(new Lineto(x, y));
            }
        }
        else if (cmd == "H") {
            while (i < tokens.size() && !std::isalpha(tokens[i][0])) {
                float x = std::stof(tokens[i++]);
                commands.push_back(new HLineTo(x));
            }
        }
        else if (cmd == "V") {
            while (i < tokens.size() && !std::isalpha(tokens[i][0])) {
                float y = std::stof(tokens[i++]);
                commands.push_back(new VLineTo(y));
            }
        }
        else if (cmd == "C") {
            while (i + 5 < tokens.size() && !std::isalpha(tokens[i][0])) {
                float x1 = std::stof(tokens[i++]);
                float y1 = std::stof(tokens[i++]);
                float x2 = std::stof(tokens[i++]);
                float y2 = std::stof(tokens[i++]);
                float x = std::stof(tokens[i++]);
                float y = std::stof(tokens[i++]);
                commands.push_back(new CurveTo(x1, y1, x2, y2, x, y));
            }
        }
        else if (cmd == "Z" || cmd == "z") {
            commands.push_back(new ClosePath());
        }
    }
}



void path::draw(HDC hdc) {
    Graphics graphics(hdc);
    GraphicsPath path;
    // Draw the path commands
    for (auto cmd : commands) {
        cmd->draw(&graphics, &path);
    }
    // Fill if has fill
    if (hasFill) {
        SolidBrush brush(Color(static_cast<BYTE>(fillOpacity * 255), fill.GetR(), fill.GetG(), fill.GetB()));
        graphics.FillPath(&brush, &path);
    }
    // Draw stroke if has stroke
    if (hasStroke) {
        Pen pen(stroke, static_cast<float>(strokeWidth));
        graphics.DrawPath(&pen, &path);
    }
}
