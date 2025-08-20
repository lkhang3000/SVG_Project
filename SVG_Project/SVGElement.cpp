#include "SVGElement.h"

//SVGContainer
SVGElement::SVGElement() {
    this->transform = "";
    this->fillOpacity = this->strokeOpacity = 1;
    this->strokeWidth = 1;
	this->fill = Color(0, 0, 0);
	this->stroke = Color(0, 0, 0, 0);
    this->fillID = this->strokeID = "";
}
void SVGElement::handleTransform(Graphics* graphics) {
    //graphics->ScaleTransform(5.0f, 5.0f);
    
    if (this->transform == "") return;
    string trans = this->transform;

    int pos = trans.find("translate(");
    if (pos != string::npos) {
        size_t start = pos + 10;
        size_t end = trans.find(")", start);
        if (end != string::npos) {
            string parameter = trans.substr(start, end - start);
            size_t comma = parameter.find(',');
            if (comma != string::npos) {
                double x = stod(parameter.substr(0, comma));
                double y = stod(parameter.substr(comma + 1));
                graphics->TranslateTransform((REAL)x, (REAL)y);
            }
        }

    }

    pos = trans.find("scale(");
    if (pos != string::npos) {
        size_t start = pos + 6;
        size_t end = trans.find(")", start);
        if (end != string::npos) {
            string parameter3 = trans.substr(start, end - start);
            size_t comma = parameter3.find(',');
            if (comma != string::npos) {
                double x2 = stod(parameter3.substr(0, comma));
                double y2 = stod(parameter3.substr(comma + 1));
                graphics->ScaleTransform((REAL)x2, (REAL)y2);
            }
            else {
                double scale = stod(parameter3);
                graphics->ScaleTransform((REAL)scale, (REAL)scale);
            }
        }
    }

    pos = trans.find("rotate(");
    if (pos != string::npos) {
        size_t start = pos + 7;
        size_t end = trans.find(")", start);
        if (end != string::npos) {
            string parameter2 = trans.substr(start, end - start);
            double angle = stod(parameter2);
            if (angle != 0 && angle != -360 && angle != 360) {
                graphics->RotateTransform((REAL)angle);
            }
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
                string sVal(val);
                if (sVal.find("url") != string::npos) this->strokeID = sVal;
                else if (sVal == "black") {
                    this->stroke = Color(0, 0, 0);
                }
                else if (sVal == "red") {
                    this->stroke = Color(255, 0, 0);
                }
                else if (sVal == "green") {
                    this->stroke = Color(0, 255, 0);
                }
                else if (sVal == "yellow") {
                    this->stroke = Color(255, 255, 0);
                }
                else if (sVal == "blue") {
                    this->stroke = Color(0, 0, 255);
                }
                else if (sVal == "midnightblue") {
                    this->stroke = Color(25, 25, 112);
                }
                else if (sVal == "darkslategray") {
                    this->stroke = Color(47, 79, 79);
                }
                else if (sVal == "blueviolet") {
                    this->stroke = Color(138, 43, 226);
                }
                else if (sVal == "CYAN") {
                    this->stroke = Color(0, 255, 255);
                }
                else if (sVal == "navy") {
                    this->stroke = Color(0, 0, 128);
                }
                else if (sVal == "purple") {
					this->stroke = Color(128, 0, 128);
				}
                else if (sVal == "darkmagenta") {
                    this->stroke = Color(139, 0, 139);
                }
                else if (sVal == "white") {
                    this->stroke = Color(255, 255, 255);
                }
                else if (sVal == "none") {
                    this->stroke = Color(0, 0, 0, 0);
                }
                else if (sVal[0] == '#') {
                    string hex = sVal.substr(1);
                    if (hex.length() == 3) {
                        hex = string(2, hex[0]) + string(2, hex[1]) + string(2, hex[2]);
                    }

                    if (hex.length() == 6) {
                        int r = stoi(hex.substr(0, 2), 0, 16);
                        int g = stoi(hex.substr(2, 2), 0, 16);
                        int b = stoi(hex.substr(4, 2), 0, 16);
                        this->stroke = Color(r, g, b);
                    }
                }
                else {
                    stringstream s(val);
                    string token = "";
                    getline(s, token, '(');
                    getline(s, token, ',');
                    BYTE red = stoi(token);
                    getline(s, token, ',');
                    BYTE green = stoi(token);
                    getline(s, token, ')');
                    if (!token.empty() && token.back() == ')') token.pop_back();
                    BYTE blue = stoi(token);
                    this->stroke = Color(red, green, blue);
                }
            }
            else if (attr == "fill") {
                string sval(val);
                if (sval.find("url") != string::npos) {
                    // Extract the ID from url(#id)
                    size_t start = sval.find('#');
                    size_t end = sval.find(')', start);
                    if (start != string::npos && end != string::npos) {
                        this->fillID = sval.substr(start + 1, end - start - 1);
                    }
                }
                else if (sval == "black") {
                    this->fill = Color(0, 0, 0);
                }
                else if (sval == "red") {
                    this->fill = Color(255, 0, 0);
                }
                else if (sval == "green") {
                    this->fill = Color(0, 255, 0);
                }
                else if (sval == "yellow") {
                    this->fill = Color(255, 255, 0);
                }
                else if (sval == "blue") {
                    this->fill = Color(0, 0, 255);
                }
                else if (sval == "grey") {
                    this->fill = Color(128, 128, 128);
                }
                else if (sval == "midnightblue") {
                    this->fill = Color(25, 25, 112);
                }
                else if (sval == "darkslategray") {
                    this->fill = Color(47, 79, 79);
                }
                else if (sval == "blueviolet") {
                    this->fill = Color(138, 43, 226);
                }
                else if (sval == "CYAN") {
                    this->fill = Color(0, 255, 255);
                }
                else if (sval == "navy") {
                    this->fill = Color(0, 0, 128);
                }
                else if (sval == "deepskyblue") {
					this->fill = Color(0, 191, 255);
                }
                else if (sval == "skyblue") {
                    this->fill = Color(135, 206, 235);
                }
                else if (sval == "black") {
                    this->fill = Color(0, 0, 0);
                }
                else if (sval == "none") {
                    this->fill = Color(0, 0, 0, 0);
                }
                else if (sval[0] == '#') {
                    string hex = sval.substr(1);
                    if (hex.length() == 3) {
                        hex = string(2, hex[0]) + string(2, hex[1]) + string(2, hex[2]);
                    }

                    if (hex.length() == 6) {
                        int r = stoi(hex.substr(0, 2), 0, 16);
                        int g = stoi(hex.substr(2, 2), 0, 16);
                        int b = stoi(hex.substr(4, 2), 0, 16);
                        this->fill = Color(r, g, b);
                    }
                }
                else {
                    stringstream s(val);
                    string token = "";
                    getline(s, token, '(');
                    getline(s, token, ',');
                    BYTE red = stoi(token);
                    getline(s, token, ',');
                    BYTE green = stoi(token);
                    getline(s, token, ')');
                    if (!token.empty() && token.back() == ')') token.pop_back();
                    BYTE blue = stoi(token);
                    this->fill = Color(red, green, blue);
                }
            }
            else if (attr == "transform") this->transform = val;
        }
    }
}


