#include "locale.hpp"
#include "render.hpp"
#include "ui.hpp"
#include <SDL3/SDL_main.h>
#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>

namespace {
using namespace biblioteca;

void reportSmoke(std::string_view result){
    if(char* path=SDL_GetPrefPath("StarryLibrary","Smoke")){
        std::ofstream(std::filesystem::path(path)/"starry-ipad-smoke.txt")<<result<<'\n';
        SDL_free(path);
    }
}

struct App {
    SDL_Window* window;
    Size logical;
    Interface ui;
    Renderer renderer;
    std::string hud;
    Uint64 last=SDL_GetTicksNS();
    float sceneScale=.75f,frameAverage=1.f/30.f;
    int slowFrames=0,fastFrames=0;
    bool suspended=false;
    int smokeFrame;
    Vec smokeStart;

    App(SDL_Window* window,Size size,bool smoke):
        window(window),logical(size),ui(size),smokeFrame(smoke?0:-1),smokeStart(ui.lib.eye()){}

    void finger(SDL_EventType type,Point p){
        SDL_Event event{};
        event.type=type;
        event.tfinger.fingerID=1;
        event.tfinger.x=p.x/logical.w;
        event.tfinger.y=p.y/logical.h;
        ui.event(event,window);
    }

    void testInput(){
        const float tile=std::clamp(std::min(logical.h*.15f,logical.w*.115f),42.f,112.f);
        const Point forward{32+tile*1.5f,logical.h-36-tile*2.5f};
        if(smokeFrame==0)finger(sdl::fingerDown,forward);
        if(smokeFrame==12){
            finger(sdl::fingerUp,forward);
            const auto delta=ui.lib.eye()-smokeStart;
            if(dot(delta,delta)<.05f)throw std::runtime_error("Touch D-pad did not move the camera");
            if(!ui.lib.visit(0))throw std::runtime_error("Could not open the test book");
        }
        if(smokeFrame>12 && ui.lib.stage()==Stage::Reading && ui.lib.target()==0){
            const Point start{logical.w*.73f,logical.h*.76f},end{logical.w*.48f,logical.h*.76f};
            finger(sdl::fingerDown,start);
            finger(sdl::fingerMotion,end);
            finger(sdl::fingerUp,end);
            if(ui.lib.target()!=1)throw std::runtime_error("Touch swipe did not turn a page");
        }
        if(smokeFrame>420)throw std::runtime_error("Test book did not reach the second spread");
    }

