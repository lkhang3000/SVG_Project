#include "SVGContainer.h"

void SVGContainer::addElements(const char* fileName) {
	tinyxml2::XMLDocument doc;
	doc.LoadFile(fileName);
	if (doc.ErrorID() != 0) {
		cout << "Error loading SVG file: " << doc.ErrorID() << endl;
		return;
	}
	tinyxml2::XMLElement* root = doc.RootElement();
	if (root == nullptr) {
		cout << "No root element found." << endl;
		return;
	}
	root = root->FirstChildElement();
	if (string(root->Name()) == "defs") {
		this->database.extractData(root);
		root = root->NextSiblingElement();
	}
	for (tinyxml2::XMLElement* child = root; child != NULL; child = child->NextSiblingElement()) {
		this->addElement(child);
	}
}

void SVGContainer::addElement(tinyxml2::XMLElement* element) {
	SVGElement* newElement = NULL;
	string elementName = element->Name();
	if (elementName == "text") newElement = new text;
	else if (elementName == "line") newElement = new line;
	else if (elementName == "polyline") newElement = new polyline;
	else if (elementName == "polygon") newElement = new polygon;
	else if (elementName == "rect") newElement = new rectangle;
	else if (elementName == "ellipse") newElement = new ellipse;
	else if (elementName == "circle") newElement = new circle;
	else if (elementName == "path") newElement = new path;
	else if (elementName == "g") newElement = new SVGGroup;
	newElement->setValue(element);
	this->content.push_back(newElement);
}

void SVGContainer::drawAllElement(HDC hdc) {
	/*Graphics graphics(hdc);
	graphics.ScaleTransform(0.19f, 0.19f);
	for (int i = 0; i < this->content.size(); i++) {
		this->content[i]->draw(&graphics); 
	}*/
	for (int i = 0; i < this->content.size(); i++) this->content[i]->draw(hdc, this->database);
}

SVGContainer::~SVGContainer() {
	for (int i = 0; i < this->content.size(); i++) {
		delete this->content[i];
		this->content[i] = NULL;
	}
}

