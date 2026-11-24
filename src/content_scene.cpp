#include "content_scene.hpp"
#include "popup.hpp"
#include <cctype>
#include <charconv>
#include <fstream>
#include <locale>
#include <map>
#include <sstream>
#include <stdexcept>
#include <variant>

namespace biblioteca {
namespace {

struct Json;
using Object=std::map<std::string,Json>;
using Array=std::vector<Json>;
struct Json {
    std::variant<std::nullptr_t,bool,double,std::string,Array,Object> value;
    const Object& object() const {return std::get<Object>(value);}
    const Array& array() const {return std::get<Array>(value);}
};

class Parser {
    std::string_view source;
    size_t at=0;
    [[noreturn]] void fail(std::string_view why) const {
        throw std::runtime_error("Invalid content scene at byte "+std::to_string(at)+": "+std::string(why));
    }
    void space(){while(at<source.size() && std::isspace(static_cast<unsigned char>(source[at])))++at;}
    bool take(char c){space();if(at<source.size() && source[at]==c){++at;return true;}return false;}
    std::string string(){
        if(!take('"'))fail("expected string");
        std::string out;
        while(at<source.size()){
            const char c=source[at++];
            if(c=='"')return out;
            if(c=='\\'){
                if(at==source.size())fail("unfinished escape");
                const char e=source[at++];
                if(e=='"'||e=='\\'||e=='/')out+=e;
                else if(e=='n')out+='\n';else if(e=='r')out+='\r';else if(e=='t')out+='\t';
                else fail("unsupported escape");
            }else{
                if(static_cast<unsigned char>(c)<32)fail("control character");
                out+=c;
            }
        }
        fail("unfinished string");
    }
    Json number(){
        space();const size_t begin=at;
        if(at<source.size() && source[at]=='-')++at;
        while(at<source.size() && std::isdigit(static_cast<unsigned char>(source[at])))++at;
        if(at<source.size() && source[at]=='.'){
            ++at;while(at<source.size() && std::isdigit(static_cast<unsigned char>(source[at])))++at;
        }
        double value=0;
#ifdef BIBLIOTECA_IPAD
        // libc++ in Xcode 16 does not implement floating-point from_chars.
        std::istringstream input(std::string(source.substr(begin,at-begin)));
        input.imbue(std::locale::classic());
        input>>value;
        if(!input || input.peek()!=std::char_traits<char>::eof())fail("invalid number");
#else
        const auto result=std::from_chars(source.data()+begin,source.data()+at,value);
        if(result.ec!=std::errc{} || result.ptr!=source.data()+at)fail("invalid number");
#endif
        return {value};
    }
    Json value(){
        space();if(at==source.size())fail("unexpected end");
        if(source[at]=='"')return {string()};
        if(source[at]=='{'){
            ++at;Object out;if(take('}'))return {out};
            do{const auto key=string();if(!take(':'))fail("expected colon");out.emplace(key,value());}while(take(','));
            if(!take('}'))fail("expected object end");
            return {out};
        }
        if(source[at]=='['){
            ++at;Array out;if(take(']'))return {out};
            do{out.push_back(value());}while(take(','));
            if(!take(']'))fail("expected array end");
            return {out};
        }
        for(const auto& [word,result]:std::array<std::pair<std::string_view,Json>,3>{{
            {"true",Json{true}},{"false",Json{false}},{"null",Json{nullptr}}}})
            if(source.substr(at,word.size())==word){at+=word.size();return result;}
        return number();
    }
public:
    explicit Parser(std::string_view text):source(text){}
    Json parse(){auto result=value();space();if(at!=source.size())fail("trailing data");return result;}
};

const Json* member(const Object& object,std::string_view key){
    const auto found=object.find(std::string(key));return found==object.end()?nullptr:&found->second;
}
std::string text(const Object& o,std::string_view key,std::string fallback={}){
    const auto* j=member(o,key);return j && std::holds_alternative<std::string>(j->value)?std::get<std::string>(j->value):fallback;
}
double number(const Object& o,std::string_view key,double fallback=0){
    const auto* j=member(o,key);return j && std::holds_alternative<double>(j->value)?std::get<double>(j->value):fallback;
}
bool boolean(const Object& o,std::string_view key,bool fallback=false){
    const auto* j=member(o,key);return j && std::holds_alternative<bool>(j->value)?std::get<bool>(j->value):fallback;
}
Vec vector(const Object& o,std::string_view key,Vec fallback={}){
    const auto* j=member(o,key);if(!j || !std::holds_alternative<Array>(j->value))return fallback;
    const auto& a=j->array();if(a.size()!=3)return fallback;
    std::array<float,3> out{};
    for(int i=0;i<3;++i){if(!std::holds_alternative<double>(a[i].value))return fallback;out[i]=float(std::get<double>(a[i].value));}
    return {out[0],out[1],out[2]};
}
ContentKind kind(std::string_view value){
    static const std::array names{std::pair{"shelf",ContentKind::Shelf},std::pair{"fire",ContentKind::Fire},
        std::pair{"lantern",ContentKind::Lantern},std::pair{"table",ContentKind::Table},
        std::pair{"chair",ContentKind::Chair},std::pair{"bench",ContentKind::Bench},
        std::pair{"book",ContentKind::Book},std::pair{"box",ContentKind::Box},
        std::pair{"spawn",ContentKind::Spawn},std::pair{"portal",ContentKind::Portal}};
    for(const auto& [name,type]:names)if(value==name)return type;
    throw std::runtime_error("Unknown content object kind: "+std::string(value));
}
Material material(std::string_view name){
    if(name=="metal")return Metal;
    if(name=="paper")return Paper;
    if(name=="leather")return Leather;
    if(name=="glass")return Frosted;
    if(name=="textile")return Textile;
    if(name=="wall")return Wall;
    if(name=="floor")return Floor;
    if(name=="light")return LightStrip;
    return Wood;
}
Vec pigment(std::string_view value){
    try{return tint(color(value));}catch(...){return tint(color("#8b7059"));}
}
Vec objectExtent(const ContentObject& item){
    Vec base{1,1,1};
    if(item.kind==ContentKind::Shelf)base={4.2f,4.3f,.85f};
    else if(item.kind==ContentKind::Table)base={3.08f,1.03f,1.52f};
    else if(item.kind==ContentKind::Chair)base={.91f,1.27f,.83f};
    else if(item.kind==ContentKind::Bench)base={4.48f,.66f,1.06f};
    else if(item.kind==ContentKind::Lantern)base={.42f,1.35f,.42f};
    else if(item.kind==ContentKind::Fire)base={2.5f,.65f,2.5f};
    else if(item.kind==ContentKind::Portal)base={1.2f,2.4f,.18f};
    return {std::abs(base.x*item.scale.x),std::abs(base.y*item.scale.y),std::abs(base.z*item.scale.z)};
}
Vec collisionExtent(const ContentObject& item){
    const auto local=objectExtent(item);const float angle=item.rotation.y*pi/180.f;
    const float c=std::abs(std::cos(angle)),s=std::abs(std::sin(angle));
    return {local.x*c+local.z*s,local.y,local.x*s+local.z*c};
}
void transformedBox(MeshData& mesh,const ContentObject& item,Vec center,Vec extent,Vec tone,Material surface,float bevel=.025f){
    MeshData local;box(local,center,extent,tone,surface,bevel);
    const auto transform=Mat::move(item.position)*Mat::ry(item.rotation.y*pi/180.f);
    for(auto& vertex:local){vertex.p=transform.point(vertex.p);vertex.n=transform.point(vertex.n,true);}
    mesh.insert(mesh.end(),local.begin(),local.end());
}
void objectMesh(MeshData& mesh,const ContentObject& item){
    if(item.kind==ContentKind::Book || item.kind==ContentKind::Spawn)return;
    const Vec size=objectExtent(item),tone=pigment(item.color);
    if(item.kind==ContentKind::Table){
        transformedBox(mesh,item,{0,size.y*.90f,0},{size.x,.12f*item.scale.y,size.z},tone,material(item.material),.04f);
        for(float x:{-.39f,.39f})for(float z:{-.35f,.35f})
            transformedBox(mesh,item,{x*size.x,size.y*.43f,z*size.z},{.12f*item.scale.x,size.y*.86f,.12f*item.scale.z},tone,Wood,.02f);
    }else if(item.kind==ContentKind::Chair){
        transformedBox(mesh,item,{0,size.y*.42f,0},{size.x,.12f*item.scale.y,size.z},tone,Textile,.03f);
        transformedBox(mesh,item,{0,size.y*.72f,size.z*.42f},{size.x,.58f*item.scale.y,.12f*item.scale.z},tone,Wood,.02f);
    }else if(item.kind==ContentKind::Lantern){
        transformedBox(mesh,item,{0,size.y*.5f,0},size,tone,LightStrip,.08f);
    }else if(item.kind==ContentKind::Fire){
        transformedBox(mesh,item,{0,size.y*.20f,0},{size.x,.24f*item.scale.y,size.z},tone,Ember,.10f);
    }else transformedBox(mesh,item,{0,size.y*.5f,0},size,tone,material(item.material));
}

} // namespace

ContentScene loadContentScene(const std::filesystem::path& path){
    std::ifstream input(path,std::ios::binary);
    if(!input)return {};
    const std::string source((std::istreambuf_iterator<char>(input)),{});
    if(source.size()>4*1024*1024)throw std::runtime_error("Content scene exceeds 4 MiB");
    const auto root=Parser(source).parse().object();
    ContentScene scene;scene.schema=int(number(root,"schema",0));
    if(scene.schema!=2)throw std::runtime_error("Unsupported content scene schema "+std::to_string(scene.schema));
    if(const auto* rooms=member(root,"rooms")){for(const auto& value:rooms->array()){
        const auto& item=value.object();
        ContentRoom room;
        room.id=text(item,"id");room.name=text(item,"name",room.id);room.origin=vector(item,"origin");
        room.size=vector(item,"size",{10,0,10});room.height=float(number(item,"height",5));room.builtin=boolean(item,"builtin");
        room.floor=text(item,"floor","wood");room.walls=text(item,"walls","plaster");room.ceiling=text(item,"ceiling","plaster");
        if(room.id.empty() || room.size.x<=1 || room.size.z<=1 || room.height<2.4f)throw std::runtime_error("Invalid content room");
        scene.rooms.push_back(std::move(room));
    }}
    if(const auto* objects=member(root,"objects")){for(const auto& value:objects->array()){
        const auto& item=value.object();ContentObject object;
        object.id=text(item,"id");object.name=text(item,"name",object.id);object.kind=kind(text(item,"kind","box"));
        object.position=vector(item,"position");object.rotation=vector(item,"rotation");object.scale=vector(item,"scale",{1,1,1});
        object.color=text(item,"color","#8b7059");object.material=text(item,"material","wood");
        object.solid=boolean(item,"solid",object.kind!=ContentKind::Book && object.kind!=ContentKind::Spawn);
        object.animated=boolean(item,"animated");object.interactive=boolean(item,"interactive");
        object.book=int(number(item,"book",-1));
        if(object.id.empty() || object.scale.x<=0 || object.scale.y<=0 || object.scale.z<=0)throw std::runtime_error("Invalid content object");
        if(object.kind==ContentKind::Book && (object.book<0 || object.book>=bookCount))throw std::runtime_error("Invalid content book id");
        scene.objects.push_back(std::move(object));
    }}
    return scene;
}

const ContentScene& contentScene(){
    static const auto scene=[](){
        try{
            const auto overlay=assetsPath()/"content"/"scene.json";
            if(std::filesystem::is_regular_file(overlay))return loadContentScene(overlay);
            return loadContentScene(assetsPath()/"scene.json");
        }
        catch(const std::exception& error){ContentScene empty;empty.warnings.push_back(error.what());return empty;}
    }();
    return scene;
}

MeshData buildContentSceneMesh(const ContentScene& scene){
    MeshData mesh;
    for(const auto& room:scene.rooms)if(!room.builtin){
        const float floorY=room.origin.y,ceiling=floorY+room.height;
        box(mesh,{room.origin.x,floorY-.06f,room.origin.z},{room.size.x,.12f,room.size.z},pigment("#8b7059"),Floor,.01f);
        box(mesh,{room.origin.x,ceiling+.06f,room.origin.z},{room.size.x,.12f,room.size.z},pigment("#c9d2bc"),Wall,.01f);
        for(float side:{-1.f,1.f}){
            box(mesh,{room.origin.x+side*room.size.x*.5f,floorY+room.height*.5f,room.origin.z},
                {.16f,room.height,room.size.z},pigment("#c9d2bc"),Wall,.01f);
            box(mesh,{room.origin.x,floorY+room.height*.5f,room.origin.z+side*room.size.z*.5f},
                {room.size.x,room.height,.16f},pigment("#c9d2bc"),Wall,.01f);
        }
    }
    for(const auto& item:scene.objects)objectMesh(mesh,item);
    return mesh;
}

std::vector<Bounds> buildContentSceneColliders(const ContentScene& scene){
    std::vector<Bounds> boxes;
    for(const auto& item:scene.objects)if(item.solid && item.kind!=ContentKind::Book && item.kind!=ContentKind::Spawn){
            const auto extent=collisionExtent(item);
            boxes.push_back(Bounds::box(item.position+Vec{0,extent.y*.5f,0},extent));
    }
    return boxes;
}

bool contentSceneContains(const ContentScene& scene,Vec point){
    for(const auto& room:scene.rooms){
        if(room.builtin)continue;
        if(std::abs(point.x-room.origin.x)<=room.size.x*.5f-personRadius &&
           std::abs(point.z-room.origin.z)<=room.size.z*.5f-personRadius)return true;
    }
    return false;
}

MeshData contentSceneMesh(){return buildContentSceneMesh(contentScene());}

const std::vector<Bounds>& contentSceneColliders(){
    static const auto result=buildContentSceneColliders(contentScene());
    return result;
}

bool contentSceneContains(Vec point){return contentSceneContains(contentScene(),point);}

}
