#include "Line.h"

//Lines

line::line() {
    this->vertices.resize(2);
    this->strokeOpacity = this->strokeWidth = 1;
}

void line::setValue(tinyxml2::XMLElement* element) {
    const char* attrs[] = { "stroke", "stroke-width", "stroke-opacity" };
    for (auto attr : attrs) {
        const char* val = element->Attribute(attr);
        if (val) {
            if (attr == "stroke-opacity") this->strokeOpacity = atof(val);
            else if (attr == "stroke-width") this->strokeWidth = atoi(val);
            else if (attr == "stroke") {
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
    }
    this->vertices[0].X = atoi(element->Attribute("x1"));
    this->vertices[0].Y = atoi(element->Attribute("y1"));
    this->vertices[1].X = atoi(element->Attribute("x2"));
    this->vertices[1].X = atoi(element->Attribute("y2"));
}

void line::draw(HDC hdc) {
    Graphics g(hdc);
    //Draw Line
    Pen pen(Color(this->strokeOpacity * 255, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB()));
    pen.SetWidth(this->strokeWidth);
    g.DrawLine(&pen, this->vertices.front(), this->vertices.back());
}