    void draw(){
        SDL_Event event{};
        while(SDL_PollEvent(&event)){
            if(event.type==SDL_EVENT_WILL_ENTER_BACKGROUND)suspended=true;
            if(event.type==SDL_EVENT_DID_ENTER_FOREGROUND){suspended=false;last=SDL_GetTicksNS();}
            ui.event(event,window);
        }
        if(ui.quit || suspended)return;
        const Uint64 now=SDL_GetTicksNS();
        const float frameTime=std::min(.1f,float(now-last)/1e9f);
        last=now;

        Size size{},pixels{};
        if(!SDL_GetWindowSize(window,&size.w,&size.h) || size.w<=0 || size.h<=0 ||
           !SDL_GetWindowSizeInPixels(window,&pixels.w,&pixels.h) || pixels.w<=0 || pixels.h<=0)
            return;
        if(size.w!=logical.w || size.h!=logical.h){logical=size;ui.resize(size);hud.clear();}
        if(smokeFrame>=0)testInput();
        const float dt=smokeFrame>=0?.04f:std::min(.09f,frameTime);
        const float sceneTime=ui.sceneTime(dt);
        ui.step(dt);

        if(ui.dirty || ui.walking() || ui.lib.animating()){
            frameAverage=.9f*frameAverage+.1f*frameTime;
            const bool slow=frameAverage>.039f,fast=frameAverage<.026f;
            slowFrames=slow?slowFrames+1:0;
            fastFrames=fast?fastFrames+1:0;
            if(slowFrames>=12){sceneScale=std::max(.55f,sceneScale-.10f);slowFrames=0;}
            if(fastFrames>=90){sceneScale=std::min(.85f,sceneScale+.05f);fastFrames=0;}
        }else{slowFrames=fastFrames=0;frameAverage=1.f/30.f;}

        renderer.draw(ui.lib,pixels,sceneTime,sceneScale);
        const auto signature=ui.signature();
        if(hud!=signature){
            Paint image(logical);
            ui.paint(image);
            renderer.hud(&image);
            hud=signature;
        }else renderer.hud(nullptr);

        if(smokeFrame>=0 && ui.lib.stage()==Stage::Reading && ui.lib.spread()==1){
            std::array<unsigned char,4> a{},b{};
            glReadPixels(pixels.w/2,pixels.h/8,1,1,GL_RGBA,GL_UNSIGNED_BYTE,a.data());
            glReadPixels(pixels.w/4,pixels.h*3/4,1,1,GL_RGBA,GL_UNSIGNED_BYTE,b.data());
            if(glGetError()!=GL_NO_ERROR || a==b || renderer.stats().triangles==0)
                throw std::runtime_error("Reading scene did not render into the display framebuffer");
            if(!SDL_GL_SwapWindow(window))throw std::runtime_error(SDL_GetError());
            reportSmoke("OK: scene rendered, D-pad moved, book opened and swipe turned a page");
            smokeFrame=-1;
        }else{
            if(!SDL_GL_SwapWindow(window))throw std::runtime_error(SDL_GetError());
            if(smokeFrame>=0)++smokeFrame;
        }
        ui.dirty=false;
    }
};



void frame(void* state){
    auto& app=*static_cast<App*>(state);
    try{
        app.draw();
        if(app.ui.quit)SDL_SetiOSAnimationCallback(app.window,1,nullptr,nullptr);
    }catch(const std::exception& error){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"%s",error.what());
        if(app.smokeFrame>=0)reportSmoke(error.what());
        SDL_SetiOSAnimationCallback(app.window,1,nullptr,nullptr);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Biblioteca de Estrellas",error.what(),app.window);
    }
}
}

int main(int argc,char** argv){
    const bool smoke=argc>1 && std::string_view(argv[1])=="--ipad-smoke-test";
    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS,"0");
    SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS,"0");
    SDL_SetHint(SDL_HINT_ORIENTATIONS,"LandscapeLeft LandscapeRight");
    SDL_SetAppMetadata("Starry Library","1.1.0","org.cuentosentreestrellas.biblioteca");
    try{
        if(!SDL_Init(SDL_INIT_VIDEO))throw std::runtime_error(SDL_GetError());
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_ES);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,0);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,8);
        SDL_GL_SetAttribute(SDL_GL_RETAINED_BACKING,0);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,0);
        const auto flags=SDL_WINDOW_OPENGL|SDL_WINDOW_FULLSCREEN|SDL_WINDOW_HIGH_PIXEL_DENSITY;
        auto* window=SDL_CreateWindow("Biblioteca de Estrellas",1024,768,flags);
        if(!window || !SDL_GL_CreateContext(window))throw std::runtime_error(SDL_GetError());
        SDL_StopTextInput(window);
        Size size{};
        if(!SDL_GetWindowSize(window,&size.w,&size.h))throw std::runtime_error(SDL_GetError());
        setEnglish(false);
        auto app=std::make_unique<App>(window,size,smoke);
        // Keep SDL_main alive and let SDL_PollEvent pump UIKit. 
        SDL_GL_SetSwapInterval(1);
        while(!app->ui.quit){
            app->draw();
            SDL_Delay(1);
        }
        return 0;
    }catch(const std::exception& error){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"%s",error.what());
        if(smoke)reportSmoke(error.what());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Biblioteca de Estrellas",error.what(),nullptr);
        return 1;
    }
}
