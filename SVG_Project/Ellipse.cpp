#include "Ellipse.h"


//Ellipse
ellipse::ellipse() {
	this->height = this->width = 0;
}

void ellipse::setValue(tinyxml2::XMLElement* element) {
	this->shape::setValue(element);
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
	Graphics graphic(hdc);
	//Trans
	GraphicsState state = graphic.Save();
	this->handleTransform(&graphic);

	SolidBrush brush(Color(this->fillOpacity * 255, this->fill.GetR(), this->fill.GetG(), this->fill.GetB()));
	graphic.FillEllipse(&brush, this->origin.X, this->origin.Y, this->width, this->height);
	//Draw Outline
	Pen pen(Color(this->strokeOpacity * 255, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB()));
	pen.SetWidth(this->strokeWidth);
	graphic.DrawEllipse(&pen, this->origin.X, this->origin.Y, this->width, this->height);
}

//Circle

void circle::setValue(tinyxml2::XMLElement* element) {
	this->shape::setValue(element);
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