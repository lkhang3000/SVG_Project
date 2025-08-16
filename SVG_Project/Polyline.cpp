#include "Polyline.h"

//Polylines
polyline::polyline() : SVGElement() {}

void polyline::setValue(tinyxml2::XMLElement* element) {
    this->SVGElement::setValue(element);
    if (element->Attribute("points") != NULL) {
        stringstream s(element->Attribute("points"));
        string token = "";
        while (getline(s, token, ' ')) {
            Point newVertex(stoi(token), stoi(token.substr(token.find_first_of(',') + 1)));
            this->vertices.push_back(newVertex);
        }
    }
}

void polyline::draw(HDC hdc, gradientDatabase& database) {
    Graphics g(hdc);
    this->draw(&g, database);
}

void polyline::draw(Graphics* g, gradientDatabase& database) {
    if (vertices.size() < 2) return;
    this->handleTransform(g);
    //Fill
    Brush* brush = NULL;
    if (this->fillID != "") brush = database.getBrush(this->fillID);
    else brush = new SolidBrush(Color(this->fillOpacity * 255, this->fill.GetR(), this->fill.GetG(), this->fill.GetB()));
    if (brush != NULL) g->FillPolygon(brush, &this->vertices.front(), this->vertices.size());
    delete brush;
    //Draw Line
    Pen pen(Color(this->strokeOpacity * 255, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB()));
    pen.SetWidth(this->strokeWidth);
    g->DrawLines(&pen, &this->vertices.front(), this->vertices.size());
}

