#pragma once
#ifndef ReadSVG_h
#define ReadSVG_h
#include <iostream>
#include <sstream>
#include <vector>
#include "tinyxml2.h"
#include "stdafx.h"
using namespace tinyxml2;
using namespace std;

void PrintPoints(const char* points);
void PrintRect(XMLElement* element);
void PrintCircle(XMLElement* element);
void PrintEllipse(XMLElement* element);
void PrintLine(XMLElement* element);
void PrintPolyline(XMLElement* element);
void PrintPolygon(XMLElement* element);
void PrintText(XMLElement* element);
void PrintAllElements(XMLElement* element);


#endif