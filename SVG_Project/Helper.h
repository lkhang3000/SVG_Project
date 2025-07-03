#pragma once
#ifndef Helper_h
#define Helper_h
#include "stdafx.h"
#include <iostream>
using namespace std;
struct SVGColor {
	int r, g, b, a;
	SVGColor() {
		r = 0;
		g = 0;
		b = 0;
		a = 255;
	}
};

struct SVGPoint {
	double x, y;
	SVGPoint() {
		x = 0.0;
		y = 0.0;
	}
};

#endif // !1
