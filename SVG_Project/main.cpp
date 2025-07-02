#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "tinyxml2.h"

using namespace Gdiplus;
using namespace std;
using namespace tinyxml2;

// --------- MyColor ---------
class MyColor {
private:
    int r, g, b;
public:
    MyColor(int r = 0, int g = 0, int b = 0) : r(r), g(g), b(b) {}
    Gdiplus::Color toGdiplusColor(double alpha = 1.0) const {
        return Gdiplus::Color(static_cast<BYTE>(alpha * 255), r, g, b);
    }
};

// --------- Point2D ---------
class Point2D {
private:
    double x, y;
public:
    Point2D(double x = 0, double y = 0) : x(x), y(y) {}
    double getX() const { return x; }
    double getY() const { return y; }
};

// --------- Abstract Shape ---------
class Shape {
protected:
    MyColor stroke;
    double strokeWidth;
    double strokeOpacity;
    MyColor fill;
    double fillOpacity;

public:
    virtual void draw(Gdiplus::Graphics& g) = 0;

    MyColor getStroke() const { return stroke; }
    double getStrokeWidth() const { return strokeWidth; }
    double getStrokeOpacity() const { return strokeOpacity; }
    MyColor getFill() const { return fill; }
    double getFillOpacity() const { return fillOpacity; }

    void setStroke(MyColor c) { stroke = c; }
    void setStrokeWidth(double w) { strokeWidth = w; }
    void setStrokeOpacity(double o) { strokeOpacity = o; }
    void setFill(MyColor c) { fill = c; }
    void setFillOpacity(double o) { fillOpacity = o; }
};

// --------- Line ---------
class Line : public Shape {
private:
    std::vector<Point2D> points;
public:
    Line(const std::vector<Point2D>& pts, MyColor stroke, double width, double opacity) {
        points = pts;
        this->stroke = stroke;
        this->strokeWidth = width;
        this->strokeOpacity = opacity;
    }

    void draw(Graphics& g) override {
        if (points.size() < 2) return;
        Pen pen(stroke.toGdiplusColor(strokeOpacity), (REAL)strokeWidth);
        g.DrawLine(&pen,
            (int)points[0].getX(), (int)points[0].getY(),
            (int)points[1].getX(), (int)points[1].getY());
    }
};

// --------- Polylines ---------
class Polylines : public Shape {
private:
    std::vector<Point2D> points;
public:
    Polylines(const std::vector<Point2D>& pts, MyColor stroke, double width, double opacity,
        MyColor fill = MyColor(0, 0, 0), double fillOpacity = 0.0) {
        this->points = pts;
        this->stroke = stroke;
        this->strokeWidth = width;
        this->strokeOpacity = opacity;
        this->fill = fill;
        this->fillOpacity = fillOpacity;
    }

    void draw(Graphics& g) override {
        if (points.size() < 2) return;

        vector<Point> gdiPoints;
        for (auto& p : points)
            gdiPoints.push_back(Point((int)p.getX(), (int)p.getY()));

        if (fillOpacity > 0.0 &&
            points.front().getX() == points.back().getX() &&
            points.front().getY() == points.back().getY()) {
            SolidBrush brush(fill.toGdiplusColor(fillOpacity));
            g.FillPolygon(&brush, gdiPoints.data(), (INT)gdiPoints.size());
        }

        Pen pen(stroke.toGdiplusColor(strokeOpacity), (REAL)strokeWidth);
        g.DrawLines(&pen, gdiPoints.data(), (INT)gdiPoints.size());
    }
};

// --------- Text ---------
class Text {
public:
    Point2D point;
    std::string content;
    double font_size;
    MyColor fill;
    double opacity;

    Text(Point2D p, const std::string& c, double fs, MyColor f, double o = 1.0)
        : point(p), content(c), font_size(fs), fill(f), opacity(o) {}
};

