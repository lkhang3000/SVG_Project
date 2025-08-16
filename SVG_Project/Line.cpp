#include "Line.h"

//Lines

line::line() {
    this->vertices.resize(2);
    this->strokeOpacity = this->strokeWidth = 1;
}

void line::setValue(tinyxml2::XMLElement* element) {
    this->SVGElement::setValue(element);
    const char* attrs[] = {"x1", "y1", "x2", "y2" };
    for (auto attr : attrs) {
        const char* val = element->Attribute(attr);
        if (val) {
            if (attr == "x1") this->vertices[0].X = atoi(element->Attribute("x1"));
            else if (attr == "y1") this->vertices[0].Y = atoi(element->Attribute("y1"));
            else if (attr == "x2") this->vertices[1].X = atoi(element->Attribute("x2"));
            else if (attr == "y2") this->vertices[1].Y = atoi(element->Attribute("y2"));
        }
    }
}

void line::draw(HDC hdc, gradientDatabase& database) {
    Graphics g(hdc);
    //Draw Line
    this->draw(&g, database);
}
void line::draw(Graphics* g, gradientDatabase& database) {
    this->handleTransform(g);
    
    //Brush
    Brush* brush = NULL;
    if (this->fillID != "") brush = database.getBrush(this->fillID);
    else brush = new SolidBrush(Color(this->fillOpacity * 255, this->fill.GetR(), this->fill.GetG(), this->fill.GetB()));
    //Pen
    Pen pen(Color(this->strokeOpacity * 255, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB()));
    pen.SetWidth(this->strokeWidth);
    if (brush != NULL) pen.SetBrush(brush);
    g->DrawLine(&pen, this->vertices.front(), this->vertices.back());
    delete brush;
}

