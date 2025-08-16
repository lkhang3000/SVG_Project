#include "Rectangle.h"


//Rectangle

rectangle::rectangle() : SVGElement() {
	this->origin.X = this->origin.Y = 0;
	this->width = this->height = 0;
}

void rectangle::setValue(tinyxml2::XMLElement* element) {
	this->SVGElement::setValue(element);
	const char* attrs[] = { "x", "y", "width", "height" };
	for (auto attr : attrs) {
		const char* val = element->Attribute(attr);
		if (val) {
			if (attr == "x") this->origin.X = atoi(val);
			else if (attr == "y") this->origin.Y = atoi(val);
			else if (attr == "width") this->width = atoi(val);
			else if (attr == "height") this->height = atoi(val);
		}
	}
}

void rectangle::draw(HDC hdc, gradientDatabase& database) {
	Graphics g(hdc);
	this->draw(&g, database);
}

void rectangle::draw(Graphics* g, gradientDatabase& database) {
	this->handleTransform(g);
	//Fill
	Brush* brush = NULL;
	if (this->fillID != "") brush = database.getBrush(this->fillID);
	else if (this->fill.GetA() > 0) brush = new SolidBrush(Color(this->fillOpacity * 255, this->fill.GetR(), this->fill.GetG(), this->fill.GetB()));
	if (brush != NULL) g->FillRectangle(brush, this->origin.X, this->origin.Y, this->width, this->height);
	delete brush;
	//Draw Outline
	if (this->stroke.GetA() > 0 && this->strokeWidth > 0) {
		Pen pen(Color(this->strokeOpacity * 255, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB()));
		pen.SetWidth(this->strokeWidth);
		g->DrawRectangle(&pen, this->origin.X, this->origin.Y, this->width, this->height);
	}
}








