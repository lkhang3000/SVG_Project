#include "Polygon.h"

//Polygon
polygon::polygon() {
	this->vertices.resize(0);
}

void polygon::draw(HDC hdc) {
	Graphics graphic(hdc);
	//Fill
	SolidBrush brush(Color(this->fillOpacity * 255, this->fill.GetR(), this->fill.GetG(), this->fill.GetB()));
	graphic.FillPolygon(&brush, &this->vertices.front(), this->vertices.size());
	//Draw Outline
	Pen pen(Color(this->strokeOpacity * 255, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB()));
	pen.SetWidth(this->strokeWidth);
	graphic.DrawPolygon(&pen, &this->vertices.front(), this->vertices.size());
}

void polygon::setValue(tinyxml2::XMLElement* element) {
	this->shape::setValue(element);
	stringstream s(element->Attribute("points"));
	string token = "";
	while (getline(s, token, ' ')) {
		Point newVertex(stoi(token), stoi(token.substr(token.find_first_of(',') + 1)));
		this->vertices.push_back(newVertex);
	}
}