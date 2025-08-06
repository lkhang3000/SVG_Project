#include "Path.h"

path::path() : SVGElement() {}

path::~path() {
    for (auto cmd : commands) delete cmd;
}

void path::setValue(tinyxml2::XMLElement* element) {
    this->SVGElement::setValue(element);
    const char* dAttr = element->Attribute("d");
    if (!dAttr) return;

    string d(dAttr);
    vector<string> tokens;
    string current;
    for (char c : d) {
        if (std::isalpha(c)) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
            tokens.push_back(string(1, c)); // push command
        }
        else if (std::isdigit(c) || c == '.' || c == '-' || c == '+') { // handle numbers
            current += c;
        }
        else if (c == ',' || std::isspace(c)) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
        }
    }
    if (!current.empty()) tokens.push_back(current);

    // Parser
    size_t i = 0;
    while (i < tokens.size()) {
        string cmd = tokens[i++];
        if (cmd == "M") {
            float x = std::stof(tokens[i++]);
            float y = std::stof(tokens[i++]);
            commands.push_back(new MoveTo(x, y));
        }
        else if (cmd == "L") {
            while (i + 1 < tokens.size() && !std::isalpha(tokens[i][0])) {
                float x = std::stof(tokens[i++]);
                float y = std::stof(tokens[i++]);
                commands.push_back(new Lineto(x, y));
            }
        }
        else if (cmd == "H") {
            while (i < tokens.size() && !std::isalpha(tokens[i][0])) {
                float x = std::stof(tokens[i++]);
                commands.push_back(new HLineTo(x));
            }
        }
        else if (cmd == "V") {
            while (i < tokens.size() && !std::isalpha(tokens[i][0])) {
                float y = std::stof(tokens[i++]);
                commands.push_back(new VLineTo(y));
            }
        }
        else if (cmd == "C") {
            while (i + 5 < tokens.size() && !std::isalpha(tokens[i][0])) {
                float x1 = stof(tokens[i++]);
                float y1 = stof(tokens[i++]);
                float x2 = stof(tokens[i++]);
                float y2 = stof(tokens[i++]);
                float x = stof(tokens[i++]);
                float y = stof(tokens[i++]);
                commands.push_back(new CurveTo(x1, y1, x2, y2, x, y));
            }
        }
        else if (cmd == "Z" || cmd == "z") {
            commands.push_back(new ClosePath());
        }
    }
}

void path::draw(HDC hdc) {
    Graphics g(hdc);
    this->draw(&g);
}

void path::draw(Graphics* g) {
    this->handleTransform(g);
    GraphicsPath path;
    // Draw the path commands
    for (auto cmd : commands) {
        cmd->draw(g, &path);
    }
    // Fill
    SolidBrush brush(Color(static_cast<BYTE>(fillOpacity * 255), fill.GetR(), fill.GetG(), fill.GetB()));
    g->FillPath(&brush, &path);
    // Stroke
    Pen pen(Color(static_cast<BYTE>(this->strokeOpacity * 255), this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB()));
    pen.SetWidth(this->strokeWidth);
    g->DrawPath(&pen, &path);
}

