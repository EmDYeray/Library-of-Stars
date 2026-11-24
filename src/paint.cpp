#include "paint.hpp"
#include <cstdlib>
#include <sstream>
#include <vector>
#ifdef BIBLIOTECA_IPAD
#include "popup.hpp"
#include <cairo-ft.h>
#include <memory>
#include <stdexcept>
#endif

namespace biblioteca {
#ifdef BIBLIOTECA_IPAD
namespace {
struct FontData {
    FT_Library library=nullptr;
    FT_Face face=nullptr;
    ~FontData(){
        if(face)FT_Done_Face(face);
        if(library)FT_Done_FreeType(library);
    }
};
using BundleFont=std::unique_ptr<cairo_font_face_t,decltype(&cairo_font_face_destroy)>;

BundleFont loadFont(const char* name){
    const auto path=assetsPath()/"fonts"/name;
    auto data=std::make_unique<FontData>();
    if(FT_Init_FreeType(&data->library) || FT_New_Face(data->library,path.c_str(),0,&data->face))
        throw std::runtime_error("Could not load "+path.string());
    BundleFont font(cairo_ft_font_face_create_for_ft_face(data->face,0),cairo_font_face_destroy);
    static cairo_user_data_key_t key;
    if(cairo_font_face_set_user_data(font.get(),&key,data.get(),[](void* p){delete static_cast<FontData*>(p);})
           !=CAIRO_STATUS_SUCCESS)
        throw std::runtime_error("Could not register "+path.string());
    data.release();
    return font;
}
}
#endif
Color color(std::string_view hex,double alpha) {
    if(hex.starts_with('#'))hex.remove_prefix(1);
    const auto n=std::strtoul(std::string(hex).c_str(),nullptr,16);
    return {double(n>>16&255)/255,double(n>>8&255)/255,double(n&255)/255,alpha};
}
Paint::Paint(Size size):
    #if defined(_WIN32) || defined(BIBLIOTECA_MOBILE)
    image(cairo_image_surface_create(CAIRO_FORMAT_ARGB32,size.w,size.h)),
#else
    image(cairo_image_surface_create(0,size.w,size.h)),
#endif
    ctx(cairo_create(image)),size(size){}
Paint::~Paint(){cairo_destroy(ctx);cairo_surface_destroy(image);}
unsigned char* Paint::pixels(){cairo_surface_flush(image);return cairo_image_surface_get_data(image);}
const unsigned char* Paint::texturePixels() const {
    cairo_surface_flush(image);
    const auto* pixels=cairo_image_surface_get_data(image);
#ifdef BIBLIOTECA_MOBILE
    rgbaPixels.resize(size_t(size.w)*size.h*4);
    const int stride=cairo_image_surface_get_stride(image);
    for(int y=0;y<size.h;++y)for(int x=0;x<size.w;++x){
        const auto* p=pixels+size_t(y)*stride+x*4;
        auto* q=rgbaPixels.data()+(size_t(y)*size.w+x)*4;
        q[0]=p[2];q[1]=p[1];q[2]=p[0];q[3]=p[3];
    }
    return rgbaPixels.data();
#endif
    return pixels;
}
void Paint::ink(Color c){cairo_set_source_rgba(ctx,c.r,c.g,c.b,c.a);}
void Paint::fill(Rect r,Color c){ink(c);cairo_rectangle(ctx,r.x,r.y,r.w,r.h);cairo_fill(ctx);}
void Paint::round(Rect r,double radius,Color inside,Color edge){
    const double x=r.x+r.w,y=r.y+r.h;
    cairo_new_path(ctx);
    cairo_arc(ctx,x-radius,r.y+radius,radius,-pi/2,0);
    cairo_arc(ctx,x-radius,y-radius,radius,0,pi/2);
    cairo_arc(ctx,r.x+radius,y-radius,radius,pi/2,pi);
    cairo_arc(ctx,r.x+radius,r.y+radius,radius,pi,3*pi/2);
    cairo_close_path(ctx);
    ink(inside);cairo_fill_preserve(ctx);ink(edge);cairo_set_line_width(ctx,1);cairo_stroke(ctx);
}
void Paint::line(double x,double y,double x2,double y2,Color c,double weight){
    ink(c);cairo_move_to(ctx,x,y);cairo_line_to(ctx,x2,y2);cairo_set_line_width(ctx,weight);cairo_stroke(ctx);
}
void Paint::diamond(double x,double y,double r,Color c){
    ink(c);cairo_move_to(ctx,x,y-r);cairo_line_to(ctx,x+r,y);cairo_line_to(ctx,x,y+r);
    cairo_line_to(ctx,x-r,y);cairo_close_path(ctx);cairo_set_line_width(ctx,1.5);cairo_stroke(ctx);
}
void Paint::gradient(Rect r,double x1,double y1,double x2,double y2,Color a,Color b){
    auto* pat=cairo_pattern_create_linear(x1,y1,x2,y2);
    cairo_pattern_add_color_stop_rgba(pat,0,a.r,a.g,a.b,a.a);
    cairo_pattern_add_color_stop_rgba(pat,1,b.r,b.g,b.b,b.a);
    cairo_set_source(ctx,pat);cairo_rectangle(ctx,r.x,r.y,r.w,r.h);cairo_fill(ctx);cairo_pattern_destroy(pat);
}
void Paint::font(double px,bool serif,bool bold){
#ifdef BIBLIOTECA_IPAD
    static const auto regular=loadFont("Barlow-Regular.ttf"),strong=loadFont("Barlow-Bold.ttf");
    (void)serif;
    cairo_set_font_face(ctx,(bold?strong:regular).get());
#elif defined(_WIN32) || defined(__ANDROID__)
    cairo_select_font_face(ctx,serif?"Liberation Serif":"Barlow",CAIRO_FONT_SLANT_NORMAL,
                           bold?CAIRO_FONT_WEIGHT_BOLD:CAIRO_FONT_WEIGHT_NORMAL);
#else
    cairo_select_font_face(ctx,serif?"Liberation Serif":"Barlow",0,bold?1:0);
#endif
    cairo_set_font_size(ctx,px);
}
void Paint::display(double px){
#ifdef BIBLIOTECA_IPAD
    static const auto condensed=loadFont("BarlowCondensed-Bold.ttf");
    cairo_set_font_face(ctx,condensed.get());
#elif defined(_WIN32) || defined(__ANDROID__)
    cairo_select_font_face(ctx,"Barlow Condensed",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_BOLD);
#else
    cairo_select_font_face(ctx,"Barlow Condensed",0,1);
#endif
    cairo_set_font_size(ctx,px);
}
double Paint::width(std::string_view t) const {
    cairo_text_extents_t e{};cairo_text_extents(ctx,std::string(t).c_str(),&e);return e.x_advance;
}
double Paint::metricsHeight() const {cairo_font_extents_t e{};cairo_font_extents(ctx,&e);return e.height;}
void Paint::text(double x,double y,std::string_view t,Color c){
    ink(c);cairo_move_to(ctx,x,y);cairo_show_text(ctx,std::string(t).c_str());
}
void Paint::middle(double x,double y,std::string_view t,Color c){text(x-width(t)/2,y,t,c);}
void Paint::tracked(double x,double y,std::string_view t,Color c,double spacing){
    for(size_t i=0;i<t.size();){
        const auto lead=static_cast<unsigned char>(t[i]);
        const size_t n=lead<128?1:lead<224?2:lead<240?3:4;
        const auto glyph=t.substr(i,n);text(x,y,glyph,c);x+=width(glyph)+spacing;i+=n;
    }
}
void Paint::paragraph(Rect r,std::string_view t,Color c,bool centered,double lineSpacing){
    std::istringstream stream{std::string(t)};
    std::vector<std::string> lines;
    std::string word, line;
    while(stream>>word){
        if(!line.empty() && width(line+" "+word)>r.w){lines.push_back(line);line.clear();}
        if(!line.empty())line+=' ';
        line+=word;
    }
    if(!line.empty())lines.push_back(line);
    const double dy=lineSpacing>0?lineSpacing:metricsHeight()*1.12;
    double baseline=r.y+(centered?std::max(0.,(r.h-dy*lines.size())/2):0)+dy*.79;
    for(const auto& l:lines){
        if(baseline>r.y+r.h)break;
        if(centered)middle(r.x+r.w/2,baseline,l,c);else text(r.x,baseline,l,c);
        baseline+=dy;
    }
}
}
