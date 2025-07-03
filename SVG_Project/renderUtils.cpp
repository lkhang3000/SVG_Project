#include "stdafx.h"
#include "renderUtils.h"

bool SVGRender::loadFile(const string& fileName) {
	cout << "[SVGRender] Loading file: " << fileName << endl;
	for (auto element : elements) {
		delete element;
	}
	elements.clear();

	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	XMLError result = doc->LoadFile(fileName.c_str());

	if (result != XML_SUCCESS) {
		delete doc;
		return false;
	}

	XMLElement* root = doc->RootElement();
	if (root == NULL) {
		delete doc;
		return false;
	}

	Parse(root);
	delete doc;
	return true;
}

void SVGRender::Parse(XMLElement* element) {
	if (element == NULL) {
		return;
	}

	const char* tagName = element->Name();
	cout << "[Parse] Element: " << tagName << endl;

	const char* tag = element->Name();
	SVGElement* svgElement = nullptr;

	if (strcmp(tagName, "circle") == 0) {
		svgElement = new Circle(element);
		elements.push_back(svgElement);
	} else if (strcmp(tagName, "ellipse") == 0) {
		svgElement = new Eclipse(element);
		elements.push_back(svgElement);
	}

	for (XMLElement* child = element->FirstChildElement(); child != NULL; child = child->NextSiblingElement()) {
		Parse(child);
	}

}

void SVGRender::Render(Graphics& graphic) {
	
	SolidBrush backgroundBrush(Color(255, 255, 255, 255));
	graphic.FillRectangle(&backgroundBrush, 0, 0, 10000, 10000); 

	graphic.SetSmoothingMode(SmoothingModeAntiAlias);
	graphic.SetTextRenderingHint(TextRenderingHintAntiAlias);

	for (SVGElement* element : elements) {
		element->Render(graphic);
	}
}

SVGRender::~SVGRender() {
	for (auto element : elements) {
		delete element;
	}
	elements.clear();
}