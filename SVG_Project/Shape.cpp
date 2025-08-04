#include "Shape.h"
#include <sstream>
#include <string>

using namespace std;

shape::shape() {
    this->fillOpacity = this->strokeOpacity = 1;
    this->strokeWidth = 1;
    this->hasFill = false;
    this->hasStroke = false;
    this->fill = Color(255, 255, 255, 255);   
    this->stroke = Color(255, 0, 0, 0);       
}

void shape::setValue(tinyxml2::XMLElement* element) {
    const char* attrs[] = { "fill", "fill-opacity", "stroke", "stroke-width", "stroke-opacity" };
    for (auto attr : attrs) {
        const char* val = element->Attribute(attr);
        if (!val) continue;

        string valueStr = val;
        if (attr == string("fill-opacity")) {
            this->fillOpacity = atof(val);
        }
        else if (attr == string("stroke-opacity")) {
            this->strokeOpacity = atof(val);
        }
        else if (attr == string("stroke-width")) {
            this->strokeWidth = atof(val);
        }
        else if (attr == string("transform")) {
            this->transform = val;
        }
        else if (attr == string("stroke")) {
            if (valueStr == "none") {
                this->hasStroke = false;
            }
            else {
                this->hasStroke = true;
                stringstream s(valueStr);
                string token = "";
                getline(s, token, '('); 
                getline(s, token, ',');
                BYTE red = stoi(token);
                getline(s, token, ',');
                BYTE green = stoi(token);
                getline(s, token, ')');
                BYTE blue = stoi(token);
                this->stroke = Color(BYTE(strokeOpacity * 255), red, green, blue);
            }
        }
        else if (attr == string("fill")) {
            if (valueStr == "none") {
                this->hasFill = false;
            }
            else {
                this->hasFill = true;
                stringstream s(valueStr);
                string token = "";
                getline(s, token, '('); 
                getline(s, token, ',');
                BYTE red = stoi(token);
                getline(s, token, ',');
                BYTE green = stoi(token);
                getline(s, token, ')');
                BYTE blue = stoi(token);
                this->fill = Color(BYTE(fillOpacity * 255), red, green, blue);
            }
        }
    }
}

void shape::handleTransform(Graphics* graphics) {
    string trans = transform;

    int pos = trans.find("translate(");
    if (pos != string::npos) {
        int start = pos + 10;
        int end = trans.find(")", start);
        string parameter = trans.substr(start, end - start);

        int comma = parameter.find(',');
        if (comma != string::npos) {
            double x = stof(parameter.substr(0, comma));
            double y = stof(parameter.substr(comma + 1));
            graphics->TranslateTransform(x, y);
        }

    }

    pos = trans.find("rotate(");
    if (pos != string::npos) {
        int start = pos + 7;
        int end = trans.find(")", start);
        string parameter2 = trans.substr(start, end - start);

        double angle = stof(parameter2);
        graphics->RotateTransform(angle);
    }

    pos = trans.find("scale(");
    if (pos != string::npos) {
        int start = pos + 6;
        int end = trans.find(")", start);
        string parameter3 = trans.substr(start, end - start);

        int comma = parameter3.find(',');
        if (comma != string::npos) {
            double x2 = stof(parameter3.substr(0, comma));
            double y2 = stof(parameter3.substr(comma + 1));
            graphics->ScaleTransform(x2, y2);
        }
        else {
            double scale = stof(parameter3);
            graphics->ScaleTransform(scale, scale);
        }
    }
}
