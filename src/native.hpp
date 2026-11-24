#pragma once

// Stable Linux SDL2, OpenGL and Cairo C ABI used by the desktop runtime.
// Only the symbols consumed by the application are declared so the build can link to runtime SONAMEs.
#include <cstddef>
#include <cstdint>

#if defined(__ANDROID__) || defined(BIBLIOTECA_IPAD)
#define BIBLIOTECA_MOBILE 1
#endif

#ifdef BIBLIOTECA_IPAD
#include <OpenGLES/ES3/gl.h>
#include <SDL3/SDL.h>
#include <cairo.h>
#elif defined(__ANDROID__)
#include <GLES3/gl3.h>
#include <SDL.h>
#include <cairo.h>
#elif defined(_WIN32)
#include <GL/glew.h>
#include <SDL.h>
#include <cairo.h>
#else
extern "C" {
struct SDL_Window;
using SDL_GLContext=void*;
struct SDL_Keysym {int scancode,sym;std::uint16_t mod;std::uint32_t unused;};
struct SDL_KeyboardEvent {std::uint32_t type,timestamp,window;std::uint8_t state,repeat,pad1,pad2;SDL_Keysym keysym;};
struct SDL_WindowEvent {std::uint32_t type,timestamp,window;std::uint8_t event,pad1,pad2,pad3;int data1,data2;};
struct SDL_MouseMotionEvent {std::uint32_t type,timestamp,window,which,state;int x,y,xrel,yrel;};
struct SDL_MouseButtonEvent {std::uint32_t type,timestamp,window,which;std::uint8_t button,state,clicks,pad;int x,y;};
struct SDL_MouseWheelEvent {std::uint32_t type,timestamp,window,which;int x,y;std::uint32_t direction;float preciseX,preciseY;int mouseX,mouseY;};
struct SDL_TextInputEvent {std::uint32_t type,timestamp,window;char text[32];};
union SDL_Event {
    std::uint32_t type;
    SDL_KeyboardEvent key;
    SDL_WindowEvent window;
    SDL_MouseMotionEvent motion;
    SDL_MouseButtonEvent button;
    SDL_MouseWheelEvent wheel;
    SDL_TextInputEvent text;
    alignas(void*) std::uint8_t padding[56];
};
static_assert(sizeof(SDL_Event)==56);
int SDL_Init(std::uint32_t flags);
void SDL_Quit();
const char* SDL_GetError();
int SDL_GL_SetAttribute(int attr,int value);
SDL_Window* SDL_CreateWindow(const char*,int,int,int,int,std::uint32_t);
void SDL_DestroyWindow(SDL_Window*);
void SDL_SetWindowMinimumSize(SDL_Window*,int,int);
void SDL_GetWindowSize(SDL_Window*,int*,int*);
int SDL_SetWindowFullscreen(SDL_Window*,std::uint32_t);
int SDL_SetRelativeMouseMode(int);
SDL_GLContext SDL_GL_CreateContext(SDL_Window*);
void SDL_GL_DeleteContext(SDL_GLContext);
void SDL_GL_GetDrawableSize(SDL_Window*,int*,int*);
void SDL_GL_SwapWindow(SDL_Window*);
int SDL_GL_SetSwapInterval(int);
int SDL_PollEvent(SDL_Event*);
int SDL_WaitEventTimeout(SDL_Event*,int);
int SDL_PushEvent(SDL_Event*);
void SDL_StartTextInput();
void SDL_StopTextInput();
int SDL_OpenURL(const char*);
struct SDL_Cursor;
SDL_Cursor* SDL_CreateSystemCursor(int);
void SDL_SetCursor(SDL_Cursor*);
void SDL_FreeCursor(SDL_Cursor*);

using GLuint=unsigned int;
using GLenum=unsigned int;
using GLint=int;
using GLsizei=int;
using GLboolean=unsigned char;
using GLfloat=float;
using GLsizeiptr=std::ptrdiff_t;
const unsigned char* glGetString(GLenum);
GLenum glGetError();
void glGetIntegerv(GLenum,GLint*);
void glGenBuffers(GLsizei,GLuint*); void glBindBuffer(GLenum,GLuint);
void glBufferData(GLenum,GLsizeiptr,const void*,GLenum);
void glDeleteBuffers(GLsizei,const GLuint*);
void glGenVertexArrays(GLsizei,GLuint*); void glBindVertexArray(GLuint);
void glDeleteVertexArrays(GLsizei,const GLuint*);
void glEnableVertexAttribArray(GLuint);
void glVertexAttribPointer(GLuint,GLint,GLenum,GLboolean,GLsizei,const void*);
GLuint glCreateShader(GLenum); void glShaderSource(GLuint,GLsizei,const char* const*,const GLint*);
void glCompileShader(GLuint); void glGetShaderiv(GLuint,GLenum,GLint*);
void glGetShaderInfoLog(GLuint,GLsizei,GLsizei*,char*); void glDeleteShader(GLuint);
GLuint glCreateProgram(); void glAttachShader(GLuint,GLuint); void glLinkProgram(GLuint);
void glGetProgramiv(GLuint,GLenum,GLint*); void glGetProgramInfoLog(GLuint,GLsizei,GLsizei*,char*);
void glUseProgram(GLuint); void glDeleteProgram(GLuint);
GLint glGetUniformLocation(GLuint,const char*);
void glUniform1i(GLint,GLint); void glUniform1f(GLint,GLfloat);
void glUniform3f(GLint,GLfloat,GLfloat,GLfloat);
void glUniform4fv(GLint,GLsizei,const GLfloat*);
void glUniformMatrix4fv(GLint,GLsizei,GLboolean,const GLfloat*);
void glDrawArrays(GLenum,GLint,GLsizei);
void glGenTextures(GLsizei,GLuint*); void glBindTexture(GLenum,GLuint);
void glDeleteTextures(GLsizei,const GLuint*); void glTexParameteri(GLenum,GLenum,GLint);
void glActiveTexture(GLenum);
void glTexImage2D(GLenum,GLint,GLint,GLsizei,GLsizei,GLint,GLenum,GLenum,const void*);
void glTexSubImage2D(GLenum,GLint,GLint,GLint,GLsizei,GLsizei,GLenum,GLenum,const void*);
void glGenerateMipmap(GLenum); void glPixelStorei(GLenum,GLint);
void glViewport(GLint,GLint,GLsizei,GLsizei); void glClearColor(GLfloat,GLfloat,GLfloat,GLfloat);
void glClear(GLenum); void glEnable(GLenum); void glDisable(GLenum); void glDepthFunc(GLenum);
void glDepthMask(GLboolean); void glDepthRange(double,double);
void glBlendFunc(GLenum,GLenum); void glReadPixels(GLint,GLint,GLsizei,GLsizei,GLenum,GLenum,void*);
void glFinish();
void glGenFramebuffers(GLsizei,GLuint*); void glBindFramebuffer(GLenum,GLuint);
void glDeleteFramebuffers(GLsizei,const GLuint*);
void glFramebufferTexture2D(GLenum,GLenum,GLenum,GLuint,GLint);
GLenum glCheckFramebufferStatus(GLenum);


struct cairo_surface_t;
struct cairo_t;
struct cairo_pattern_t;
struct cairo_text_extents_t {double x_bearing,y_bearing,width,height,x_advance,y_advance;};
struct cairo_font_extents_t {double ascent,descent,height,max_x_advance,max_y_advance;};
cairo_surface_t* cairo_image_surface_create(int,int,int);
cairo_surface_t* cairo_image_surface_create_for_data(unsigned char*,int,int,int,int);
cairo_surface_t* cairo_image_surface_create_from_png(const char*);
int cairo_surface_status(cairo_surface_t*);
int cairo_image_surface_get_width(cairo_surface_t*);
int cairo_image_surface_get_height(cairo_surface_t*);
void cairo_set_source_surface(cairo_t*,cairo_surface_t*,double,double);
void cairo_paint(cairo_t*);
unsigned char* cairo_image_surface_get_data(cairo_surface_t*);
void cairo_surface_flush(cairo_surface_t*); void cairo_surface_destroy(cairo_surface_t*);
int cairo_surface_write_to_png(cairo_surface_t*,const char*);
cairo_t* cairo_create(cairo_surface_t*); void cairo_destroy(cairo_t*);
void cairo_set_source_rgba(cairo_t*,double,double,double,double); void cairo_set_source(cairo_t*,cairo_pattern_t*);
void cairo_rectangle(cairo_t*,double,double,double,double); void cairo_fill(cairo_t*); void cairo_fill_preserve(cairo_t*);
void cairo_move_to(cairo_t*,double,double); void cairo_line_to(cairo_t*,double,double); void cairo_close_path(cairo_t*);
void cairo_new_path(cairo_t*); void cairo_clip(cairo_t*); void cairo_arc(cairo_t*,double,double,double,double,double);
void cairo_stroke(cairo_t*); void cairo_set_line_width(cairo_t*,double);
void cairo_save(cairo_t*); void cairo_restore(cairo_t*); void cairo_rotate(cairo_t*,double); void cairo_translate(cairo_t*,double,double);
void cairo_scale(cairo_t*,double,double);
void cairo_select_font_face(cairo_t*,const char*,int,int); void cairo_set_font_size(cairo_t*,double);
struct cairo_matrix_t {double xx,yx,xy,yy,x0,y0;};
void cairo_set_font_matrix(cairo_t*,const cairo_matrix_t*);
void cairo_text_extents(cairo_t*,const char*,cairo_text_extents_t*);
void cairo_font_extents(cairo_t*,cairo_font_extents_t*); void cairo_show_text(cairo_t*,const char*);
cairo_pattern_t* cairo_pattern_create_linear(double,double,double,double);
cairo_pattern_t* cairo_pattern_create_radial(double,double,double,double,double,double);
void cairo_pattern_add_color_stop_rgba(cairo_pattern_t*,double,double,double,double,double);
void cairo_pattern_destroy(cairo_pattern_t*);
}
#endif

