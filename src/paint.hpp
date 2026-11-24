#pragma once
#include "library.hpp"
#include "native.hpp"
#include <string_view>
#include <vector>

namespace biblioteca {
struct Color { double r=0,g=0,b=0,a=1; };
Color color(std::string_view hex,double alpha=1);
struct Rect {
    double x,y,w,h;
    bool has(Point p) const {return p.x>=x && p.x<=x+w && p.y>=y && p.y<=y+h;}
};
class Paint {
    cairo_surface_t* image;
#ifdef BIBLIOTECA_MOBILE
    mutable std::vector<unsigned char> rgbaPixels;
#endif
public:
    cairo_t* ctx;
    const Size size;
    explicit Paint(Size size);
    ~Paint();
    Paint(const Paint&)=delete;
    Paint& operator=(const Paint&)=delete;
    unsigned char* pixels();
    const unsigned char* texturePixels() const;
    void ink(Color c);
    void fill(Rect r,Color c);
    void round(Rect r,double radius,Color fill,Color edge);
    void line(double x,double y,double x2,double y2,Color c,double weight=1);
    void diamond(double x,double y,double r,Color c);
    void gradient(Rect r,double x1,double y1,double x2,double y2,Color a,Color b);
    void font(double px,bool serif=false,bool bold=false);
    void display(double px);
    double width(std::string_view text) const;
    double metricsHeight() const;
    void text(double x,double baseline,std::string_view text,Color color);
    void middle(double x,double baseline,std::string_view text,Color color);
    void tracked(double x,double baseline,std::string_view text,Color c,double spacing=2);
    void paragraph(Rect r,std::string_view text,Color c,bool centered=false,double lineSpacing=0);
};
}
