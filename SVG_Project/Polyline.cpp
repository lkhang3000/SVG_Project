#include "Polyline.h"



//Polylines
void polyline::setValue(tinyxml2::XMLElement* element) {
    this->shape::setValue(element);
    stringstream s(element->Attribute("points"));
    string token = "";
    while (getline(s, token, ' ')) {
        Point newVertex(stoi(token), stoi(token.substr(token.find_first_of(',') + 1)));
        this->vertices.push_back(newVertex);
    }
}

void polyline::draw(HDC hdc) {
    if (vertices.size() < 2) return;
    Graphics g(hdc);
    //Fill
    SolidBrush brush(Color(this->fillOpacity * 255, this->fill.GetR(), this->fill.GetG(), this->fill.GetB()));
    g.FillPolygon(&brush, &this->vertices.front(), this->vertices.size());
    //Draw Line
    Pen pen(Color(this->strokeOpacity * 255, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB()));
    pen.SetWidth(this->strokeWidth);
    g.DrawLines(&pen, &this->vertices.front(), this->vertices.size());
}
