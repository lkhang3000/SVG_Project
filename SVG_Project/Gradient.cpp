#include "Gradient.h"
//Gradient
void gradient::setValue(tinyxml2::XMLElement* element) {
    const char* attrs[] = { "id" };
    for (auto attr : attrs) {
        const char* val = element->Attribute(attr);
        if (val) {
            if (attr == "id") {
                this->id = string(val);
            }
        }
    }
}

string gradient::getID() {
    return id;
}

//linearGradient
void linearGradient::setValue(tinyxml2::XMLElement* element) {
    gradient::setValue(element);

    const char* attrs[] = { "x1", "y1", "x2", "y2" };
    for (auto attr : attrs) {
        const char* val = element->Attribute(attr);
        if (val) {
            if (attr == "x1") this->x1 = atof(val);
            else if (attr == "y1") this->y1 = atof(val);
            else if (attr == "x2") this->x2 = atof(val);
            else if (attr == "y2") this->y2 = atof(val);
        }
        else {
            if (attr == "x1") this->x1 = 0.0;
            else if (attr == "y1") this->y1 = 0.0;
            else if (attr == "x2") this->x2 = 0.0;
            else if (attr == "y2") this->y2 = 0.0;
        }
    }

    count = 0;
    for (tinyxml2::XMLElement* child = element->FirstChildElement("stop"); child != nullptr && count < 10; child = child->NextSiblingElement("stop")) {
        const char* Attrs[] = { "offset", "stop-color" };
        for (auto attr : Attrs) {
            const char* val = child->Attribute(attr);
            if (val) {
                if (attr == "offset") {
                    string s(val);
                    if (s.back() == '%') {
                        s.pop_back();
                        this->offset[count] = atof(s.c_str()) / 100.0;

                    }
                    else {
                        this->offset[count] = atof(val);
                    }
                }
                else if (attr == "stop-color") {
                    string s(val);
                    if (s == "black") {
                        this->colors[count] = Color(0, 0, 0);

                    }
                    else if (s == "white") {
                        this->colors[count] = Color(255, 255, 255);
                    }
                    else if (s == "red") {
						this->colors[count] = Color(255, 0, 0);
                    }
                    else if (s == "green") {
						this->colors[count] = Color(0, 255, 0);
                    }
                    else if (s == "yellow") {
                        this->colors[count] = Color(255, 255, 0);
                    }
                    else if (s[0] == '#') {
						string hex = s.substr(1);
                        if(hex.length() == 3) {
                            hex = string(2, hex[0]) + string(2, hex[1]) + string(2, hex[2]);
						}
                        if(hex.length() == 6) {
                            BYTE r = stoi(hex.substr(0, 2), nullptr, 16);
                            BYTE g = stoi(hex.substr(2, 2), nullptr, 16);
                            BYTE b = stoi(hex.substr(4, 2), nullptr, 16);
                            this->colors[count] = Color(r, g, b);
                        }
                        else {
                            this->colors[count] = Color(0, 0, 0);
						}
                    }
                }
            }
        }
    }
}

Brush* linearGradient::getBrush() {
	PointF start((float)(x1 * 100), (float)(y1 * 100));
    PointF end((float)(x2 * 100), (float)(y2 * 100));

	LinearGradientBrush* brush = new LinearGradientBrush(start, end, colors[0], colors[count - 1]);
    if (count > 2) {
		Color* colorArray = new Color[count];
		REAL* offsetArray = new REAL[count];

        for (int i = 0; i < count; i++) {
            colorArray[i] = colors[i];
            offsetArray[i] = (REAL)offset[i];
        }

		brush->SetInterpolationColors(colorArray, offsetArray, count);

		delete[] colorArray;
		delete[] offsetArray;
    }
}

//radialGradient
Brush* radialGradient::getBrush() {
    GraphicsPath path;
    path.AddEllipse(Rect(0, 0, 500, 500));
    PathGradientBrush* pgb = new PathGradientBrush(&path);
    if (!stops.empty()) {
        pgb->SetCenterColor(stops.front().color);
        int count = 1;
        Color surround[1] = { stops.back().color };
        pgb->SetSurroundColors(surround, &count);
    }
    else {
        pgb->SetCenterColor(Color(255, 255, 255, 0));
        int count = 1;
        Color surround[1] = { Color(255, 0, 0, 255) };
        pgb->SetSurroundColors(surround, &count);
    }
    return pgb;
}
void radialGradient::setValue(tinyxml2::XMLElement* element) {
    gradient::setValue(element);
    if (const char* val = element->Attribute("cx")) cx = atof(val) / 100.0f;
    if (const char* val = element->Attribute("cy")) cy = atof(val) / 100.0f;
    if (const char* val = element->Attribute("r"))  r = atof(val) / 100.0f;
	
    for (tinyxml2::XMLElement* stopEl = element->FirstChildElement("stop");
        stopEl;
        stopEl = stopEl->NextSiblingElement("stop"))
    {
        GradientStop stop{};
        if (const char* off = stopEl->Attribute("offset")) {
            stop.offset = static_cast<float>(atof(off));
        }
        if (const char* colorStr = stopEl->Attribute("stop-color")) {
            int r = 0, g = 0, b = 0;
            if (colorStr[0] == '#') {
                sscanf_s(colorStr + 1, "%02x%02x%02x", &r, &g, &b);
            }
            stop.color = Color(255, r, g, b);
        }
        stops.push_back(stop);
    }
}


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
