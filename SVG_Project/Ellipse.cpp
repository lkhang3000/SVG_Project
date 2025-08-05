#include "Ellipse.h"


//Ellipse
ellipse::ellipse() : SVGElement() {
	this->height = this->width = 0;
}

void ellipse::setValue(tinyxml2::XMLElement* element) {
	this->SVGElement::setValue(element);
	const char* attrs[] = { "cx", "cy", "rx", "ry" };
	for (auto attr : attrs) {
		const char* val = element->Attribute(attr);
		if (val) {
			if (attr == "cx") this->origin.X = atoi(val);
			else if (attr == "cy") this->origin.Y = atoi(val);
			else if (attr == "rx") this->width = atoi(val);
			else if (attr == "ry") this->height = atoi(val);
		}
	}
}

void ellipse::draw(HDC hdc) {
	Graphics g(hdc);
	this->draw(&g);
}

void ellipse::draw(Graphics* g) {
	this->handleTransform(g);
	//Fill
	SolidBrush brush(Color(this->fillOpacity * 255, this->fill.GetR(), this->fill.GetG(), this->fill.GetB()));
	g->FillEllipse(&brush, this->origin.X, this->origin.Y, this->width, this->height);
	//Draw Outline
	Pen pen(Color(this->strokeOpacity * 255, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB()));
	pen.SetWidth(this->strokeWidth);
	g->DrawEllipse(&pen, this->origin.X, this->origin.Y, this->width, this->height);
}

//Circle
circle::circle() : ellipse() {}

void circle::setValue(tinyxml2::XMLElement* element) {
	this->SVGElement::setValue(element);
	const char* attrs[] = { "cx", "cy", "r" };
	for (auto attr : attrs) {
		const char* val = element->Attribute(attr);
		if (val) {
			if (attr == "cx") this->origin.X = atoi(val);
			else if (attr == "cy") this->origin.Y = atoi(val);
			else if (attr == "r") this->width = this->height = atoi(val);
		}
	}
}