namespace gl {
constexpr GLenum vertex=0x8b31,fragment=0x8b30,compile=0x8b81,link=0x8b82;
constexpr GLenum array=0x8892,staticDraw=0x88e4,streamDraw=0x88e0,fp=0x1406,triangles=4;
constexpr GLenum texture=0x0de1,texture0=0x84c0,rgba8=0x8058,rgb8=0x8051,rgb=0x1907,bgra=0x80e1,byte=0x1401;
constexpr GLenum wrapS=0x2802,wrapT=0x2803,clampEdge=0x812f,repeat=0x2901,minFilter=0x2801,magFilter=0x2800;
constexpr GLenum framebuffer=0x8d40,colorAttachment=0x8ce0,complete=0x8cd5;
constexpr GLenum framebufferBinding=0x8ca6,rgba=0x1908;
#ifdef BIBLIOTECA_MOBILE
constexpr GLenum paintFormat=rgba;
inline void depthRange(float low,float high){glDepthRangef(low,high);}
#else
constexpr GLenum paintFormat=bgra;
inline void depthRange(float low,float high){glDepthRange(low,high);}
#endif
constexpr GLenum linear=0x2601,mipLinear=0x2703,unpack=0x0cf5;
constexpr GLenum cullFace=0x0b44;
constexpr GLenum blend=0x0be2,depth=0x0b71,lequal=0x0203,one=1,srcAlpha=0x0302,oneMinusAlpha=0x0303;
constexpr GLenum colorBit=0x4000,depthBit=0x0100,renderer=0x1f01,multisample=0x809d;
}

