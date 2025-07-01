#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <string>
#include <sstream>
#include "tinyxml2.h"
using namespace tinyxml2;
using namespace std;
using namespace Gdiplus;

class CustomColor {
public:
    int r, g, b;
    CustomColor(int r = 0, int g = 0, int b = 0) : r(r), g(g), b(b) {}
};

class Point2D {
public:
    double x, y;
    Point2D(double x = 0, double y = 0) : x(x), y(y) {}
};

class Text {
public:
    Point2D point;
    std::string content;
    double font_size;
    CustomColor fill;
    double opacity; 

    Text(Point2D p, const std::string& c, double fs, CustomColor f, double o = 1.0)
        : point(p), content(c), font_size(fs), fill(f), opacity(o) {
    }
};


CustomColor ParseColor(const char* fill) {
    int r = 0, g = 0, b = 0;
    if (fill && strncmp(fill, "rgb", 3) == 0) {
        sscanf_s(fill, "rgb(%d,%d,%d)", &r, &g, &b);
    }
    return CustomColor(r, g, b);
}

vector<Text> ParseSvgText(const char* filename) {
    vector<Text> texts;
    tinyxml2::XMLDocument doc;
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
        CustomColor color = ParseColor(fill);
        double opacity = el->DoubleAttribute("opacity", 1.0); 
        const char* txt = el->GetText();
        std::string content = txt ? txt : "";
        texts.emplace_back(Point2D(x, y), content, font_size, color, opacity); 
    }
    return texts;
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
