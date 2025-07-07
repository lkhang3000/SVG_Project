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
text::text() {
	this->origin.X = this->origin.Y = 0;
	this->content = "";
	this->fontSize = this->fillOpacity = 1;
}

void text::setValue(tinyxml2::XMLElement* element) {
	const char* attrs[] = { "x", "y", "font-size", "fill-opacity", "fill" };
	for (auto attr : attrs) {
		const char* val = element->Attribute(attr);
		if (val) {
			if (attr == "x") this->origin.X = atoi(val);
			else if (attr == "y") this->origin.Y = atoi(val);
			else if (attr == "font-size") this->fontSize = atof(val);
			else if (attr == "fill-opacity") this->fillOpacity = atof(val);
			else if (attr == "fill") {
				stringstream s(val);
				string token = "";
				getline(s, token, '(');
				getline(s, token, ',');
				BYTE red = stoi(token);
				getline(s, token, ',');
				BYTE green = stoi(token);
				getline(s, token, ')');
				BYTE blue = stoi(token);
				this->fill = Color(red, green, blue);
			}
		}
	}
	this->content = element->GetText();
}

void text::draw(HDC hdc) {
	Graphics g(hdc);
	//Setting up Brush
	SolidBrush brush(Color(this->fillOpacity * 255, this->fill.GetR(), this->fill.GetG(), this->fill.GetB()));
	//Setting up Font
	FontFamily fontFamily(L"Arial");
	Font font(&fontFamily, this->fontSize, FontStyleRegular, UnitPixel);
	//Converting string to Wstring to fit the arguments of Draw function
	wstring wideString = this->Utf8ToWstring(this->content);
	//Draw
	g.DrawString(wideString.c_str(), -1, &font, PointF(this->origin.X, this->origin.Y), &brush);
}
