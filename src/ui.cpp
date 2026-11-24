#include "ui.hpp"
#include "locale.hpp"
#include "references.hpp"
#include <cctype>
#include <cstdint>

namespace biblioteca {
namespace {
const auto paper=color("#f6f1e7"),hudFill=color("#171b1c",.90);
const auto catalogInk=color("#142c31"),catalogSubtle=color("#345b5f"),catalogRule=color("#668080");
constexpr int rows=7, rowHeight=33;

std::string normalize(std::string_view text){
    constexpr std::array<std::string_view,23> accents{"á","à","ä","é","è","ë","í","ì","ï","ó","ò","ö",
        "ú","ù","ü","ñ","Á","É","Í","Ó","Ú","Ü","Ñ"};
    constexpr std::string_view replacements="aaaeeeiiiooouuunAEIOUUN";
    std::string clean;
    for(size_t i=0;i<text.size();){
        bool folded=false;
        for(size_t j=0;j<accents.size();++j)if(text.substr(i).starts_with(accents[j])){
            clean+=char(std::tolower(static_cast<unsigned char>(replacements[j])));
            i+=accents[j].size();folded=true;break;
        }
        if(!folded)clean+=char(std::tolower(static_cast<unsigned char>(text[i++])));
    }
    return clean;
}
int movementKey(int k){
    if(k=='w' || k==sdl::upKey)return 0;
    if(k=='s' || k==sdl::downKey)return 1;
    if(k=='a')return 2;
    if(k=='d')return 3;
    return -1;
}
double distance(Point a,Point b){return std::abs(a.x-b.x)+std::abs(a.y-b.y);}
void trimChar(std::string& x){
    if(x.empty())return;
    auto end=x.size()-1;
    while(end && (static_cast<unsigned char>(x[end])&0xc0)==0x80)--end;
    x.erase(end);
}
std::string fitLabel(const Paint& p,std::string_view label,double limit){
    if(p.width(label)<=limit)return std::string(label);
    std::string head(label);
    do{trimChar(head);}while(!head.empty() && p.width(head+"…")>limit);
    return head+"…";
}
void button(Paint& p,Rect r,std::string_view text,bool enabled,bool hot,bool onPaper,bool focused,bool pressed){
    const auto fill=onPaper?color(pressed?"#d2dccd":hot?"#e1e9de":"#ecefe7"):
                            pressed?color("#4a4c46",.98):hot?color("#333732",.96):hudFill;
    const auto ink=onPaper?catalogInk:enabled?paper:color("#aaa9a0");
    p.round(r,6,fill,color("#23616a",focused?1.:0.));
    const double x=r.x+r.w/2,y=r.y+r.h/2;
    if(text=="‹" || text=="›"){
        const double sign=text=="‹"?-1:1;
        p.line(x-sign*3,y-5,x+sign*3,y,ink,1.65);
        p.line(x+sign*3,y,x-sign*3,y+5,ink,1.65);
        return;
    }
    if(text=="↑" || text=="↓" || text=="←" || text=="→"){
        const double dx=text=="→"?1.:text=="←"?-1.:0.;
        const double dy=text=="↓"?1.:text=="↑"?-1.:0.;
        const double reach=std::min(r.w,r.h)*.2;
        const double tipX=x+dx*reach,tipY=y+dy*reach;
        const double headX=tipX-dx*reach*.8,headY=tipY-dy*reach*.8;
        const double wing=reach*.55,weight=std::max(3.,reach*.18);
        p.line(x-dx*reach,y-dy*reach,tipX,tipY,ink,weight);
        p.line(tipX,tipY,headX-dy*wing,headY+dx*wing,ink,weight);
        p.line(tipX,tipY,headX+dy*wing,headY-dx*wing,ink,weight);
        return;
    }
    p.font(15,false,true);
    p.middle(x,y+5,fitLabel(p,text,r.w-21),ink);
}
}

Interface::Interface(Size size):size_(size){syncCatalog();}
std::vector<Interface::Button> Interface::buttons() const {
    const double x=size_.w/2.,h=size_.h,w=size_.w;
    constexpr double topY=25,topH=38,topGap=11;
    if(catalog_){
        const auto d=dialog();
        return {{{d.x+25,d.y+16,98,topH},Action::Language,isEnglish()?"EN / ES":"ES / EN",true},
                {{d.x+d.w-112,d.y+16,87,topH},Action::Close,isEnglish()?"Close":"Cerrar",true},
                {{d.x+d.w-194,d.y+76,169,topH},Action::Genre,genres_[filter_]+"  v",true}};
    }
    if(hideControls_)return {};
    if(lib.selected()<0){
        const bool child=lib.section()==Section::Children;
        std::vector<Button> list{{{w-151,topY,119,topH},Action::Catalog,isEnglish()?"Catalog":"Catálogo",true},
                                 {{w-258-topGap,topY,96,topH},Action::Sources,isEnglish()?"Lectern":"Atril",true},
                                 {{31,topY,191,topH},Action::Section,
                                  std::string(isEnglish()?sectionName(child?Section::Adult:Section::Children):(child?"Sala juvenil y adulta":"Sala infantil")),true}};
        if(child){
            list.push_back({{x-55,h-64,45,43},Action::Left,"‹",lib.wing()>0});
            list.push_back({{x+10,h-64,45,43},Action::Right,"›",lib.wing()<1});
        }
#ifdef BIBLIOTECA_MOBILE
#ifdef BIBLIOTECA_IPAD
        const double tile=std::clamp(std::min(h*.15,w*.115),42.,112.);
        constexpr double gap=8,left=24,bottom=20;
#else
        constexpr double tile=112,gap=8,left=24,bottom=20;
#endif
        list.push_back({{left+tile+gap,h-bottom-3*tile-2*gap,tile,tile},Action::Forward,"↑",true});
        list.push_back({{left+tile+gap,h-bottom-tile,tile,tile},Action::Back,"↓",true});
        list.push_back({{left,h-bottom-2*tile-gap,tile,tile},Action::StrafeLeft,"←",true});
        list.push_back({{left+2*(tile+gap),h-bottom-2*tile-gap,tile,tile},Action::StrafeRight,"→",true});
#endif
        list.push_back({{w/2-49,topY,98,topH},Action::Language,isEnglish()?"EN / ES":"ES / EN",true});
        return list;
    }
    const auto stage=lib.stage();
    std::vector<Button> list{
        {{w-151,topY,119,topH},Action::Close,isEnglish()?"Close":"Cerrar",stage!=Stage::Closing && stage!=Stage::Returning}};
    if(stage==Stage::Reading){
        const bool settled=lib.turning()==1;
        list.push_back({{x-93,h-64,43,43},Action::Left,"‹",settled && lib.spread()>0});
        list.push_back({{x+50,h-64,43,43},Action::Right,"›",settled && lib.spread()<spreadCount(lib.selected())-1});
        if(lib.selected()!=referenceBookId)
            list.push_back({{31,topY,103,topH},Action::Motion,motion_?(isEnglish()?"Pause":"Pausar"):(isEnglish()?"Resume":"Reanudar"),true});
#ifdef BIBLIOTECA_IPAD
        if(lib.selected()!=referenceBookId){
            list.push_back({{w-135,h-70,49,49},Action::ZoomOut,"−",true});
            list.push_back({{w-78,h-70,49,49},Action::ZoomIn,"+",true});
        }
#endif
    }
    list.push_back({{w/2-49,topY,98,topH},Action::Language,isEnglish()?"EN / ES":"ES / EN",true});
    return list;
}
Interface::Action Interface::actionAt(Point p,bool includeDisabled) const {
    for(const auto& b:buttons())if((b.enabled || includeDisabled) && b.rect.has(p))return b.action;
    return Action::None;
}
Rect Interface::dialog() const {
    const double w=std::min(620,size_.w-52),h=std::min(425,size_.h-30);
    return {(size_.w-w)/2,(size_.h-h)/2,w,h};
}
const char* Interface::sourceAt(Point p) const {
    const auto page=lib.referencePage(p,size_);
    if(!page)return nullptr;
    const int side=int(page->x);
    const float x=page->x-side;
    if(x<.078f || x>.923f)return nullptr;
    for(int row=0;row<2;++row){
        const auto y=referenceLinkY(row);
        if(page->y>=y-.070f && page->y<=y+.016f)
            return references[lib.spread()][side*2+row].url;
    }
    return nullptr;
}
std::optional<int> Interface::itemAt(Point p) const {
    const auto d=dialog();
    for(int i=0;i<rows && scroll_+i<int(results_.size());++i)
        if(Rect{d.x+25,d.y+132+i*rowHeight,d.w-50,rowHeight}.has(p))return scroll_+i;
    return {};
}
void Interface::refreshCatalog(){
    results_.clear();
    const auto q=normalize(search_);
    for(int i=0;i<bookCount;++i){
        if(bookSection(i)!=catalogRoom_)continue;
        const auto& b=books()[i];
        if(filter_>0 && b.genre!=genres_[filter_])continue;
        if(normalize(b.title+" "+b.genre).find(q)!=std::string::npos)results_.push_back(i);
    }
    cursor_=scroll_=0;dirty=true;
}
void Interface::syncCatalog(){
    catalogRoom_=lib.section();
    genres_.resize(1);
    genres_[0]=isEnglish()?"All genres":"Todos los géneros";
    for(int i=0;i<bookCount;++i){
        if(bookSection(i)!=catalogRoom_)continue;
        const auto& genre=books()[i].genre;
        if(std::find(genres_.begin(),genres_.end(),genre)==genres_.end())genres_.push_back(genre);
    }
    std::sort(genres_.begin()+1,genres_.end());
    search_.clear();filter_=0;refreshCatalog();
}
void Interface::stopWalking(){
    motionKeys_.fill(false);sprintKeys_.fill(false);stride_={};wheelTravel_=0;touchDrive_=Action::None;
}
bool Interface::touchDriveAction(Action a) const {
    return a==Action::Forward || a==Action::Back || a==Action::StrafeLeft || a==Action::StrafeRight;
}
void Interface::setTouchDrive(Action a,bool down){
    int key=-1;
    if(a==Action::Forward)key=0;
    else if(a==Action::Back)key=1;
    else if(a==Action::StrafeLeft)key=2;
    else if(a==Action::StrafeRight)key=3;
    if(key>=0)motionKeys_[key]=down;
}
void Interface::showCatalog(bool show){
    catalog_=show;lib.hover(-1);dirty=true;
    if(show){lib.close();stopWalking();lookDrag_=false;}
    if(show && mouseLook_){mouseLook_=false;sdl::relativeMouse(false);}
    if(show){
        refreshCatalog();
#ifdef BIBLIOTECA_IPAD
        catalogFocus_=0;
#else
        focusCatalog(0);
#endif
    }else sdl::textInput(false);
}
void Interface::openLectern(){
    if(!lib.openReferences())return;
    stopWalking();
    if(mouseLook_)sdl::relativeMouse(false);
    mouseLook_=lookDrag_=drag_=false;
    dirty=true;
}
void Interface::focusCatalog(int field){
    catalogFocus_=field;
    sdl::textInput(field==0);
    dirty=true;
}
float Interface::sceneTime(float dt){
    if(motion_)sceneSeconds_+=dt;
    return sceneSeconds_;
}
void Interface::choose(int index){
    if(index<0 || index>=int(results_.size()))return;
    const int id=results_[index];
    showCatalog(false);lib.visit(id);
}
void Interface::act(Action a){
    switch(a){
        case Action::None:return;
        case Action::Catalog:showCatalog(true);break;
        case Action::Language:setEnglish(!isEnglish());syncCatalog();break;
        case Action::Close:
            if(catalog_)showCatalog(false);
            else lib.close();
            break;
        case Action::Sources:openLectern();break;
        case Action::Genre:filter_=(filter_+1)%int(genres_.size());refreshCatalog();break;
        case Action::Motion:motion_=!motion_;break;
        case Action::Section:
            stopWalking();
            lib.enter(lib.section()==Section::Children?Section::Adult:Section::Children);
            break;
        case Action::Left:case Action::Right:
            if(lib.selected()<0){stopWalking();lib.wingTo(a==Action::Right?1:0);}
            else lib.turn(a==Action::Right?1:-1);
            break;
        case Action::ZoomOut:lib.zoom(-1.55f);break;
        case Action::ZoomIn:lib.zoom(1.55f);break;
        case Action::Forward:case Action::Back:case Action::StrafeLeft:case Action::StrafeRight:
            return;
    }
    dirty=true;
}
void Interface::resize(Size size){size_=size;dirty=true;}
bool Interface::walking() const {
    return !catalog_ && lib.selected()<0 &&
        (motionKeys_[0]!=motionKeys_[1] || motionKeys_[2]!=motionKeys_[3] ||
         dot(stride_,stride_)>.0016f || std::abs(wheelTravel_)>.006f);
}
void Interface::step(float dt){
    const bool active=lib.animating();
    if(walking()){
        const float f=float(motionKeys_[0])-float(motionKeys_[1]),s=float(motionKeys_[3])-float(motionKeys_[2]);
        const float dtUsed=std::min(dt,.1f);
        const float speed=(sprintKeys_[0] || sprintKeys_[1]?7.7f:4.55f)
#ifdef BIBLIOTECA_MOBILE
            *(touchDrive_!=Action::None?2.f:1.f)
#endif
            ;
        const Vec target=Vec{s,0,f}*(speed/std::max(1.f,std::hypot(s,f)));
        const float blend=1-std::exp(-dtUsed*(s!=0 || f!=0?13.f:19.f));
        stride_=stride_+(target-stride_)*blend;
        if(dot(stride_,stride_)<.0016f)stride_={};
        const float rolled=wheelTravel_*(1-std::exp(-dtUsed*16.f));
        wheelTravel_-=rolled;
        if(std::abs(wheelTravel_)<.006f)wheelTravel_=0;
        lib.walk(stride_.z*dtUsed+rolled,stride_.x*dtUsed);
        dirty=true;
    }
    lib.setMotionEnabled(motion_);
    lib.update(dt);
    if(catalogRoom_!=lib.section())syncCatalog();
    const Point target=mouseLook_?Point{size_.w*.5f,size_.h*.5f}:pos_;
    const int hit=(!catalog_ && !lookDrag_ && (mouseLook_ || actionAt(pos_)==Action::None))
        ?lib.pick(target,size_):-1;
    if(hit!=lib.hovered()){lib.hover(hit);dirty=true;}
    dirty=dirty || active;
}
int Interface::pointerKind() const {
    if(drag_ || lookDrag_ || mouseLook_)return sdl::grabCursor;
    const bool clickable=actionAt(pos_)!=Action::None || lib.hovered()>=0 ||
        (catalog_ && itemAt(pos_)) || sourceAt(pos_) || (!catalog_ && lib.pickReferences(pos_,size_));
    const bool movableBook=lib.selected()!=referenceBookId && lib.onReader(pos_,size_);
    return clickable || movableBook?sdl::handCursor:sdl::arrowCursor;
}
void Interface::cancelInput(){
    drag_=lookDrag_=catalogDrag_=touchSwipe_=false;
    pressed_=Action::None;
    stopWalking();
#ifdef BIBLIOTECA_MOBILE
    touchDriveFinger_=touchLookFinger_=-1;
    touchLookMoved_=false;
#endif
#ifdef BIBLIOTECA_IPAD
    touchPointer_=-1;
#endif
    if(mouseLook_){mouseLook_=false;sdl::relativeMouse(false);}
    dirty=true;
}
#ifdef BIBLIOTECA_IPAD
void Interface::fingerPointer(SDL_EventType type,Point point,SDL_Window* window){
    SDL_Event pointer{};
    pointer.type=type;
    if(type==sdl::motion){
        pointer.motion.which=SDL_TOUCH_MOUSEID;
        pointer.motion.x=point.x;pointer.motion.y=point.y;
    }else{
        pointer.button.which=SDL_TOUCH_MOUSEID;
        pointer.button.button=SDL_BUTTON_LEFT;
        pointer.button.x=point.x;pointer.button.y=point.y;
    }
    event(pointer,window);
}
#endif
void Interface::event(const SDL_Event& e,SDL_Window* window){
    switch(e.type){
    case sdl::quit:quit=true;break;
#ifdef BIBLIOTECA_IPAD
    case SDL_EVENT_WINDOW_MOUSE_LEAVE:pos_={-100,-100};dirty=true;break;
    case SDL_EVENT_WINDOW_FOCUS_LOST:
    case SDL_EVENT_DID_ENTER_BACKGROUND:cancelInput();break;
    case SDL_EVENT_FINGER_CANCELED:
        if(sdl::fingerId(e.tfinger)==touchDriveFinger_){
            setTouchDrive(touchDrive_,false);touchDrive_=Action::None;touchDriveFinger_=-1;
        }
        if(sdl::fingerId(e.tfinger)==touchLookFinger_ || sdl::fingerId(e.tfinger)==touchPointer_){
            touchLookFinger_=touchPointer_=-1;
            touchLookMoved_=drag_=catalogDrag_=touchSwipe_=false;
            pressed_=Action::None;
        }
        dirty=true;break;
#else
    case sdl::window:
        if(e.window.event==11)pos_={-100,-100};
        if(e.window.event==13)cancelInput();
        dirty=true;break;
#endif
#ifdef BIBLIOTECA_MOBILE
    case sdl::fingerDown:{
        const Point p{e.tfinger.x*size_.w,e.tfinger.y*size_.h};
        const auto action=actionAt(p);
        if(touchDriveFinger_==-1 && touchDriveAction(action)){
            touchDriveFinger_=sdl::fingerId(e.tfinger);
            touchDrive_=action;
            setTouchDrive(action,true);
            dirty=true;
            break;
        }
#ifdef BIBLIOTECA_IPAD
        if(touchPointer_!=SDL_FingerID(-1))break;
        touchPointer_=sdl::fingerId(e.tfinger);
        fingerPointer(sdl::down,p,window);
#endif
        if(touchLookFinger_==-1 && !catalog_ && lib.selected()<0 &&
                 p.x>size_.w*.45f && action==Action::None){
            touchLookFinger_=sdl::fingerId(e.tfinger);
            touchLookLast_=p;
            touchLookMoved_=false;
        }
        break;
    }
    case sdl::fingerMotion:{
        const Point p{e.tfinger.x*size_.w,e.tfinger.y*size_.h};
        if(sdl::fingerId(e.tfinger)==touchDriveFinger_){
            const auto next=actionAt(p);
            if(next!=touchDrive_){
                setTouchDrive(touchDrive_,false);
                touchDrive_=touchDriveAction(next)?next:Action::None;
                setTouchDrive(touchDrive_,true);
                dirty=true;
            }
        }else if(sdl::fingerId(e.tfinger)==touchLookFinger_){
            const float dx=p.x-touchLookLast_.x,dy=p.y-touchLookLast_.y;
            if(std::abs(dx)+std::abs(dy)>2.f){
                touchLookMoved_=true;
                moved_=true;
                lib.look(dx,dy);
                dirty=true;
            }
            touchLookLast_=p;
        }
#ifdef BIBLIOTECA_IPAD
        if(sdl::fingerId(e.tfinger)==touchPointer_)fingerPointer(sdl::motion,p,window);
#endif
        break;
    }
    case sdl::fingerUp:
        if(sdl::fingerId(e.tfinger)==touchDriveFinger_){
            setTouchDrive(touchDrive_,false);
            touchDrive_=Action::None;
            touchDriveFinger_=-1;
            dirty=true;
        }
        if(sdl::fingerId(e.tfinger)==touchLookFinger_){
            if(touchLookMoved_)moved_=true;
            touchLookFinger_=-1;
            touchLookMoved_=false;
        }
#ifdef BIBLIOTECA_IPAD
        if(sdl::fingerId(e.tfinger)==touchPointer_){
            touchPointer_=-1;
            fingerPointer(sdl::up,{e.tfinger.x*size_.w,e.tfinger.y*size_.h},window);
        }
#endif
        break;
#endif
    case sdl::keyUp:{
        const int code=sdl::keyCode(e.key);
        if(const int key=movementKey(code);key>=0)motionKeys_[key]=false;
        if(code==sdl::leftShift)sprintKeys_[0]=false;
        if(code==sdl::rightShift)sprintKeys_[1]=false;
        break;
    }
    case sdl::keyDown:{
        const int k=sdl::keyCode(e.key);
        if(k==9 && !catalog_ && lib.selected()<0 && !e.key.repeat){
            const bool request=!mouseLook_;
            if(!window || sdl::relativeMouse(request)==0)mouseLook_=request;
            lookDrag_=false;dirty=true;break;
        }
        if(k==1073741890 && !e.key.repeat){act(Action::Language);break;} // F9
        if(k==sdl::f2){hideControls_=!hideControls_;dirty=true;break;}
        if(k==sdl::f11 && window){full_=!full_;sdl::fullscreen(window,full_);break;}
        if(catalog_){
            if(k==27)showCatalog(false);
            else if(k==9 && !e.key.repeat)focusCatalog((catalogFocus_+(sdl::shiftHeld(e.key)?2:1))%3);
            else if(k==8 && catalogFocus_==0){trimChar(search_);refreshCatalog();}
            else if(k==13 && catalogFocus_==0)choose(cursor_);
            else if((k==13 || k==' ') && catalogFocus_!=0){
                act(catalogFocus_==1?Action::Genre:Action::Close);
            }else if(catalogFocus_==0 && !results_.empty() && (k==sdl::upKey || k==sdl::downKey)){
                cursor_=std::clamp(cursor_+(k==sdl::upKey?-1:1),0,int(results_.size())-1);
                scroll_=std::clamp(scroll_,std::max(0,cursor_-rows+1),cursor_);dirty=true;
            }
        }else{
            if(k==27){if(mouseLook_){mouseLook_=false;sdl::relativeMouse(false);}
                else if(lib.selected()>=0 || lib.stage()==Stage::Traveling)lib.close();
                else if(full_ && window){full_=false;sdl::fullscreen(window,false);}}
            else if(k==sdl::left)act(Action::Left);
            else if(k==sdl::right)act(Action::Right);
            else if(k=='m' && !e.key.repeat)act(Action::Motion);
            else if((k=='/' || k=='k') && lib.selected()<0 && !e.key.repeat)showCatalog(true);
            else if(k=='f' && lib.selected()<0 && !e.key.repeat)openLectern();
            else if(k=='g' && lib.selected()<0 && !e.key.repeat)act(Action::Section);
            else if(k==sdl::leftShift || k==sdl::rightShift)sprintKeys_[k==sdl::rightShift]=true;
            else if((k==13 || k=='e') && !catalog_ && lib.pickReferences(
                    mouseLook_?Point{size_.w*.5f,size_.h*.5f}:pos_,size_))openLectern();
            else if((k==13 || k=='e') && lib.hovered()>=0){
                if(lib.open(lib.hovered())){
                    stopWalking();
                    if(mouseLook_){mouseLook_=false;sdl::relativeMouse(false);}
                }
            }else if(const int key=movementKey(k);key>=0 && lib.selected()<0)motionKeys_[key]=true;
        }
        dirty=true;break;
    }
    case sdl::text:
        if(catalog_ && catalogFocus_==0 && search_.size()<96 && std::string_view(e.text.text)!="/"){
            search_+=e.text.text;refreshCatalog();
        }
        break;
    case sdl::motion:{
        pos_={float(e.motion.x),float(e.motion.y)};
#ifdef BIBLIOTECA_IPAD
        if(e.motion.which==SDL_TOUCH_MOUSEID && catalogDrag_){
            const int steps=int((last_.y-pos_.y)/rowHeight);
            if(steps){
                scroll_=std::clamp(scroll_+steps,0,std::max(0,int(results_.size())-rows));
                last_.y-=steps*rowHeight;
                dirty=true;
            }
            if(distance(pos_,start_)>8)moved_=true;
            break;
        }
        if(e.motion.which==SDL_TOUCH_MOUSEID && touchSwipe_)break;
#endif
        if((lookDrag_ || mouseLook_) && !catalog_ && lib.selected()<0){
            const float dx=mouseLook_ || e.motion.xrel!=0?float(e.motion.xrel):pos_.x-last_.x;
            const float dy=mouseLook_ || e.motion.yrel!=0?float(e.motion.yrel):pos_.y-last_.y;
            lib.look(dx,dy);last_=pos_;moved_=true;dirty=true;
        }else if(drag_){
            if(distance(pos_,start_)>5)moved_=true;
            if(moved_)lib.orbit(pos_.x-last_.x,pos_.y-last_.y);
            last_=pos_;
        }
        break;
    }
    case sdl::down:
        if(e.button.button==3 && !catalog_ && lib.selected()<0){
            lookDrag_=true;last_={float(e.button.x),float(e.button.y)};dirty=true;
        }else if(e.button.button==1){
            start_=last_=pos_={float(e.button.x),float(e.button.y)};
            pressed_=mouseLook_?Action::None:actionAt(pos_,true);
            moved_=false;
            drag_=!catalog_ && pressed_==Action::None && lib.selected()!=referenceBookId && lib.onReader(pos_,size_);
#ifdef BIBLIOTECA_IPAD
            if(e.button.which==SDL_TOUCH_MOUSEID && pressed_==Action::None){
                const auto d=dialog();
                catalogDrag_=catalog_ && d.has(pos_) && pos_.y>=d.y+132;
                touchSwipe_=!catalog_ && lib.stage()==Stage::Reading && pos_.y>size_.h*.67f;
            }
#endif
            dirty=true;
        }
        break;
    case sdl::up:
        if(e.button.button==3){lookDrag_=false;dirty=true;}
        else if(e.button.button==1 && mouseLook_){
            const Point center{size_.w*.5f,size_.h*.5f};
            if(lib.pickReferences(center,size_))openLectern();
            else if(const int id=lib.pick(center,size_);id>=0 && lib.open(id)){
                stopWalking();mouseLook_=false;sdl::relativeMouse(false);dirty=true;
            }
        }else if(e.button.button==1){
            pos_={float(e.button.x),float(e.button.y)};
            const auto a=actionAt(pos_);
#ifdef BIBLIOTECA_IPAD
            const float dx=pos_.x-start_.x,dy=pos_.y-start_.y;
            if(touchSwipe_ && e.button.which==SDL_TOUCH_MOUSEID &&
               std::abs(dx)>48 && std::abs(dx)>std::abs(dy)*1.4f){
                act(dx<0?Action::Right:Action::Left);
                moved_=true;
            }
#endif
            if(!moved_ && distance(pos_,start_)<=5){
                if(a!=Action::None && pressed_==a)act(a);
                else if(a==Action::None && pressed_==Action::None && actionAt(pos_,true)==Action::None){
                    if(const auto url=sourceAt(pos_))SDL_OpenURL(url);
                    else if(catalog_){
                        const auto d=dialog();
                        if(auto i=itemAt(pos_))choose(*i);
                        else if(Rect{d.x+25,d.y+76,d.w-222,39}.has(pos_))focusCatalog(0);
                        else if(!d.has(pos_))showCatalog(false);
                    }
                    else if(lib.pickReferences(pos_,size_))openLectern();
                    else if(int id=lib.pick(pos_,size_);id>=0){if(lib.open(id))stopWalking();}
                    else if(lib.stage()==Stage::Reading && !lib.onReader(pos_,size_))lib.close();
                }
            }
            drag_=catalogDrag_=touchSwipe_=false;pressed_=Action::None;dirty=true;
        }
        break;
    case sdl::wheel:{
        const float wheel=sdl::verticalWheel(e.wheel);
        const float y=e.wheel.direction==1?-wheel:wheel;
        if(catalog_){
            const int move=y==0?0:int(std::copysign(std::max(1.f,std::round(std::abs(y))),y));
            scroll_=std::clamp(scroll_-move,0,std::max(0,int(results_.size())-rows));
            dirty=true;
        }else if(lib.selected()<0){
            wheelTravel_=std::clamp(wheelTravel_+y*.82f,-3.3f,3.3f);
            dirty=true;
        }
        else lib.zoom(y);
        break;
    }
    default:break;
    }
}
std::string Interface::signature() const {
    return std::to_string(int(lib.section()))+":"+std::to_string(lib.selected())+":"+std::to_string(lib.wing())+":"+std::to_string(lib.hovered())+
        ":"+std::to_string(lib.spread())+":"+std::to_string(int(lib.stage()))+":"+std::to_string(lib.turning()==1)+
        ":"+std::to_string(int(actionAt(pos_)))+":"+std::to_string(catalog_)+":"+std::to_string(cursor_)+
        ":"+std::to_string(itemAt(pos_).value_or(-1))+":"+std::to_string(filter_)+":"+std::to_string(scroll_)+
        ":"+search_+":"+std::to_string(mouseLook_)+":"+std::to_string(hideControls_)+":"+std::to_string(size_.w)+":"+std::to_string(size_.h)+
        ":"+std::to_string(motion_)+":"+std::to_string(catalogFocus_)+":"+std::to_string(int(pressed_))+
        ":"+std::to_string(isEnglish());
}
void Interface::paint(Paint& p) const {
    if(hideControls_ && !catalog_)return;
    const double w=size_.w,h=size_.h,cx=w/2;
    const bool reading=lib.selected()>=0;
    if(!reading && !catalog_){
        if(mouseLook_){
            p.line(cx-4,h*.5,cx+4,h*.5,paper,1);
            p.line(cx,h*.5-4,cx,h*.5+4,paper,1);
        }
        if(const int id=lib.hovered();id>=0){
            const auto& slot=positions()[id];
            const auto at=lib.project(slot.spine()+Vec{0,slot.height()/2,0},size_);
            p.font(16,false,true);
            const auto title=fitLabel(p,books()[id].title,254);
            const auto width=p.width(title)+24;
            const double x=std::clamp(double(at.x)-width/2,16.,w-width-16.),y=std::max(72.,double(at.y)-47.);
            p.round({x,y,width,30},6,hudFill,color("#171b1c",0));
            p.text(x+12,y+20,title,paper);
        }
    }else if(reading && lib.stage()==Stage::Reading){
        const int first=lib.spread()*2+1;
        const auto pages=std::to_string(first)+"–"+std::to_string(first+1)+" / "+
                         std::to_string(spreadCount(lib.selected())*2);
        p.round({cx-44,h-64,88,43},6,hudFill,color("#171b1c",0));
        p.font(15,false,true);p.middle(cx,h-37,pages,paper);
    }
    if(catalog_){
        p.fill({0,0,w,h},color("#0e2129",.70));
        const auto d=dialog();
        p.round(d,6,color("#f5ebd3"),catalogRule);
        p.display(39);p.text(d.x+25,d.y+44,isEnglish()?"Catalog":"Catálogo",catalogInk);
        p.font(15,false,true);p.text(d.x+161,d.y+41,isEnglish()?sectionName(catalogRoom_):(catalogRoom_==Section::Children?"Sala infantil":"Sala juvenil y adulta"),catalogSubtle);
        const Rect field{d.x+25,d.y+76,d.w-222,39};
        p.round(field,3,color("#fff8e7"),catalogFocus_==0?color("#185e64"):catalogRule);
        if(catalogFocus_==0)p.round({field.x-3,field.y-3,field.w+6,field.h+6},5,color("#185e64",0),color("#185e64"));
        p.font(15);
        if(search_.empty())p.text(d.x+37,d.y+101,isEnglish()?"Search this room":"Buscar en esta sala",catalogSubtle);
        else{
            std::string display=search_;
            while(display.size()>2 && p.width(display)>d.w-242){
                size_t length=1;
                while(length<display.size() && (static_cast<unsigned char>(display[length])&0xc0)==0x80)++length;
                display.erase(0,length);
            }
            p.text(d.x+37,d.y+101,display,catalogInk);
        }
        if(results_.empty()){
            p.font(18);p.middle(cx,d.y+183,isEnglish()?"Try another title or change the genre.":"Prueba otro título o cambia el género.",catalogSubtle);
        }
        for(int i=0;i<rows && scroll_+i<int(results_.size());++i){
            const auto id=results_[scroll_+i];const auto& b=books()[id];const auto y=d.y+132+i*rowHeight;
            const auto item=itemAt(pos_);
            if(scroll_+i==cursor_ || item==scroll_+i)
                p.round({d.x+25,y,d.w-50,rowHeight-1},2,color("#d5e6db"),
                        scroll_+i==cursor_ && catalogFocus_==0?color("#185e64"):catalogRule);
            else p.line(d.x+26,y+rowHeight,d.x+d.w-26,y+rowHeight,color("#a2ada2",.65));
            p.fill({d.x+37,y+11,5,11},color(b.cover));
            p.font(13,false,true);p.text(d.x+50,y+22,b.number(id),catalogSubtle);
            p.font(16,false,true);p.text(d.x+91,y+22,fitLabel(p,b.title,d.w-127),catalogInk);
        }
        p.line(d.x+25,d.y+d.h-49,d.x+d.w-25,d.y+d.h-49,catalogRule);
        const int available=catalogRoom_==Section::Children?childrenCount:adultCount;
        p.font(13);p.text(d.x+25,d.y+d.h-25,std::to_string(results_.size())+(isEnglish()?" of ":" de ")+std::to_string(available)+(isEnglish()?" books":" libros"),catalogSubtle);
    }
    for(const auto& b:buttons()){
        if(!reading && !b.enabled)continue;
        const bool focused=catalog_ && ((b.action==Action::Genre && catalogFocus_==1) ||
                                       (b.action==Action::Close && catalogFocus_==2));
        const bool hot=b.enabled && b.rect.has(pos_);
        button(p,b.rect,b.text,b.enabled,hot,catalog_,focused,hot && pressed_==b.action);
    }
}
}
