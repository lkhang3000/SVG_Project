#include "Rectangle.h"

//ShapeList (using linked list to contain SVGElements)

shapeList::shapeList() {
	this->pHead = this->pTail = NULL;
}

void shapeList::addFirst(tinyxml2::XMLElement* data) {
	this->pHead = this->pTail = new shapeListNode;
	this->pHead->addNextShape(data);
}

void shapeList::addTail(tinyxml2::XMLElement* data) {
	if (this->pHead == NULL) this->addFirst(data);
	else {
		shapeListNode* pTemp = this->pTail;
		this->pTail = new shapeListNode;
		this->pTail->addNextShape(data);
		pTemp->addNextShape(this->pTail);
	}
}

void shapeList::drawAllShape(HDC hdc) {
	if (this->pHead == NULL) return;
	for (shapeListNode* pTemp = this->pHead; pTemp != NULL; pTemp = pTemp->getNextNode()) {
		pTemp->drawShape(hdc);
	}
}

shapeList::~shapeList() {
	delete this->pHead;
	this->pHead = this->pTail = NULL;
}


//ShapeListNode

shapeListNode::shapeListNode() {
	this->data = NULL;
	this->pNext = NULL;
}

void shapeListNode::addNextShape(tinyxml2::XMLElement* data) {
	string name = data->Name();
	if (name == "rect") this->data = new rectangle;
	else this->data = new polygon;
	this->data->setValue(data);
}

void shapeListNode::addNextShape(shapeListNode* data) {
	this->pNext = data;
}

void shapeListNode::drawShape(HDC hdc) {
	this->data->draw(hdc);
}

shapeListNode* shapeListNode::getNextNode() {
	return this->pNext;
}

shapeListNode::~shapeListNode() {
	delete this->data;
	this->data = NULL;
	delete this->pNext;
	this->pNext = NULL;
}

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

//Rectangle

rectangle::rectangle() {
	this->origin.X = this->origin.Y = 0;
	this->width = this->height = 0;
}

void rectangle::draw(HDC hdc) {
	Graphics graphic(hdc);
	//Fill
	SolidBrush brush(Color(this->fillOpacity * 255, this->fill.GetR(), this->fill.GetB(), this->fill.GetG()));
	graphic.FillRectangle(&brush, this->origin.X, this->origin.Y, this->width, this->height);
	//Draw Outline
	Pen pen(Color(this->strokeOpacity * 255, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB()));
	pen.SetWidth(this->strokeWidth);
	graphic.DrawRectangle(&pen, this->origin.X, this->origin.Y, this->width, this->height);
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
			/*else if (attr == "fill-opacity") this->fillOpacity = atof(val);
			else if (attr == "stroke-opacity") this->strokeOpacity = atof(val);
			else if (attr == "stroke-width") this->strokeWidth = atoi(val);
			else if (attr == "stroke") {
				stringstream s(val);
				string token = "";
				getline(s, token, '(');
				s.ignore(1);
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
				s.ignore(1);
				getline(s, token, ',');
				BYTE red = stoi(token);
				getline(s, token, ',');
				BYTE green = stoi(token);
				getline(s, token, ')');
				BYTE blue = stoi(token);
				this->fill = Color(red, green, blue);
			}*/
		}
	}
}


//Polygon
polygon::polygon() {
	this->vertices.resize(0);
}

void polygon::draw(HDC hdc) {
	Graphics graphic(hdc);
	//Fill
	SolidBrush brush(Color(this->fillOpacity * 255, this->fill.GetR(), this->fill.GetG(), this->fill.GetB()));
	graphic.FillPolygon(&brush, &this->vertices.front(), this->vertices.size());
	//Draw Outline
	Pen pen(Color(this->strokeOpacity * 255, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB()));
	pen.SetWidth(this->strokeWidth);
	graphic.DrawPolygon(&pen, &this->vertices.front() , this->vertices.size());
}

void polygon::setValue(tinyxml2::XMLElement* element) {
	this->shape::setValue(element);
	stringstream s(element->Attribute("points"));
	string token = "";
	while (getline(s, token, ' ')) {
		Point newVertex(stoi(token), stoi(token.substr(token.find_first_of(',') + 1)));
		this->vertices.push_back(newVertex);
	}
}


