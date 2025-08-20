#include "Path.h"

// Helper to check if a string is a command letter
bool path::isCommand(const string& token) {
    return token.size() == 1 && isalpha(token[0]);
}

// Robust tokenizer for SVG path data
vector<string> path::tokenizeSVGPath(const string& d) {
    vector<string> tokens;
    string current;
    bool inNumber = false;
    size_t i = 0;
    while (i < d.size()) {
        char c = d[i];
        if (isalpha(c)) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
            tokens.push_back(string(1, c));
            i++;
            inNumber = false;
        }
        else if (c == '-' || c == '+') {
            // If in the middle of a number and next is e/E, it's part of scientific notation
            if (!current.empty() && (d[i - 1] == 'e' || d[i - 1] == 'E')) {
                current += c;
                i++;
            }
            else {
                if (!current.empty()) tokens.push_back(current);
                current = c;
                i++;
                inNumber = true;
            }
        }
        else if (isdigit(c) || c == '.') {
            current += c;
            i++;
            inNumber = true;
        }
        else if (c == 'e' || c == 'E') {
            current += c;
            i++;
            inNumber = true;
        }
        else if (c == ',' || isspace(c)) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
            i++;
            inNumber = false;
        }
        else {
            i++; // skip unknown chars
        }
    }
    if (!current.empty()) tokens.push_back(current);
    return tokens;
}
float path::getNextFloat(const vector<string>& tokens, size_t& i) {
    if (i < tokens.size() && !isCommand(tokens[i])) {
        float value = stof(tokens[i]);
        i++;
        return value;
    }
    return 0.0f; // Default value if no more tokens or next is a command
}

int path::getNextInt(const vector<string>& tokens, size_t& i) {
    if (i < tokens.size() && !isCommand(tokens[i])) {
        int value = stoi(tokens[i]);
        i++;
        return value;
    }
    return 0; // Default value if no more tokens or next is a command
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
            if (i + 1 < tokens.size() && !isCommand(tokens[i]) && !isCommand(tokens[i + 1])) {
                float x = getNextFloat(tokens, i);
                float y = getNextFloat(tokens, i);
                if (cmd == "m") { x += currentX; y += currentY; }
                commands.push_back(new MoveTo(x, y));
                currentX = startX = x;
                currentY = startY = y;
            }

            // --- Handle implicit lineto after moveto ---
            while (i + 1 < tokens.size() && !isCommand(tokens[i]) && !isCommand(tokens[i + 1])) {
                float x1 = getNextFloat(tokens, i);
                float y1 = getNextFloat(tokens, i);
                if (cmd == "m") { x1 += currentX; y1 += currentY; }
                commands.push_back(new Lineto(x1, y1));
                currentX = x1;
                currentY = y1;
            }
        }
        else if (cmd == "L" || cmd == "l") {
            while (i + 1 < tokens.size() && !isCommand(tokens[i]) && !isCommand(tokens[i + 1])) {
                float x = getNextFloat(tokens, i);
                float y = getNextFloat(tokens, i);
                if (cmd == "l") { x += currentX; y += currentY; }
                commands.push_back(new Lineto(x, y));
                currentX = x;
                currentY = y;
            }
        }
        else if (cmd == "H" || cmd == "h") {
            while (i < tokens.size() && !isCommand(tokens[i])) {
                float x = getNextFloat(tokens, i);
                if (cmd == "h") x += currentX;
                commands.push_back(new Lineto(x, currentY));
                currentX = x;
            }
        }
        else if (cmd == "V" || cmd == "v") {
            while (i < tokens.size() && !isCommand(tokens[i])) {
                float y = getNextFloat(tokens, i);
                if (cmd == "v") y += currentY;
                commands.push_back(new Lineto(currentX, y));
                currentY = y;
            }
        }
        else if (cmd == "C" || cmd == "c") {
            while (i + 5 < tokens.size() &&
                !isCommand(tokens[i]) && !isCommand(tokens[i + 1]) && !isCommand(tokens[i + 2]) &&
                !isCommand(tokens[i + 3]) && !isCommand(tokens[i + 4]) && !isCommand(tokens[i + 5])) {
                float x1 = getNextFloat(tokens, i);
                float y1 = getNextFloat(tokens, i);
                float x2 = getNextFloat(tokens, i);
                float y2 = getNextFloat(tokens, i);
                float x = getNextFloat(tokens, i);
                float y = getNextFloat(tokens, i);
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
            while (i + 6 < tokens.size() &&
                !isCommand(tokens[i]) && !isCommand(tokens[i + 1]) && !isCommand(tokens[i + 2]) &&
                !isCommand(tokens[i + 3]) && !isCommand(tokens[i + 4]) && !isCommand(tokens[i + 5]) && !isCommand(tokens[i + 6])) {
                float rx = getNextFloat(tokens, i);
                float ry = getNextFloat(tokens, i);
                float xAxisRotation = getNextFloat(tokens, i);

                // Only accept "0" or "1" for arc flags
                int largeArcFlag = (tokens[i] == "0" || tokens[i] == "1") ? std::stoi(tokens[i++]) : 0;
                int sweepFlag = (tokens[i] == "0" || tokens[i] == "1") ? std::stoi(tokens[i++]) : 0;

                float x = getNextFloat(tokens, i);
                float y = getNextFloat(tokens, i);
                if (cmd == "a") { x += currentX; y += currentY; }
                commands.push_back(new Arcto(rx, ry, xAxisRotation, largeArcFlag, sweepFlag, x, y));
                currentX = x;
                currentY = y;
            }
        }
        else if (cmd == "S" || cmd == "s") {
            float prevC2X = currentX, prevC2Y = currentY;
            // Find previous CurveTo or SmoothCubicBezierTo for reflection
            if (!commands.empty()) {
                auto* prev = commands.back();
                if (auto* c = dynamic_cast<CurveTo*>(prev)) {
                    prevC2X = 2 * currentX - c->x2();
                    prevC2Y = 2 * currentY - c->y2();
                }
                else if (auto* s = dynamic_cast<SmoothCubicBezierTo*>(prev)) {
                    prevC2X = 2 * currentX - s->x2;
                    prevC2Y = 2 * currentY - s->y2;
                }
            }
            while (i + 3 < tokens.size() &&
                !isCommand(tokens[i]) && !isCommand(tokens[i + 1]) &&
                !isCommand(tokens[i + 2]) && !isCommand(tokens[i + 3])) {
                float x2 = getNextFloat(tokens, i);
                float y2 = getNextFloat(tokens, i);
                float x = getNextFloat(tokens, i);
                float y = getNextFloat(tokens, i);
                float ctrl2X = (cmd == "s") ? (currentX + x2) : x2;
                float ctrl2Y = (cmd == "s") ? (currentY + y2) : y2;
                float endX = (cmd == "s") ? (currentX + x) : x;
                float endY = (cmd == "s") ? (currentY + y) : y;
                commands.push_back(new SmoothCubicBezierTo(prevC2X, prevC2Y, ctrl2X, ctrl2Y, endX, endY));
                prevC2X = ctrl2X; prevC2Y = ctrl2Y;
                currentX = endX; currentY = endY;
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
    if (brush) {
        g->FillPath(brush, &path);
    }
    delete brush;
    // Stroke
    if (this->stroke.GetA() > 0 && this->strokeWidth > 0) {
        Pen pen(Color(static_cast<BYTE>(this->strokeOpacity * 255), this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB()));
        pen.SetWidth(this->strokeWidth);
        g->DrawPath(&pen, &path);
    }

    g->Restore(state);
}
