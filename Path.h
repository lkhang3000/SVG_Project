#pragma once
#include "Shape.h"
#include "PathCommand.h"
#include <vector>
#include <string>

class path : public shape {
    std::vector<PathCommand*> commands;
public:
    ~path();
    void setValue(tinyxml2::XMLElement* element) override;
    void draw(HDC hdc) override;
};
