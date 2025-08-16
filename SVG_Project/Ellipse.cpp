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

void ellipse::draw(HDC hdc, gradientDatabase& database) {
	Graphics g(hdc);
	this->draw(&g, database);
}

void ellipse::draw(Graphics* g, gradientDatabase& database) {
    this->handleTransform(g);
	Brush* brush = NULL;
	//Fill
	if (this->fillID != "") brush = database.getBrush(this->fillID);
    else if (this->fill.GetA() > 0) brush = new SolidBrush(Color(this->fillOpacity * 255, this->fill.GetR(), this->fill.GetG(), this->fill.GetB()));
	if (brush != NULL) g->FillEllipse(brush, static_cast<INT>(this->origin.X - this->width / 2), static_cast<INT>(this->origin.Y - this->height / 2), static_cast<INT>(this->width), static_cast<INT>(this->height));
    delete brush;
	//Stroke
    if (this->stroke.GetA() > 0 && this->strokeWidth > 0) {
        Pen pen(Color(this->strokeOpacity * 255, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB()));
        pen.SetWidth(this->strokeWidth);
        g->DrawEllipse(&pen, static_cast<INT>(this->origin.X - this->width / 2), static_cast<INT>(this->origin.Y - this->height / 2), static_cast<INT>(this->width), static_cast<INT>(this->height));
    }
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
			else if (attr == "r") this->width = this->height = atoi(val) * 2;
		}
	}
}
