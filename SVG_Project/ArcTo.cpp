#include "ArcTo.h"
#include <algorithm> 
#include <cmath>

using namespace Gdiplus;

constexpr double M_PI = 3.14159265358979323846;
// Helper: radians
static inline double deg2rad(double deg) {
    return deg * M_PI / 180.0;
}

// Constructor
Arcto::Arcto(float rx_, float ry_, float xAxisRotation_,
    int largeArcFlag_, int sweepFlag_,
    float x, float y)
    : rx(rx_), ry(ry_), xAxisRotation(xAxisRotation_),
    largeArcFlag(largeArcFlag_), sweepFlag(sweepFlag_),
    destination(x, y) {
}

// Core draw
void Arcto::draw(Graphics* graphics, GraphicsPath* path) {
    PointF last;
    path->GetLastPoint(&last);

    // If radii are zero → straight line
    if (rx == 0 || ry == 0) {
        path->AddLine(last, destination);
        return;
    }

    // ---- SVG Arc implementation ----
    // Reference: SVG 1.1 spec (implementation notes)
    // https://www.w3.org/TR/SVG/implnote.html#ArcImplementationNotes

    double x1 = last.X;
    double y1 = last.Y;
    double x2 = destination.X;
    double y2 = destination.Y;
    double phi = deg2rad(xAxisRotation);
    double cosPhi = cos(phi);
    double sinPhi = sin(phi);

    // Step 1: Compute (x1', y1')
    double dx = (x1 - x2) / 2.0;
    double dy = (y1 - y2) / 2.0;
    double x1p = cosPhi * dx + sinPhi * dy;
    double y1p = -sinPhi * dx + cosPhi * dy;

    // Correct radii
    double rx_sq = rx * rx;
    double ry_sq = ry * ry;
    double x1p_sq = x1p * x1p;
    double y1p_sq = y1p * y1p;

    double lam = x1p_sq / rx_sq + y1p_sq / ry_sq;
    if (lam > 1) {
        double s = sqrt(lam);
        rx *= s;
        ry *= s;
        rx_sq = rx * rx;
        ry_sq = ry * ry;
    }

    // Step 2: Compute (cx', cy')
    double sign = (largeArcFlag != sweepFlag) ? 1.0 : -1.0;
    double num = rx_sq * ry_sq - rx_sq * y1p_sq - ry_sq * x1p_sq;
    double den = rx_sq * y1p_sq + ry_sq * x1p_sq;
    double coef = sign * sqrt((std::max)(0.0, num / den));
    double cxp = coef * (rx * y1p) / ry;
    double cyp = coef * -(ry * x1p) / rx;

    // Step 3: Transform back to get (cx, cy)
    double cx = cosPhi * cxp - sinPhi * cyp + (x1 + x2) / 2.0;
    double cy = sinPhi * cxp + cosPhi * cyp + (y1 + y2) / 2.0;

    // Step 4: Compute angles
    auto vecAngle = [](double ux, double uy, double vx, double vy) {
        double dot = ux * vx + uy * vy;
        double len = sqrt((ux * ux + uy * uy) * (vx * vx + vy * vy));
        double ang = acos((std::max)(-1.0, (std::min)(1.0, dot / len)));
        if (ux * vy - uy * vx < 0) ang = -ang;
        return ang;
        };

    double ux = (x1p - cxp) / rx;
    double uy = (y1p - cyp) / ry;
    double vx = (-x1p - cxp) / rx;
    double vy = (-y1p - cyp) / ry;

    double theta1 = vecAngle(1, 0, ux, uy);
    double deltaTheta = vecAngle(ux, uy, vx, vy);

    if (!sweepFlag && deltaTheta > 0) deltaTheta -= 2 * M_PI;
    else if (sweepFlag && deltaTheta < 0) deltaTheta += 2 * M_PI;

    // Step 5: Approximate arc with cubic Beziers
    int segs = static_cast<int>(ceil(fabs(deltaTheta / (M_PI / 2.0))));
    double delta = deltaTheta / segs;
    double t = theta1;

    for (int i = 0; i < segs; i++) {
        double t1 = t;
        double t2 = t + delta;

        double cosT1 = cos(t1), sinT1 = sin(t1);
        double cosT2 = cos(t2), sinT2 = sin(t2);

        PointF p1(
            static_cast<REAL>(cx + rx * cosPhi * cosT1 - ry * sinPhi * sinT1),
            static_cast<REAL>(cy + rx * sinPhi * cosT1 + ry * cosPhi * sinT1)
        );
        PointF p4(
            static_cast<REAL>(cx + rx * cosPhi * cosT2 - ry * sinPhi * sinT2),
            static_cast<REAL>(cy + rx * sinPhi * cosT2 + ry * cosPhi * sinT2)
        );

        // Approximate control points
        double alpha = sin(delta) * (sqrt(4 + 3 * pow(tan(delta / 2), 2)) - 1) / 3.0;
        PointF p2(
            static_cast<REAL>(p1.X + alpha * (-rx * cosPhi * sinT1 - ry * sinPhi * cosT1)),
            static_cast<REAL>(p1.Y + alpha * (-rx * sinPhi * sinT1 + ry * cosPhi * cosT1))
        );
        PointF p3(
            static_cast<REAL>(p4.X + alpha * (rx * cosPhi * sinT2 + ry * sinPhi * cosT2)),
            static_cast<REAL>(p4.Y + alpha * (rx * sinPhi * sinT2 - ry * cosPhi * cosT2))
        );

        path->AddBezier(p1, p2, p3, p4);

        t += delta;
    }
}
