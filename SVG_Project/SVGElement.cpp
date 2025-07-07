#include "SVGElement.h"

//ShapeList (using linked list to contain SVGElements)
/*
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
*/
//SVGContainer