namespace sdl {
#ifdef BIBLIOTECA_IPAD
constexpr auto quit=SDL_EVENT_QUIT,keyDown=SDL_EVENT_KEY_DOWN,keyUp=SDL_EVENT_KEY_UP;
constexpr auto text=SDL_EVENT_TEXT_INPUT,motion=SDL_EVENT_MOUSE_MOTION;
constexpr auto down=SDL_EVENT_MOUSE_BUTTON_DOWN,up=SDL_EVENT_MOUSE_BUTTON_UP,wheel=SDL_EVENT_MOUSE_WHEEL;
constexpr auto fingerDown=SDL_EVENT_FINGER_DOWN,fingerMotion=SDL_EVENT_FINGER_MOTION,fingerUp=SDL_EVENT_FINGER_UP;
inline SDL_FingerID fingerId(const SDL_TouchFingerEvent& event){return event.fingerID;}
constexpr auto right=SDLK_RIGHT,left=SDLK_LEFT,downKey=SDLK_DOWN,upKey=SDLK_UP,f2=SDLK_F2,f11=SDLK_F11;
constexpr auto leftShift=SDLK_LSHIFT,rightShift=SDLK_RSHIFT;
constexpr int arrowCursor=SDL_SYSTEM_CURSOR_DEFAULT,grabCursor=SDL_SYSTEM_CURSOR_MOVE,handCursor=SDL_SYSTEM_CURSOR_POINTER;
inline int keyCode(const SDL_KeyboardEvent& key){return int(key.key);}
inline bool shiftHeld(const SDL_KeyboardEvent& key){return (key.mod&SDL_KMOD_SHIFT)!=0;}
inline float verticalWheel(const SDL_MouseWheelEvent& wheel){return wheel.y;}
inline void textInput(bool enabled){
    if(auto* window=SDL_GetKeyboardFocus()){
        if(enabled)SDL_StartTextInput(window);else SDL_StopTextInput(window);
    }
}
inline void fullscreen(SDL_Window* window,bool enabled){SDL_SetWindowFullscreen(window,enabled);}
inline int relativeMouse(bool enabled){
    auto* window=SDL_GetKeyboardFocus();
    return window && SDL_SetWindowRelativeMouseMode(window,enabled)?0:-1;
}
#else
inline int relativeMouse(bool enabled){
#if defined(_WIN32) || defined(__ANDROID__)
    return SDL_SetRelativeMouseMode(enabled?SDL_TRUE:SDL_FALSE);
#else
    return SDL_SetRelativeMouseMode(enabled?1:0);
#endif
}
constexpr unsigned video=0x20,openGL=0x2,hidden=0x8,resizable=0x20,highDpi=0x2000,fullDesktop=0x1001;
constexpr int centered=0x2fff0000;
#if defined(_WIN32) || defined(__ANDROID__)
constexpr SDL_GLattr doubleBuffer=SDL_GL_DOUBLEBUFFER,depthSize=SDL_GL_DEPTH_SIZE,
    msBuffers=SDL_GL_MULTISAMPLEBUFFERS,msSamples=SDL_GL_MULTISAMPLESAMPLES,
    major=SDL_GL_CONTEXT_MAJOR_VERSION,minor=SDL_GL_CONTEXT_MINOR_VERSION,
    profile=SDL_GL_CONTEXT_PROFILE_MASK;
#else
constexpr int doubleBuffer=5,depthSize=6,msBuffers=13,msSamples=14,major=17,minor=18,profile=21;
#endif
constexpr unsigned quit=0x100,window=0x200,keyDown=0x300,keyUp=0x301,text=0x303,motion=0x400,down=0x401,up=0x402,wheel=0x403;
constexpr int right=0x4000004f,left=0x40000050,downKey=0x40000051,upKey=0x40000052,f2=0x4000003b,f11=0x40000044;
constexpr int leftShift=0x400000e1,rightShift=0x400000e5;
inline int keyCode(const SDL_KeyboardEvent& key){return key.keysym.sym;}
inline bool shiftHeld(const SDL_KeyboardEvent& key){return (key.keysym.mod&3)!=0;}
inline float verticalWheel(const SDL_MouseWheelEvent& wheel){return wheel.preciseY!=0?wheel.preciseY:float(wheel.y);}
inline void textInput(bool enabled){if(enabled)SDL_StartTextInput();else SDL_StopTextInput();}
inline void fullscreen(SDL_Window* window,bool enabled){SDL_SetWindowFullscreen(window,enabled?fullDesktop:0);}
#ifdef __ANDROID__
constexpr auto fingerDown=SDL_FINGERDOWN,fingerMotion=SDL_FINGERMOTION,fingerUp=SDL_FINGERUP;
inline SDL_FingerID fingerId(const SDL_TouchFingerEvent& event){return event.fingerId;}
#endif
#if defined(_WIN32) || defined(__ANDROID__)
constexpr SDL_SystemCursor arrowCursor=SDL_SYSTEM_CURSOR_ARROW,
    grabCursor=SDL_SYSTEM_CURSOR_SIZEALL,handCursor=SDL_SYSTEM_CURSOR_HAND;
#else
constexpr int arrowCursor=0,grabCursor=9,handCursor=11;
#endif
#endif
}
