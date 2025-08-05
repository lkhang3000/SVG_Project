#include "Group.h"

SVGGroup::SVGGroup() {
	this->fillOpacity = this->strokeOpacity = 1;
	this->strokeWidth = 1;
}
void SVGGroup::draw(HDC hdc) {
	Graphics g(hdc);
	this->draw(&g);
}
void SVGGroup::draw(Graphics* g) {
	this->handleTransform(g);
	for (int i = 0; i < this->content.size(); i++) {
		this->content[i]->draw(g);
	}
}

void SVGGroup::setValue(tinyxml2::XMLElement* element) {
	const char* attrs[] = { "fill", "fill-opacity", "stroke", "stroke-width", "stroke-opacity", "transform" };
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
			else if (attr == "transform") this->transform = val;
		}
	}
	addElements(element);
}

void SVGGroup::addElements(tinyxml2::XMLElement* element) {
	if (element->FirstChildElement() == NULL) return;
	for (tinyxml2::XMLElement* child = element->FirstChildElement(); child != NULL; child = child->NextSiblingElement()) {
		SVGElement* newElement = NULL;
		string elementName = child->Name();
		if (elementName == "g") {
			newElement = new SVGGroup;
			const char* attrs[] = { "fill", "fill-opacity", "stroke", "stroke-width", "stroke-opacity" };
			for (auto attr : attrs) { //add all "father" group attributes into "son" group attributes, so that the "father"s attributes do affect the "son".
				const char* val = element->Attribute(attr);
				if (val) child->SetAttribute(attr, val);
			}
			newElement->setValue(child);
			this->content.push_back(newElement);
			continue;
		}
		else if (elementName == "text") newElement = new text;
		else if (elementName == "line") newElement = new line;
		else if (elementName == "polyline") newElement = new polyline;
		else if (elementName == "polygon") newElement = new polygon;
		else if (elementName == "rect") newElement = new rectangle;
		else if (elementName == "ellipse") newElement = new ellipse;
		else if (elementName == "circle") newElement = new circle;
		else if (elementName == "path") newElement = new path;
		newElement->setValue(element);
		newElement->deleteTransform();
		newElement->setValue(child);
		this->content.push_back(newElement);
	}
}