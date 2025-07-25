#include "Rectangle.h"


//Rectangle

rectangle::rectangle() {
	this->origin.X = this->origin.Y = 0;
	this->width = this->height = 0;
}

void rectangle::setValue(tinyxml2::XMLElement* element) {
	this->shape::setValue(element);
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

void rectangle::draw(HDC hdc) {
	Graphics graphic(hdc);
	//Fill
	SolidBrush brush(Color(this->fillOpacity * 255, this->fill.GetR(), this->fill.GetG(), this->fill.GetB()));
	graphic.FillRectangle(&brush, this->origin.X, this->origin.Y, this->width, this->height);
	//Draw Outline
	Pen pen(Color(this->strokeOpacity * 255, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB()));
	pen.SetWidth(this->strokeWidth);
	graphic.DrawRectangle(&pen, this->origin.X, this->origin.Y, this->width, this->height);
}







