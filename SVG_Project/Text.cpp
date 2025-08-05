#include "Text.h"

//Private
//Convert String to WString
wstring text::Utf8ToWstring(const string& str) {
	if (str.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}


//Public
text::text() : SVGElement() {
	this->origin.X = this->origin.Y = 0;
	this->content = "";
	this->fontSize = 1;
}

void text::setValue(tinyxml2::XMLElement* element) {
	this->SVGElement::setValue(element);
	const char* attrs[] = { "x", "y", "font-size" };
	for (auto attr : attrs) {
		const char* val = element->Attribute(attr);
		if (val) {
			if (attr == "x") this->origin.X = atoi(val);
			else if (attr == "y") this->origin.Y = atoi(val);
			else if (attr == "font-size") this->fontSize = atof(val);
		}
	}
	if (element->GetText() != NULL) this->content = element->GetText();
}

void text::draw(HDC hdc) {
	Graphics g(hdc);
	this->draw(&g);
}

void text::draw(Graphics* g) {
	this->handleTransform(g);
	GraphicsPath outline;
	//Setting up Brush
	SolidBrush brush(Color(this->fillOpacity * 255, this->fill.GetR(), this->fill.GetG(), this->fill.GetB()));
	//Setting up Pen
	Pen pen(Color(this->strokeOpacity * 255, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB()));
	pen.SetWidth(this->strokeWidth);
	//Setting up Font
	FontFamily fontFamily(L"Arial");
	Font font(&fontFamily, this->fontSize, FontStyleRegular, UnitPixel);
	//Converting string to Wstring to fit the arguments of Draw function
	wstring wideString = this->Utf8ToWstring(this->content);
	//Draw
	//Draw Text Outline
	FontFamily family;
	font.GetFamily(&family);
	outline.AddString(wideString.c_str(), -1, &family, font.GetStyle(), font.GetSize(), PointF(this->origin.X, this->origin.Y), NULL);
	g->DrawPath(&pen, &outline);
	//Draw Fill
	g->DrawString(wideString.c_str(), -1, &font, PointF(this->origin.X, this->origin.Y), &brush);
	
}

