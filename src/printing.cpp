#include "printing.hpp"
#include "adult_books.hpp"
#include "book_art.hpp"
#include "child_text.hpp"
#include "locale.hpp"
#include "paint.hpp"
#include "popup.hpp"
#include "references.hpp"
#include <memory>
#include "stories.hpp"
#include <sstream>
namespace biblioteca {
GLuint upload(Paint& p){
    GLuint id=0;
    glGenTextures(1,&id);glBindTexture(gl::texture,id);
    glTexParameteri(gl::texture,gl::wrapS,gl::clampEdge);glTexParameteri(gl::texture,gl::wrapT,gl::clampEdge);
    glTexParameteri(gl::texture,gl::minFilter,gl::mipLinear);glTexParameteri(gl::texture,gl::magFilter,gl::linear);
    glTexImage2D(gl::texture,0,gl::rgba8,p.size.w,p.size.h,0,gl::paintFormat,gl::byte,p.texturePixels());glGenerateMipmap(gl::texture);
    return id;
}
namespace {
void orbit(Paint& p,double x,double y,double rx,double ry,double angle,Color ink,int count=48){
    const double c=std::cos(angle),s=std::sin(angle);
    p.ink(ink);
    for(int i=0;i<=count;++i){
        const double t=2*pi*i/count,ax=rx*std::cos(t),ay=ry*std::sin(t);
        const double px=x+ax*c-ay*s,py=y+ax*s+ay*c;
        if(i==0)cairo_move_to(p.ctx,px,py);else cairo_line_to(p.ctx,px,py);
    }
    cairo_set_line_width(p.ctx,1.7);cairo_stroke(p.ctx);
}
void circle(Paint& p,double x,double y,double radius,Color ink){
    p.ink(ink);cairo_arc(p.ctx,x,y,radius,0,2*pi);cairo_fill(p.ctx);
}

}

Color bindingColor(int id){
    const auto base=color(books()[id].cover);
    return {.090+.72*base.r,.10+.74*base.g,.13+.76*base.b,1};
}
GLuint printSpine(int id){
#ifdef BIBLIOTECA_MOBILE
    Paint p({132,285});
    cairo_scale(p.ctx,.75,.75);
#else
    Paint p({176,380});
#endif
    const auto& book=books()[id];
    const auto wrap=bindingColor(id),ink=color("#eae1bf");
    p.fill({0,0,176,380},wrap);
    p.fill({0,0,12,380},color("#142a39",.30));
    p.fill({164,0,12,380},color("#142a39",.36));
    for(int y:{44,327}){
        p.line(28,y,148,y,color(book.foil,.72),3);
        p.line(57,y+9,119,y+9,color(book.foil,.48),1.5);
    }
    if(!panoramicBook(id) || !wordlessBinding(id)){
        p.font(18,false,true);p.middle(88,364,book.number(id),ink);
        cairo_save(p.ctx);cairo_translate(p.ctx,88,192);cairo_rotate(p.ctx,-pi/2);
        double size=38;
        p.display(size);
        while(p.width(book.title)>246 && size>13)p.display(--size);
        p.middle(0,10,book.title,ink);cairo_restore(p.ctx);
    }else{
        orbit(p,88,190,39,39,0,color(book.foil,.56),28);
        orbit(p,88,190,57,23,-.66,ink,28);
    }
    return upload(p);
}
GLuint printRoomPlaque(std::string_view title){
    Paint p({512,78});
    const auto ink=color("#f4e9ca");
    p.fill({0,0,512,78},color("#193f42"));
    p.line(10,8,502,8,color("#b69767"),1.5);
    p.line(10,70,502,70,color("#b69767"),1.5);
    p.display(42);p.middle(256,53,title,ink);
    return upload(p);
}
namespace {
struct CoverPhoto { const char* file; double x,y; };

CoverPhoto coverPhoto(int id){
    using Art=Illustration;
    switch(illustrationFor(id)){
        case Art::Eagle: return id%2==0?CoverPhoto{"astronauta",.54,.44}:CoverPhoto{"luna",.47,.50};
        case Art::Intrepid: return {"apollo",.52,.50};
        case Art::Falcon: return {"cohete2",.50,.50};
        case Art::Kepler: return {"kepler",.52,.50};
        case Art::Pioneer: return {"pioneer",.50,.46};
        case Art::Insight: return {"insight",.53,.50};
        case Art::Rovers:
            if(id%3==0)return {"marte",.54,.48};
            if(id%3==1)return {"rover",.47,.44};
            return {"perseverance",.75,.47};
        case Art::Earth: return id%2==0?CoverPhoto{"tierra",.50,.50}:CoverPhoto{"tierra2",.50,.46};
        case Art::Europa: return id%2==0?CoverPhoto{"europa",.48,.5}:CoverPhoto{"jupiter",.52,.48};
        case Art::Orbit:
            if(id%4==0)return {"estacion",.53,.49};
            if(id%4==1)return {"cohete",.50,.44};
            if(id%4==2)return {"hubble",.49,.50};
            return {"antena",.51,.48};
        case Art::Saturn: return id%2==0?CoverPhoto{"saturno",.53,.44}:CoverPhoto{"saturno2",.52,.45};
        case Art::Sun: return id%2==0?CoverPhoto{"sol",.51,.51}:CoverPhoto{"sol2",.49,.46};
    }
    return {"tierra",.50,.50};
}

bool paintPhoto(Paint& p,Rect area,CoverPhoto photo){
    const auto path=std::filesystem::path(photo.file).is_absolute()?std::filesystem::path(photo.file)
        :assetsPath()/"covers"/(std::string(photo.file)+".png");
    const std::unique_ptr<cairo_surface_t,decltype(&cairo_surface_destroy)> image(
        cairo_image_surface_create_from_png(path.string().c_str()),cairo_surface_destroy);
    if(cairo_surface_status(image.get())!=0)return false;
    const double w=cairo_image_surface_get_width(image.get()),h=cairo_image_surface_get_height(image.get());
    const double scale=std::max(area.w/w,area.h/h);
    cairo_save(p.ctx);
    cairo_rectangle(p.ctx,area.x,area.y,area.w,area.h);cairo_clip(p.ctx);
    cairo_translate(p.ctx,area.x+(area.w-w*scale)*photo.x,area.y+(area.h-h*scale)*photo.y);
    cairo_scale(p.ctx,scale,scale);
    cairo_set_source_surface(p.ctx,image.get(),0,0);cairo_paint(p.ctx);
    cairo_restore(p.ctx);
    return true;
}

void paintCitation(Paint& p,const Citation& citation,int row,Color ink,Color accent){
    constexpr double font=35,step=45,left=60,right=700;
    const double link=referenceLinkY(row)*referencePageHeight;
    p.font(font,true);
    const auto title=citation.text.find(citation.title);
    const auto titleEnd=title+citation.title.size();
    const double space=p.width(" ");
    double x=left,y=link-181;
    for(size_t at=0;at<citation.text.size();){
        const auto next=citation.text.find(' ',at);
        const auto end=next==std::string_view::npos?citation.text.size():next;
        const auto word=citation.text.substr(at,end-at);
        const double width=p.width(word);
        if(x>left && x+width>right){x=left;y+=step;}
        if(title!=std::string_view::npos && at>=title && at<titleEnd){
            const auto italicLength=std::min(end,titleEnd)-at;
            cairo_save(p.ctx);
            const cairo_matrix_t slant{font,0,-6.6,font,0,0};
            cairo_set_font_matrix(p.ctx,&slant);
            p.text(x,y,word.substr(0,italicLength),ink);
            cairo_restore(p.ctx);
            if(italicLength<word.size())
                p.text(x+p.width(word.substr(0,italicLength)),y,word.substr(italicLength),ink);
        }else p.text(x,y,word,ink);
        x+=width+space;
        at=end+1;
    }
    p.font(24,false,true);
    std::string_view url(citation.url);
    double linkY=p.width(url)>right-left?link-31:link;
    while(!url.empty()){
        size_t count=url.size();
        while(p.width(url.substr(0,count))>right-left)--count;
        if(count<url.size()){
            const auto separator=url.find_last_of("/-?&",count-1);
            if(separator!=std::string_view::npos && separator>count*2/3)count=separator+1;
        }
        const auto line=url.substr(0,count);
        p.text(left,linkY,line,accent);
        p.line(left,linkY+5,left+p.width(line),linkY+5,accent,1.2);
        url.remove_prefix(count);linkY+=31;
    }
}
}

GLuint printCover(int id,bool shelf){
    const auto& book=books()[id];
    const auto foil=color(book.foil),ink=color("#fff4dc");
    const auto binding=bindingColor(id);
#ifdef BIBLIOTECA_MOBILE
    Paint p(shelf?Size{204,290}:Size{500,710});
#else
    Paint p(shelf?Size{272,386}:Size{500,710});
#endif
    cairo_scale(p.ctx,p.size.w/500.,p.size.h/710.);
    p.fill({0,0,500,710},binding);
    constexpr Rect artwork{28,15,458,680};
    const auto custom=assetsPath()/"content"/"covers"/(std::to_string(id)+".png");
    if(!paintPhoto(p,artwork,{custom.string().c_str(),.5,.5}) && !paintPhoto(p,artwork,coverPhoto(id)))
        illustrate(p,artwork,illustrationFor(id),id%7,false);
    const Color night{binding.r*.20,binding.g*.20,binding.b*.25,1};
    p.gradient({28,335,458,360},28,335,28,691,{night.r,night.g,night.b,0},night);
    p.fill({0,0,24,710},binding);
    p.fill({23,15,5,680},foil);
    p.line(40,27,474,27,color("#fff4dc",.52),1.25);
    p.line(40,681,474,681,color("#fff4dc",.61),1.25);
    p.line(52,476,111,476,foil,4);
    double title=64;
    p.display(title);
    while(p.width(book.title)>860 && title>36)p.display(--title);
    p.paragraph({51,495,404,170},book.title,ink,false,title*.92);
    return upload(p);
}
GLuint printReferencePage(int index){
    Paint p({referencePageWidth,referencePageHeight});
    constexpr double w=referencePageWidth,h=referencePageHeight;
    const bool mars=index<2;
    const auto ink=color("#243231"), accent=color(mars?"#854226":"#245763");
    const auto fine=color(mars?"#bf9d82":"#9aafa8");
    const auto page=color("#f8edda");
    p.fill({0,0,w,h},page);
    p.gradient({0,0,w,h},0,0,w,h,color("#d0ab74",.24),color("#fff9eb",.45));
    p.round({26,24,w-52,h-48},9,color("#f8edda",0),fine);
    p.fill({48,51,6,75},accent);
    p.display(69);p.text(69,112,mars?(isEnglish()?"Mars":"Marte"):(isEnglish()?"Moon":"Luna"),ink);
    p.line(60,151,w-60,151,fine,2);
    p.line(60,435,w-60,435,fine,2);
    p.line(60,729,w-60,729,fine,2);
    for(int row=0;row<2;++row)paintCitation(p,references[index/2][(index%2)*2+row],row,ink,accent);
    p.font(28,false,true);p.middle(w/2,771,std::to_string(index+1),accent);
    return upload(p);
}
namespace {
std::string_view pageHeading(int id,int index){
    if(id>=firstImport)return importedBooks()[id-firstImport].text.chapters[index/2].title;
    if(id>=originalCount)return adultBooks()[id-originalCount].chapters[index/2].title;
    if(bookSection(id)==Section::Adult)return chapter(id,index).title;
    return childSpread(id,index/2).title;
}

void body(Paint& p,Rect area,std::string_view content,Color ink,double size,bool serif){
    double step=0;
    std::vector<std::string> lines;
    for(;;){
        p.font(size,serif);
        lines.clear();
        std::istringstream paragraphs{std::string(content)};
        std::string paragraph;
        while(std::getline(paragraphs,paragraph)){
            if(!lines.empty())lines.emplace_back();
            std::istringstream words(paragraph);
            std::string word,line;
            while(words>>word){
                if(!line.empty() && p.width(line+" "+word)>area.w){
                    lines.push_back(std::move(line));
                    line.clear();
                }
                if(!line.empty())line+=' ';
                line+=word;
            }
            if(!line.empty())lines.push_back(std::move(line));
        }
        step=size*1.25;
        double required=0;
        for(const auto& line:lines)required+=line.empty()?step*.53:step;
        if(required<=area.h)break;
        size-=1;
    }
    double y=area.y+size*.80;
    for(const auto& line:lines){
        if(!line.empty())p.text(area.x,y,line,ink);
        y+=line.empty()?step*.53:step;
    }
}

struct PagePalette {std::string_view sky,glow,edge,accent,ink,paper;};
const PagePalette& pagePalette(int id){
    static constexpr PagePalette palettes[]{
        {"#163b46","#237078","#347c76","#79dfc2","#fff8df","#ebf6e8"},
        {"#273c63","#387191","#547a9e","#9ce5de","#f2fbff","#edf7f3"},
        {"#253755","#3a598a","#516591","#f3be78","#fff8e9","#f2f0e7"},
        {"#522e36","#a95545","#a95555","#ffc776","#fff3d1","#fcf1d8"},
        {"#2b3759","#566b98","#607194","#c3d8f1","#f7f4ff","#f2f0eb"},
        {"#4b324c","#775975","#816575","#f4c887","#fff4df","#fff3d9"},
        {"#683d30","#b97d3c","#a47345","#ffe0a1","#fff5d8","#fff3d5"}
    };
    static constexpr std::array imported{Destination::Moon,Destination::Moon,Destination::Moon,
        Destination::Orbit,Destination::Saturn,Destination::Saturn,Destination::Mars,Destination::Mars};
    const auto place=id>=firstImport && bookSection(id)==Section::Children?imported[id-firstImport]:destination(id);
    return palettes[static_cast<int>(place)];
}

GLuint illustratedPage(int id,int index){
    const auto& book=books()[id];
    const bool technical=bookSection(id)==Section::Adult,right=index%2;
    const auto ink=color("#182f3b"),rule=color(technical?"#506e6b":"#4d6981");
    const auto art=illustrationFor(id);
    Paint p({666,954});
    cairo_scale(p.ctx,666./558.,954./800.);
    const auto& palette=pagePalette(id);
    p.fill({0,0,558,800},color(technical?"#eee8d4":palette.paper));
    const double left=right?55:43;
    constexpr double width=460;
    const auto label=std::string_view(book.title);
    p.font(19,false,true);
    if(p.width(label)>width)p.font(16,false,true);
    if(technical){
        p.text(left,49,label,rule);
        p.line(left,62,left+width,62,rule,1.9);
        p.fill({left,83,width,108},color("#c7d6ca"));
        p.line(left,194,left+width,194,rule,1.4);
        if(right)illustrate(p,{left+199,84,261,109},art,index,true);
    }else{
        p.fill({0,0,558,335},color(palette.sky));
        p.fill({0,73,558,247},color(palette.glow));
        p.text(left,53,label,color(palette.ink));
        p.line(left,67,left+width,67,color(palette.accent,.58),1.4);
        for(int i=0;i<4;++i)
            orbit(p,right?86:458,204,76+i*45,27+i*22,-.4,color(palette.accent,.35-i*.044),56);
        for(int i=0;i<31;++i){
            const double x=left+12+(i*91+index*55)%425,y=92+(i*47)%215;
            circle(p,x,y,1.05+double(i%5)*.26,color(palette.ink,i%9==0?.92:.46));
        }
        p.fill({0,329,558,6},color(palette.accent));
    }
    const auto heading=right && !technical?popupAssets()[popupIndex(id,index/2)].name:pageHeading(id,index);
    const double firstLine=technical?274:415;
    double px=technical?38:right?33:48;
    p.display(px);
    while(p.width(heading)>width && px>20)p.display(--px);
    p.text(left,firstLine-17,heading,ink);
    body(p,{left,firstLine,width,716-firstLine},pageText(id,index),ink,technical?27:34,technical);
    p.line(left,736,left+width,736,rule,1.4);
    p.font(19,false,true);
    p.text(right?left+width-26:left,772,std::to_string(index+1),rule);
    return upload(p);
}

unsigned starHash(unsigned n){
    n=(n^61u)^(n>>16);n*=9;n^=n>>4;n*=0x27d4eb2du;return n^(n>>15);
}

void spreadSky(Paint& p,int id,int index){
    const auto& palette=pagePalette(id);
    const double shift=(index%2)*512;
    const auto accent=color(palette.accent,.83);
    p.fill({0,0,512,734},color(palette.sky));
    p.fill({0,83,512,326},color(palette.glow));
    p.line(0,82,512,82,color(palette.accent,.78),1.3);
    // Coordinates match across the fold, including while the leaf turns.
    for(int i=0;i<61;++i){
        const unsigned seed=unsigned(id*353+index/2*8137+i*137);
        const double x=double(starHash(seed)%1024)-shift,y=103.+starHash(seed+313u)%283;
        if(x<-4 || x>516)continue;
        const int kind=i%13;
        const double radius=kind==0?2.5:.65+.30*(i%4);
        circle(p,x,y,radius,color(palette.ink,kind==0?.96:.38+.09*(i%5)));
        if(kind==0){
            p.line(x-6,y,x+6,y,accent,.75);
            p.line(x,y-7,x,y+7,accent,.75);
        }
    }
    const double center=520-shift;
    const double tilt=-.22+.09*static_cast<int>(destination(id));
    for(int i=0;i<2;++i)
        orbit(p,center,244,322.+i*98,68.+i*66,tilt,color(palette.accent,.51-.20*i),110);
    const double feature=(index/2)%2==0?157.:882.;
    const double planetX=feature-shift;
    if(planetX>-135 && planetX<647){
        orbit(p,planetX,227,86,86,0,color(palette.accent,.46),56);
        orbit(p,planetX,227,86,39,tilt,color(palette.accent,.37),48);
        orbit(p,planetX,227,37,86,tilt,color(palette.accent,.32),48);
    }
    p.line(22,711,490,711,color(palette.accent,.43),1.4);
    for(double x:{21.,491.}){
        p.line(x,29,x,697,color(palette.accent,.21),1.2);
        p.line(x-4,31,x+4,31,accent,1.55);
        p.line(x-4,696,x+4,696,accent,1.55);
    }
    p.gradient({0,0,512,734},shift==0?512:0,0,shift==0?481:31,0,
                color("#050a13",.48),color("#050a13",0));
}
}

std::string_view pageEdge(int id){
    return bookSection(id)==Section::Adult?"#c5aa88":pagePalette(id).edge;
}
GLuint printPage(int id,int index){
    if(!panoramicBook(id))return illustratedPage(id,index);
    Paint p({688,986});
    cairo_scale(p.ctx,688./512.,986./734.);
    const auto& book=books()[id];
    const auto& palette=pagePalette(id);
    const auto accent=color(palette.accent);
    const auto ink=color("#192f39"),paper=color(palette.paper);
    spreadSky(p,id,index);
    p.fill({0,409,512,293},paper);
    p.fill({0,409,512,6},accent);
    double font=43;
    p.display(font);
    const auto heading=index%2?popupAssets()[popupIndex(id,index/2)].name:pageHeading(id,index);
    if(index%2){font=33;p.display(font);}
    while(p.width(heading)>433 && font>23)p.display(--font);
    p.text(40,475,heading,ink);
    body(p,{40,511,432,171},pageText(id,index),ink,32,false);
    p.font(21,false,true);
    if(p.width(book.title)>442)p.font(17,false,true);
    p.text(35,62,book.title,color(palette.ink));
    p.text(index%2?456:36,727,std::to_string(index+1),color(palette.ink));
    return upload(p);
}

}
