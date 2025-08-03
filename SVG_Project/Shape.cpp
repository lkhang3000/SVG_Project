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
