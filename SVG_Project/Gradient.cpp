#include "Gradient.h"


//gradientDatabase
void gradientDatabase::extractData(tinyxml2::XMLElement* element) {
	for (tinyxml2::XMLElement* child = element->FirstChildElement(); child != NULL; child = child->NextSiblingElement()) {
        gradient* newGradient = NULL;
		if (string(child->Name()) == "linearGradient") newGradient = new linearGradient;
        else newGradient = new radialGradient;
        newGradient->setValue(child);
        this->content.push_back(newGradient);
	}
}
Brush* gradientDatabase::getBrush(string id) {
    for (int i = 0; i < this->content.size(); i++) {
        if (this->content[i]->getID() == id) return this->content[i]->getBrush();
    }
    return NULL;
}
gradientDatabase::~gradientDatabase() {
    for (int i = 0; i < this->content.size(); i++) delete this->content[i];
    this->content.resize(0);
}
