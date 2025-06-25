#include <iostream>
#include <sstream>
#include <vector>
#include "tinyxml2.h"
using namespace tinyxml2;
using namespace std;

void PrintPoints(const char* points) {
    if (!points) return;
    cout << "points:" << endl;
    stringstream ss(points);
    string point;
    int idx = 1;
    while (getline(ss, point, ' ')) {
        size_t comma = point.find(',');
        if (comma != string::npos) {
            string x = point.substr(0, comma);
            string y = point.substr(comma + 1);
            cout << "  Point" << idx++ << ": (" << x << "," << y << ")" << endl;
        }
    }
}

void PrintRect(XMLElement* element) {
    cout << "Rect" << endl;
    const char* attrs[] = { "x", "y", "width", "height", "fill", "fill-opacity", "stroke", "stroke-width", "stroke-opacity" };
    for (auto attr : attrs) {
        const char* val = element->Attribute(attr);
        if (val) cout << attr << ": " << val << endl;
    }
    cout << endl;
}

void PrintCircle(XMLElement* element) {
    cout << "Circle" << endl;
    const char* attrs[] = { "cx", "cy", "r", "fill", "fill-opacity", "stroke", "stroke-width", "stroke-opacity" };
    for (auto attr : attrs) {
        const char* val = element->Attribute(attr);
        if (val) cout << attr << ": " << val << endl;
    }
    cout << endl;
}

void PrintEllipse(XMLElement* element) {
    cout << "Ellipse" << endl;
    const char* attrs[] = { "cx", "cy", "rx", "ry", "fill", "fill-opacity", "stroke", "stroke-width", "stroke-opacity" };
    for (auto attr : attrs) {
        const char* val = element->Attribute(attr);
        if (val) cout << attr << ": " << val << endl;
    }
    cout << endl;
}

void PrintLine(XMLElement* element) {
    cout << "Line" << endl;
    const char* attrs[] = { "x1", "y1", "x2", "y2", "stroke", "stroke-width", "stroke-opacity" };
    for (auto attr : attrs) {
        const char* val = element->Attribute(attr);
        if (val) cout << attr << ": " << val << endl;
    }
    cout << endl;
}

void PrintPolyline(XMLElement* element) {
    cout << "Polyline" << endl;
    const char* attrs[] = { "stroke", "stroke-width", "stroke-opacity", "fill", "fill-opacity" };
    for (auto attr : attrs) {
        const char* val = element->Attribute(attr);
        if (val) cout << attr << ": " << val << endl;
    }
    PrintPoints(element->Attribute("points"));
    cout << endl;
}

void PrintPolygon(XMLElement* element) {
    cout << "Polygon" << endl;
    const char* attrs[] = { "fill", "fill-opacity", "stroke", "stroke-width", "stroke-opacity" };
    for (auto attr : attrs) {
        const char* val = element->Attribute(attr);
        if (val) cout << attr << ": " << val << endl;
    }
    PrintPoints(element->Attribute("points"));
    cout << endl;
}

void PrintText(XMLElement* element) {
    cout << "Text" << endl;
    const char* attrs[] = { "x", "y", "fill", "font-size" };
    for (auto attr : attrs) {
        const char* val = element->Attribute(attr);
        if (val) cout << attr << ": " << val << endl;
    }
    if (element->GetText()) {
        cout << "content: " << element->GetText() << endl;
    }
    cout << endl;
}

void PrintAllElements(XMLElement* element) {
    if (!element) return;
    string name = element->Name();
    if (name == "rect") PrintRect(element);
    else if (name == "circle") PrintCircle(element);
    else if (name == "ellipse") PrintEllipse(element);
    else if (name == "line") PrintLine(element);
    else if (name == "polyline") PrintPolyline(element);
    else if (name == "polygon") PrintPolygon(element);
    else if (name == "text") PrintText(element);


    for (XMLElement* child = element->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
        PrintAllElements(child);
    }
}

int main() {
    XMLDocument doc;
    doc.LoadFile("demo.svg");
    if (doc.ErrorID() != 0) {
        cout << "Error loading SVG file: " << doc.ErrorID() << endl;
        return 1;
    }
    XMLElement* root = doc.RootElement();
    if (root == nullptr) {
        cout << "No root element found." << endl;
        return 1;
    }
    PrintAllElements(root);
    return 0;
}
