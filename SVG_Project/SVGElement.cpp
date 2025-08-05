#include "SVGElement.h"

//SVGContainer
SVGElement::SVGElement() {
    this->transform = "";
    this->fillOpacity = this->strokeOpacity = 1;
    this->strokeWidth = 1;
}
void SVGElement::handleTransform(Graphics* graphics) {
    if (this->transform == "") return;
    string trans = this->transform;

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

void SVGElement::deleteTransform() {
    this->transform = "";
}

void SVGElement::setValue(tinyxml2::XMLElement* element) {
    const char* attrs[] = { "fill", "fill-opacity", "stroke", "stroke-width", "stroke-opacity", "transform" };
    for (auto attr : attrs) {
        const char* val = element->Attribute(attr);
        if (val) {
            if (attr == "fill-opacity") this->fillOpacity = atof(val);
            else if (attr == "stroke-opacity") this->strokeOpacity = atof(val);
            else if (attr == "stroke-width") this->strokeWidth = atoi(val);
            else if (attr == "stroke") {
                if (string(val) != "none") {
                    stringstream s(val);
                    string token = "";
                    getline(s, token, '(');
                    getline(s, token, ',');
                    BYTE red = stoi(token);
                    getline(s, token, ',');
                    BYTE green = stoi(token);
                    getline(s, token, ')');
                    BYTE blue = stoi(token);
                    this->stroke = Color(red, green, blue);
                }
            }
            else if (attr == "fill") {
                if (string(val) != "none") {
                    stringstream s(val);
                    string token = "";
                    getline(s, token, '(');
                    getline(s, token, ',');
                    BYTE red = stoi(token);
                    getline(s, token, ',');
                    BYTE green = stoi(token);
                    getline(s, token, ')');
                    BYTE blue = stoi(token);
                    this->fill = Color(red, green, blue);
                }
            }
            else if (attr == "transform") this->transform = val;
        }
    }
}


