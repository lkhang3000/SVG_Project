#include "stdafx.h"
#include "Shape.h"

bool SVGElement::ParseColor(string colType, SVGColor& color) {
	if (colType == "") {
		return false;
	}

	/*if (colType == "red") {
		color.r = 255;
		color.g = 0;
		color.b = 0;
		return true;
	}
	else if (colType == "green") {
		color.r = 0;
		color.g = 255;
		color.b = 0;
		return true;
	}
	else if (colType == "blue") {
		color.r = 0;
		color.g = 0;
		color.b = 255;
		return true;
	}
	else if (colType == "black") {
		color.r = 0;
		color.g = 0;
		color.b = 0;
		return true;
	}*/
	std::smatch match;
	std::regex rgbRegex(R"(rgb\(\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)\s*\))");

	if (std::regex_match(colType, match, rgbRegex)) {
		color.r = std::stoi(match[1].str());
		color.g = std::stoi(match[2].str());
		color.b = std::stoi(match[3].str());
		color.a = 255; 
		return true;
	}

	return false;
}


Circle::Circle(XMLElement* elem) {
	x = elem->FloatAttribute("cx", 0);
	y = elem->FloatAttribute("cy", 0);
	r = elem->FloatAttribute("r", 0);
	strokeWid = elem->FloatAttribute("stroke-width", 1);

	checkFill = SVGElement::ParseColor(elem->Attribute("fill"), fillCol);
	checkStroke = SVGElement::ParseColor(elem->Attribute("stroke"), strokeCol);

	fillOpa = elem->FloatAttribute("fill-opacity", 1.0);
	strokeOpa = elem->FloatAttribute("stroke-opacity", 1.0);

	fillCol.a = fillOpa * 255;
	strokeCol.a = strokeOpa * 255;
}

void Circle::Render(Graphics& graphic) {
	float diameter = r * 2;
	float cx = x - r;
	float cy = y - r;

	if (checkFill) {
		SolidBrush brush(Color(fillCol.a, fillCol.r, fillCol.g, fillCol.b));
		graphic.FillEllipse(&brush, cx, cy, diameter, diameter);
	}

	if (checkStroke) {
		Pen pen(Color(strokeCol.a, strokeCol.r, strokeCol.g, strokeCol.b), strokeWid);
		graphic.DrawEllipse(&pen, cx, cy, diameter, diameter);
	}
}

Eclipse::Eclipse(XMLElement* elem) {
	cx = elem->FloatAttribute("cx", 0);
	cy = elem->FloatAttribute("cy", 0);
	rx = elem->FloatAttribute("rx", 0);
	ry = elem->FloatAttribute("ry", 0);

	strokeWid = elem->FloatAttribute("stroke-width", 1);

	checkFill = SVGElement::ParseColor(elem->Attribute("fill"), fillCol);
	checkStroke = SVGElement::ParseColor(elem->Attribute("stroke"), strokeCol);

	fillOpa = elem->FloatAttribute("fill-opacity", 1.0);
	strokeOpa = elem->FloatAttribute("stroke-opacity", 1.0);

	fillCol.a = fillOpa * 255;
	strokeCol.a = strokeOpa * 255;
}

void Eclipse::Render(Graphics & graphic) {
	float w = rx * 2;
	float h = ry * 2;
	float x = cx - rx;
	float y = cy - ry;
	

	if (checkFill) {
		SolidBrush brush(Color(fillCol.a, fillCol.r, fillCol.g, fillCol.b));
		graphic.FillEllipse(&brush, x, y, w, h);
	}

	if (checkStroke) {
		Pen pen(Color(strokeCol.a, strokeCol.r, strokeCol.g, strokeCol.b), strokeWid);
		graphic.DrawEllipse(&pen, x, y, w, h);
	}
}