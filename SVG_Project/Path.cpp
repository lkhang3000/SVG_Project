#include "Path.h"

// Helper to check if a string is a command letter
bool path::isCommand(const string& token) {
    return token.size() == 1 && isalpha(token[0]);
}

// Robust tokenizer for SVG path data
vector<string> path::tokenizeSVGPath(const string& d) {
    vector<string> tokens;
    string current;
    size_t i = 0;
    while (i < d.size()) {
        char c = d[i];
        if (isalpha(c)) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
            tokens.push_back(std::string(1, c));
            i++;
        }
        else if (c == '-' || c == '+') {
            // Sign can be part of a number, but if not at start, finish current token
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
            current += c;
            i++;
        }
        else if (isdigit(c) || c == '.') {
            current += c;
            i++;
        }
        else if (c == ',' || isspace(c)) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
            i++;
        }
        else {
            // Unhandled character, skip
            i++;
        }
    }
    if (!current.empty()) tokens.push_back(current);
    return tokens;
}

path::path() : SVGElement() {}

path::~path() {
    for (auto cmd : commands) delete cmd;
}

void path::setValue(tinyxml2::XMLElement* element) {
    float currentX = 0, currentY = 0;
    float startX = 0, startY = 0;

    this->SVGElement::setValue(element);
    const char* dAttr = element->Attribute("d");
    if (!dAttr) return;

    std::string d(dAttr);
    auto tokens = tokenizeSVGPath(d);

    size_t i = 0;
    while (i < tokens.size()) {
        std::string cmd = tokens[i++];

        if (cmd == "M" || cmd == "m") {
            float x = std::stof(tokens[i++]);
            float y = std::stof(tokens[i++]);
            if (cmd == "m") { x += currentX; y += currentY; }
            commands.push_back(new MoveTo(x, y));
            currentX = startX = x;
            currentY = startY = y;

            // --- Handle implicit lineto after moveto ---
            while (i + 1 < tokens.size() && !isCommand(tokens[i])) {
                float x1 = std::stof(tokens[i++]);
                float y1 = std::stof(tokens[i++]);
                if (cmd == "m") { x1 += currentX; y1 += currentY; }
                commands.push_back(new Lineto(x1, y1));
                currentX = x1;
                currentY = y1;
            }
        }
        else if (cmd == "L" || cmd == "l") {
            while (i + 1 < tokens.size() && !isCommand(tokens[i])) {
                float x = std::stof(tokens[i++]);
                float y = std::stof(tokens[i++]);
                if (cmd == "l") { x += currentX; y += currentY; }
                commands.push_back(new Lineto(x, y));
                currentX = x;
                currentY = y;
            }
        }

        else if (cmd == "H" || cmd == "h") {
            while (i < tokens.size() && !isCommand(tokens[i])) {
                float x = std::stof(tokens[i++]);
                if (cmd == "h") x += currentX;
                commands.push_back(new Lineto(x, currentY));
                currentX = x;
            }
        }

        else if (cmd == "V" || cmd == "v") {
            while (i < tokens.size() && !isCommand(tokens[i])) {
                float y = std::stof(tokens[i++]);
                if (cmd == "v") y += currentY;
                commands.push_back(new Lineto(currentX, y));
                currentY = y;
            }
        }

        else if (cmd == "C" || cmd == "c") {
            while (i + 5 < tokens.size() && !isCommand(tokens[i])) {
                float x1 = std::stof(tokens[i++]);
                float y1 = std::stof(tokens[i++]);
                float x2 = std::stof(tokens[i++]);
                float y2 = std::stof(tokens[i++]);
                float x = std::stof(tokens[i++]);
                float y = std::stof(tokens[i++]);
                if (cmd == "c") {
                    x1 += currentX; y1 += currentY;
                    x2 += currentX; y2 += currentY;
                    x += currentX; y += currentY;
                }
                commands.push_back(new CurveTo(x1, y1, x2, y2, x, y));
                currentX = x;
                currentY = y;
            }
        }

        else if (cmd == "A" || cmd == "a") {
            while (i + 6 < tokens.size() && !isCommand(tokens[i])) {
            float rx = std::stof(tokens[i++]);
            float ry = std::stof(tokens[i++]);
            float xAxisRotation = std::stof(tokens[i++]);
            int largeArcFlag = std::stoi(tokens[i++]);
            int sweepFlag = std::stoi(tokens[i++]);
            float x = std::stof(tokens[i++]);
            float y = std::stof(tokens[i++]);
            if (cmd == "a") { x += currentX; y += currentY; }
            commands.push_back(new Arcto(rx, ry, xAxisRotation, largeArcFlag, sweepFlag, x, y));
            currentX = x;
            currentY = y;
            }
        }
                
        else if (cmd == "Z" || cmd == "z") {
            commands.push_back(new ClosePath());
            currentX = startX;
            currentY = startY;
        }
    }
}

void path::draw(HDC hdc, gradientDatabase& database) {
    Graphics g(hdc);
    this->draw(&g, database);
}

void path::draw(Graphics* g, gradientDatabase& database) {
    GraphicsState state = g->Save();
    this->handleTransform(g);
    GraphicsPath path;
    // Draw the path commands
    for (auto cmd : commands) {
        cmd->draw(g, &path);
    }
    // Fill
    Brush* brush = NULL;
    if (this->fillID != "") brush = database.getBrush(this->fillID);
    else if (fill.GetA() != 0) brush = new SolidBrush(Color(this->fillOpacity * 255, this->fill.GetR(), this->fill.GetG(), this->fill.GetB()));
    g->FillPath(brush, &path);
    delete brush;
    // Stroke
    if (this->stroke.GetA() > 0 && this->strokeWidth > 0) {
        Pen pen(Color(static_cast<BYTE>(this->strokeOpacity * 255), this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB()));
        pen.SetWidth(this->strokeWidth);
        g->DrawPath(&pen, &path);
    }

    g->Restore(state);
}
