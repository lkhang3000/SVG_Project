#include "Shape.h"

//Shape

shape::shape() {
	this->fillOpacity = this->strokeOpacity = 1;
	this->strokeWidth = 1;
}

void shape::setValue(tinyxml2::XMLElement* element) {
	const char* attrs[] = { "fill", "fill-opacity", "stroke", "stroke-width", "stroke-opacity" };
	for (auto attr : attrs) {
		const char* val = element->Attribute(attr);
		if (val) {
			if (attr == "fill-opacity") this->fillOpacity = atof(val);
			else if (attr == "stroke-opacity") this->strokeOpacity = atof(val);
			else if (attr == "stroke-width") this->strokeWidth = atoi(val);
			else if (attr == "stroke") {
				stringstream s(val);
				string token = "";
				getline(s, token, '(');
				getline(s, token, ',');
				BYTE red = stoi(token);
				getline(s, token, ',');
				BYTE green = stoi(token);
				getline(s, token, ')');
				BYTE blue = stoi(token);
				this->stroke = Color(red, green, blue);
			}
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
}