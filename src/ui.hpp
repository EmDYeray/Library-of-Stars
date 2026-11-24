#pragma once
#include "paint.hpp"
#include <string>
#include <vector>

namespace biblioteca {
class Interface {
    enum class Action { None, Catalog, Left, Right, Close, Genre, Section, Motion, Sources, Language,
                        Forward, Back, StrafeLeft, StrafeRight, ZoomOut, ZoomIn };
    struct Button { Rect rect; Action action; std::string text; bool enabled; };
    Size size_;
    Point pos_{-100,-100},start_{},last_{};
    bool drag_=false,moved_=false,full_=false,catalog_=false,hideControls_=false;
    bool lookDrag_=false,mouseLook_=false;
    bool catalogDrag_=false,touchSwipe_=false;
    bool motion_=true;
    std::array<bool,4> motionKeys_{};
    std::array<bool,2> sprintKeys_{};
    Vec stride_{};
    float wheelTravel_=0;
    float sceneSeconds_=0;
    Action pressed_=Action::None;
    Action touchDrive_=Action::None;
#ifdef BIBLIOTECA_MOBILE
    SDL_FingerID touchDriveFinger_=-1,touchLookFinger_=-1;
    Point touchLookLast_{};
    bool touchLookMoved_=false;
#endif
#ifdef BIBLIOTECA_IPAD
    SDL_FingerID touchPointer_=-1;
    void fingerPointer(SDL_EventType type,Point point,SDL_Window* window);
#endif
    int cursor_=0,filter_=0,scroll_=0;
    int catalogFocus_=0;
    std::string search_;
    std::vector<int> results_;
    std::vector<std::string> genres_{"All genres"};
    Section catalogRoom_=Section::Children;
    std::vector<Button> buttons() const;
    Action actionAt(Point p,bool includeDisabled=false) const;
    Rect dialog() const;
    const char* sourceAt(Point p) const;
    std::optional<int> itemAt(Point p) const;
    void refreshCatalog();
    void syncCatalog();
    void stopWalking();
    void showCatalog(bool show);
    void openLectern();
    void focusCatalog(int field);
    void choose(int index);
    void act(Action a);
    bool touchDriveAction(Action a) const;
    void setTouchDrive(Action a,bool down);
    void cancelInput();
public:
    Library lib;
    bool quit=false,dirty=true;
    explicit Interface(Size size);
    void resize(Size size);
    bool walking() const;
    void step(float dt);
    void event(const SDL_Event& event,SDL_Window* window);
    int pointerKind() const;
    std::string signature() const;
    void paint(Paint& target) const;
    bool catalogVisible() const {return catalog_;}
    bool motionEnabled() const {return motion_;}
    float sceneTime(float dt);
    int resultCount() const {return int(results_.size());}
};
}