// --------- SVG Text Parser ---------
MyColor ParseColor(const char* fill) {
    int r = 0, g = 0, b = 0;
    if (fill && strncmp(fill, "rgb", 3) == 0) {
        sscanf_s(fill, "rgb(%d,%d,%d)", &r, &g, &b);
    }
    return MyColor(r, g, b);
}

vector<Text> ParseSvgText(const char* filename) {
    vector<Text> texts;
    XMLDocument doc;
    if (doc.LoadFile(filename) != XML_SUCCESS) {
        MessageBoxA(NULL, "Failed to load sample.svg", "SVG Error", MB_ICONERROR);
        return texts;
    }
    XMLElement* root = doc.RootElement();
    for (XMLElement* el = root->FirstChildElement("text"); el; el = el->NextSiblingElement("text")) {
        double x = el->DoubleAttribute("x", 0);
        double y = el->DoubleAttribute("y", 0);
        double font_size = el->DoubleAttribute("font-size", 16);
        const char* fill = el->Attribute("fill");
        MyColor color = ParseColor(fill);
        double opacity = el->DoubleAttribute("opacity", 1.0);
        const char* txt = el->GetText();
        std::string content = txt ? txt : "";
        texts.emplace_back(Point2D(x, y), content, font_size, color, opacity);
    }
    return texts;
}

// --------- Data Parser for Line and Polyline ---------
void ParseLinesAndPolylinesFromFile(const string& filename,
    vector<Line>& lines, vector<Polylines>& polylines) {

    ifstream fin(filename);
    string line;

    while (getline(fin, line)) {
        stringstream ss(line);
        string type;
        ss >> type;

        vector<Point2D> pts;
        int r, g, b;
        double width, strokeOpacity, fillOpacity = 0;
        int fr = 0, fg = 0, fb = 0;

        int x, y;
        while (ss >> x >> y) {
            if (ss.peek() == ',') ss.ignore();
            pts.emplace_back(x, y);
        }

        fin >> r >> g >> b >> width >> strokeOpacity;
        if (type == "polyline") {
            fin >> fr >> fg >> fb >> fillOpacity;
            polylines.emplace_back(pts, MyColor(r, g, b), width, strokeOpacity,
                                   MyColor(fr, fg, fb), fillOpacity);
        } else {
            lines.emplace_back(pts, MyColor(r, g, b), width, strokeOpacity);
        }
        getline(fin, line); // flush
    }
}

std::wstring Utf8ToWstring(const std::string& str) {
    if (str.empty()) return std::wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

#pragma comment (lib,"Gdiplus.lib")

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static vector<Text> texts;
    switch (msg) {
    case WM_CREATE:
        texts = ParseSvgText("sample.svg");
        break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        Graphics graphics(hdc);

        RECT rc;
        GetClientRect(hwnd, &rc);
        SolidBrush bgBrush(Color(255, 255, 255, 255));
        graphics.FillRectangle(&bgBrush, static_cast<REAL>(rc.left), static_cast<REAL>(rc.top), static_cast<REAL>(rc.right - rc.left), static_cast<REAL>(rc.bottom - rc.top));

        for (const auto& t : texts) {
            BYTE alpha = static_cast<BYTE>(t.opacity * 255.0);
            Gdiplus::Color color(alpha, t.fill.r, t.fill.g, t.fill.b);
            SolidBrush brush(color);
            FontFamily fontFamily(L"Arial");
            Font font(&fontFamily, (REAL)t.font_size, FontStyleRegular, UnitPixel);
            std::wstring wcontent = Utf8ToWstring(t.content);
            graphics.DrawString(
                wcontent.c_str(), -1, &font,
                PointF((REAL)t.point.x, (REAL)t.point.y), &brush
            );
        }
        EndPaint(hwnd, &ps);
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"SVGWindow";
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(wc.lpszClassName, L"SVG Text Renderer",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1000, 600,
        NULL, NULL, hInstance, NULL);
    
    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return 0;
}
