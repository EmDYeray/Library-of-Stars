#include "render.hpp"
#include "geometry.hpp"
#include "hearth.hpp"
#include "room_layout.hpp"
#include "printing.hpp"
#include "popup.hpp"
#include <fstream>
#include <sstream>
#include "native.hpp"
#include "paint.hpp"
#include "locale.hpp"
#include "content_scene.hpp"
#include <array>
#include <stdexcept>

namespace biblioteca {
namespace {
GLuint shader(GLenum type,const char* src){
    const GLuint sh=glCreateShader(type);
    glShaderSource(sh,1,&src,nullptr);glCompileShader(sh);
    GLint ok=0;glGetShaderiv(sh,gl::compile,&ok);
    if(!ok){char log[4096]{};glGetShaderInfoLog(sh,sizeof(log),nullptr,log);glDeleteShader(sh);throw std::runtime_error(log);}
    return sh;
}
std::string readText(const std::filesystem::path& path){
    std::ifstream input(path);
    if(!input)throw std::runtime_error("No se pudo abrir "+path.string());
    std::ostringstream output;output<<input.rdbuf();return output.str();
}
GLuint createProgram(){
    const auto dir=assetsPath()/"shaders";
    auto vertex=readText(dir/"scene.vert"),fragment=readText(dir/"scene.frag");
#ifdef BIBLIOTECA_MOBILE
    const auto forEs=[](std::string& source){
        constexpr std::string_view desktop="#version 330 core";
        if(!source.starts_with(desktop))throw std::runtime_error("Unsupported shader version");
        source.replace(0,desktop.size(),"#version 300 es\nprecision highp float;\nprecision highp int;");
    };
    forEs(vertex);forEs(fragment);
    fragment.insert(fragment.find('\n')+1,"#define BIBLIOTECA_MOBILE 1\n");
#endif
    const auto vs=shader(gl::vertex,vertex.c_str()),fs=shader(gl::fragment,fragment.c_str());
    const GLuint p=glCreateProgram();
    glAttachShader(p,vs);glAttachShader(p,fs);glLinkProgram(p);glDeleteShader(vs);glDeleteShader(fs);
    GLint ok=0;glGetProgramiv(p,gl::link,&ok);
    if(!ok){char log[4096]{};glGetProgramInfoLog(p,sizeof(log),nullptr,log);glDeleteProgram(p);throw std::runtime_error(log);}
    return p;
}

GLuint loadMaterialGrain(){
    const auto path=assetsPath()/"materials"/"grain.ppm";
    std::ifstream input(path,std::ios::binary);
    std::string format;
    int width=0,height=0,range=0;
    if(!(input>>format>>width>>height>>range) || format!="P6" ||
       width<1 || width>2048 || height<1 || height>2048 || range!=255 || input.get()!='\n')
        throw std::runtime_error("Invalid material texture: "+path.string());
    std::vector<unsigned char> pixels(size_t(width)*height*3);
    if(!input.read(reinterpret_cast<char*>(pixels.data()),pixels.size()))
        throw std::runtime_error("Incomplete material texture: "+path.string());
    GLuint id=0;
    glGenTextures(1,&id);glActiveTexture(gl::texture0+3);glBindTexture(gl::texture,id);
    glTexParameteri(gl::texture,gl::wrapS,gl::repeat);glTexParameteri(gl::texture,gl::wrapT,gl::repeat);
    glTexParameteri(gl::texture,gl::minFilter,gl::mipLinear);glTexParameteri(gl::texture,gl::magFilter,gl::linear);
    glPixelStorei(gl::unpack,1);
    glTexImage2D(gl::texture,0,gl::rgb8,width,height,0,gl::rgb,gl::byte,pixels.data());
    glPixelStorei(gl::unpack,4);
    glGenerateMipmap(gl::texture);
    glActiveTexture(gl::texture0);
    return id;
}

struct Mesh {
    GLuint vao=0,vbo=0;
    GLsizei count=0;
    void set(const MeshData& data,GLenum usage=gl::staticDraw){
        if(!vao){
            glGenVertexArrays(1,&vao);glBindVertexArray(vao);
            glGenBuffers(1,&vbo);glBindBuffer(gl::array,vbo);
            auto add=[](GLuint index,int size,size_t pos){
                glEnableVertexAttribArray(index);
                glVertexAttribPointer(index,size,gl::fp,0,sizeof(Vertex),reinterpret_cast<const void*>(pos));
            };
            add(0,3,offsetof(Vertex,p));add(1,3,offsetof(Vertex,n));add(2,3,offsetof(Vertex,c));
            add(3,2,offsetof(Vertex,u));add(4,1,offsetof(Vertex,mat));
        }
        glBindBuffer(gl::array,vbo);
        glBufferData(gl::array,data.size()*sizeof(Vertex),data.data(),usage);
        count=GLsizei(data.size());
    }
    ~Mesh(){glDeleteBuffers(1,&vbo);glDeleteVertexArrays(1,&vao);}
};

Bounds meshBounds(const MeshData& mesh){
    Bounds box{mesh.front().p,mesh.front().p};
    for(const auto& v:mesh){
        box.min={std::min(box.min.x,v.p.x),std::min(box.min.y,v.p.y),std::min(box.min.z,v.p.z)};
        box.max={std::max(box.max.x,v.p.x),std::max(box.max.y,v.p.y),std::max(box.max.z,v.p.z)};
    }
    return box;
}
bool inView(const Mat& view,Bounds box){
    unsigned outside=63;
    const auto& m=view.a;
    for(float x:{box.min.x,box.max.x})for(float y:{box.min.y,box.max.y})for(float z:{box.min.z,box.max.z}){
        const float w=m[3]*x+m[7]*y+m[11]*z+m[15];
        unsigned side=0;
        for(int i=0;i<3;++i){
            const float q=m[i]*x+m[i+4]*y+m[i+8]*z+m[i+12];
            if(q<-w)side|=1u<<(i*2);
            if(q>w)side|=2u<<(i*2);
        }
        outside&=side;
        if(!outside)return true;
    }
    return false;
}

float ascentProgress(float seconds){
    const float progress=std::clamp(seconds/launchDuration,0.f,1.f);
    return progress*std::sqrt(progress);
}

Vec rocketTravel(const Mat& view,const Mat& placement,Bounds bounds,Vec up){
    // Translate along camera up until even the lowest corner clears the
    // top clip plane. This also holds when the reader tilts or zooms the book.
    const auto& m=(view*placement).a;
    constexpr float margin=1.04f;
    const auto topGap=[&](int col){return margin*m[col*4+3]-m[col*4+1];};
    const float gap=topGap(3)+std::max(bounds.min.x*topGap(0),bounds.max.x*topGap(0))
        +std::max(bounds.min.y*topGap(1),bounds.max.y*topGap(1))
        +std::max(bounds.min.z*topGap(2),bounds.max.z*topGap(2));
    const auto& v=view.a;
    const float rise=v[1]*up.x+v[5]*up.y+v[9]*up.z;
    return up*(std::max(gap,0.f)/rise);
}

void exhaustCloud(MeshData& out,Vec center,Vec right,Vec up,Vec color,float alpha){
    auto vertex=[&](float x,float y){
        return Vertex{center+right*x+up*y,{},color,x,y,alpha};
    };
    quad(out,{vertex(-1,-1),vertex(1,-1),vertex(1,1),vertex(-1,1)});
}

} // namespace

struct Renderer::Data {
    GLuint program=createProgram(),cover=0,hudTexture=0,grainTexture=0;
    Size hudSize{},viewport{};
    GLuint maskFbo=0,maskTexture=0;
#ifdef BIBLIOTECA_MOBILE
    GLuint sceneFbo=0,sceneColor=0,sceneDepth=0;
    Size sceneSize{};
    bool sceneAvailable=true;
    GLint displayFbo=0;
    bool beginScene(Size requested,Size& target){
        glGetIntegerv(gl::framebufferBinding,&displayFbo);
        if(!sceneAvailable || requested.w<1 || requested.h<1)return false;
        if(!sceneFbo){
            glGenFramebuffers(1,&sceneFbo);
            glGenTextures(1,&sceneColor);
            glGenRenderbuffers(1,&sceneDepth);
            glActiveTexture(gl::texture0+4);
            glBindTexture(gl::texture,sceneColor);
            glTexParameteri(gl::texture,gl::minFilter,gl::linear);
            glTexParameteri(gl::texture,gl::magFilter,gl::linear);
            glTexParameteri(gl::texture,gl::wrapS,gl::clampEdge);
            glTexParameteri(gl::texture,gl::wrapT,gl::clampEdge);
        }
        if(requested.w!=sceneSize.w || requested.h!=sceneSize.h){
            glActiveTexture(gl::texture0+4);
            glBindTexture(gl::texture,sceneColor);
            glTexImage2D(gl::texture,0,gl::rgba8,requested.w,requested.h,0,gl::rgba,gl::byte,nullptr);
            glBindRenderbuffer(GL_RENDERBUFFER,sceneDepth);
            glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT16,requested.w,requested.h);
            glBindFramebuffer(gl::framebuffer,sceneFbo);
            glFramebufferTexture2D(gl::framebuffer,gl::colorAttachment,gl::texture,sceneColor,0);
            glFramebufferRenderbuffer(gl::framebuffer,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,sceneDepth);
            if(glCheckFramebufferStatus(gl::framebuffer)!=gl::complete){
                sceneAvailable=false;
                glBindFramebuffer(gl::framebuffer,GLuint(displayFbo));
                return false;
            }
            sceneSize=requested;
        }else glBindFramebuffer(gl::framebuffer,sceneFbo);
        glViewport(0,0,requested.w,requested.h);
        target=requested;
        return true;
    }
    void presentScene(Size screen){
        glBindFramebuffer(GL_READ_FRAMEBUFFER,sceneFbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,GLuint(displayFbo));
        glBlitFramebuffer(0,0,sceneSize.w,sceneSize.h,0,0,screen.w,screen.h,GL_COLOR_BUFFER_BIT,GL_LINEAR);
        glBindFramebuffer(gl::framebuffer,GLuint(displayFbo));
        glViewport(0,0,screen.w,screen.h);
    }
#endif
    int maskAsset=-1;
    bool maskCompact=false;
    float maskDeploy=-1;
    float maskMotion=-1,maskTime=-1,maskLaunchTime=-1;
    bool maskReady=false;
    struct InteriorMesh {Bounds bounds{};Mesh geometry;};
    std::array<InteriorMesh,roomSectionCount> interior;
    Mesh cube,boardMesh,spineMesh,folioMesh,interfacePlane,paper,attached,flames,exhaust;
    Mesh referenceLectern;
    Mesh contentGeometry;
    Bounds contentBounds{};
    bool hasContentGeometry=false;
    MeshData exhaustVertices;
    Mat view;
    Vec cameraUp,cameraRight;
    std::array<Mesh,2> doorSigns;
    std::array<GLuint,2> doorLabels{};
    std::array<std::array<Mesh,2>,popupCount> popups;
    std::array<std::array<MeshData,2>,popupCount> popupGeometry;
    struct MovingMesh { PopupActor actor; std::unique_ptr<Mesh> geometry; Bounds bounds; int side; };
    struct FoldedScene { std::array<Mesh,2> mesh; std::array<MeshData,2> geometry; };
    struct SceneMotion {
        std::array<Mesh,2> scenery;
        std::vector<MovingMesh> actors;
        std::unique_ptr<FoldedScene> afterLaunch;
    };
    std::array<SceneMotion,popupCount> motions;
    std::array<GLuint,bookCount> spines{},shelfCovers{};
    std::array<GLuint,spreads*2> folios{};
    std::array<GLuint,4> referenceFolios{};
    int cached=-1;
    bool lastLanguage=isEnglish();
    RenderStats stats;
    const GLint vp=loc("vp"),model=loc("model"),tintLoc=loc("tint"),eye=loc("eye"),
        hearth=loc("hearth"),fireTime=loc("fireTime"),readerLight=loc("readerLight"),
        dim=loc("dim"),mode=loc("mode"),material=loc("material"),shaded=loc("shaded"),
        popColor=loc("popColor"),popupLightMotion=loc("popupLightMotion");
    GLint loc(const char* name){return glGetUniformLocation(program,name);}

    Data(){
        glUseProgram(program);glUniform1i(loc("frontImage"),0);glUniform1i(loc("backImage"),1);glUniform1i(loc("maskImage"),2);
        glUniform1i(loc("materialGrain"),3);
        grainTexture=loadMaterialGrain();
        const auto& pieces=furniture();
        std::array<float,furnitureCount*4> grounding{};
        for(size_t i=0;i<pieces.size();++i){
            const auto b=furnitureBounds(pieces[i]);
            const Vec radius=(b.max-b.min)/2;
            grounding[i*4]=pieces[i].at.x;
            grounding[i*4+1]=pieces[i].at.z;
            grounding[i*4+2]=radius.x;
            grounding[i*4+3]=radius.z;
        }
        glUniform4fv(loc("furnitureFloor[0]"),furnitureCount,grounding.data());
        std::array<float,lanterns.size()*4> lampPositions{};
        for(size_t i=0;i<lanterns.size();++i){
            lampPositions[i*4]=lanterns[i].x;
            lampPositions[i*4+1]=lanterns[i].y;
            lampPositions[i*4+2]=lanterns[i].z;
        }
        glUniform4fv(loc("readingLamps[0]"),lanterns.size(),lampPositions.data());
        const auto sections=room();
        for(size_t i=0;i<sections.size();++i){
            interior[i].bounds=meshBounds(sections[i].mesh);
            interior[i].geometry.set(sections[i].mesh);
        }
        const auto content=contentSceneMesh();
        if(!content.empty()){
            contentBounds=meshBounds(content);
            contentGeometry.set(content);
            hasContentGeometry=true;
        }
        flames.set(hearthFlames());
        MeshData lectern;
        const auto stand=Mat::move(referenceLecternAt)*Mat::ry(-pi*.5f);
        auto lecternPart=[&](Vec at,Vec size,const char* pigment,Material surface,
                             const Mat& transform){
            MeshData piece;
            box(piece,at,size,tint(color(pigment)),surface,.025f);
            for(auto& vertex:piece){
                vertex.p=transform.point(vertex.p);
                vertex.n=transform.point(vertex.n,true);
            }
            lectern.insert(lectern.end(),piece.begin(),piece.end());
        };
        lecternPart({0,.054f,0},{.92f,.108f,.68f},"#49372e",Wood,stand);
        lecternPart({0,.110f,0},{.75f,.035f,.49f},"#b59462",Brass,stand);
        lecternPart({0,.70f,-.04f},{.19f,1.20f,.21f},"#685142",Wood,stand);
        const auto top=stand*Mat::move({0,1.35f,.02f})*Mat::rx(-.56f);
        lecternPart({0,0,0},{1.10f,.55f,.09f},"#594139",Wood,top);
        lecternPart({0,-.253f,.055f},{1.07f,.034f,.053f},"#c7a16b",Brass,top);
        referenceLectern.set(lectern);
        for(int i=0;i<2;++i){
            const float facing=i==0?-1.f:1.f;
            const auto point=[&](float u,float v){
                const Vec p{roomHalfWidth+facing*.134f,1.955f+(.5f-v)*.328f,doorZ-facing*(u-.5f)*2.41f};
                return Vertex{p,{facing,0,0},white,u,v,printedFront};
            };
            MeshData sign;
            quad(sign,{point(0,1),point(1,1),point(1,0),point(0,0)});
            doorSigns[i].set(sign);
            doorLabels[i]=printRoomPlaque(sectionName(i==0?Section::Adult:Section::Children));
        }
        MeshData v;box(v,{0,0,0},{1,1,1},white,Wood,.075f);cube.set(v);
        v.clear();box(v,{0,0,0},{1,1,1},white,Leather,.058f,2,1);boardMesh.set(v);
        v.clear();box(v,{0,0,0},{1,1,1},white,Leather,.079f,0,-1);spineMesh.set(v);
        v.clear();box(v,{0,0,0},{1,1,1},white,Paper,.025f,2,1);folioMesh.set(v);
        v.clear();
        auto vertex=[](float x,float y){return Vertex{{x,y,0},{0,0,1},white,x+.5f,.5f-y,Paper};};
        quad(v,{vertex(-.5f,-.5f),vertex(.5f,-.5f),vertex(.5f,.5f),vertex(-.5f,.5f)});
        interfacePlane.set(v);
        // Generate text textures on demand as shelves enter the camera view.
        const auto meshes=assetsPath()/"popups";
        for(int i=0;i<popupCount;++i){
            auto scene=loadPopup(meshes/popupAssets()[i].file);
            popupGeometry[i]=foldablePanels(scene);
            for(int side=0;side<2;++side)popups[i][side].set(popupGeometry[i][side]);
            if(scene.actors.empty())continue;
            const auto scenery=splitPaper(scene.resting);
            for(int side=0;side<2;++side)motions[i].scenery[side].set(scenery[side]);
            if(std::any_of(scene.actors.begin(),scene.actors.end(),[](const auto& actor){
                return actor.motion==PopupMotion::Launch;
            })){
                auto folded=std::make_unique<FoldedScene>();
                folded->geometry=foldablePanels(scene,false);
                for(int side=0;side<2;++side)folded->mesh[side].set(folded->geometry[side]);
                motions[i].afterLaunch=std::move(folded);
            }
            for(auto& actor:scene.actors){
                if(actor.mesh.empty())continue;
                const Bounds bounds=meshBounds(actor.mesh);
                auto geometry=std::make_unique<Mesh>();
                geometry->set(actor.mesh);
                actor.mesh.clear();
                actor.mesh.shrink_to_fit();
                const int side=actor.pivot.x<0?0:1;
                motions[i].actors.push_back({std::move(actor),std::move(geometry),bounds,side});
            }
        }
    }
    ~Data(){
        glDeleteTextures(shelfCovers.size(),shelfCovers.data());
        clearPages();glDeleteTextures(spines.size(),spines.data());glDeleteTextures(1,&hudTexture);
        glDeleteTextures(doorLabels.size(),doorLabels.data());
        glDeleteTextures(referenceFolios.size(),referenceFolios.data());
        glDeleteTextures(1,&maskTexture);glDeleteTextures(1,&grainTexture);glDeleteFramebuffers(1,&maskFbo);
#ifdef BIBLIOTECA_MOBILE
        glDeleteRenderbuffers(1,&sceneDepth);glDeleteTextures(1,&sceneColor);glDeleteFramebuffers(1,&sceneFbo);
#endif
        glDeleteProgram(program);
    }
    void refreshLanguage(){
        if(lastLanguage==isEnglish())return;
        lastLanguage=isEnglish();
        clearPages();
        glDeleteTextures(referenceFolios.size(),referenceFolios.data());
        referenceFolios.fill(0);
        glDeleteTextures(spines.size(),spines.data());spines.fill(0);
        glDeleteTextures(shelfCovers.size(),shelfCovers.data());shelfCovers.fill(0);
    }
    void clearPages(){
        glDeleteTextures(folios.size(),folios.data());folios.fill(0);
        glDeleteTextures(1,&cover);cover=0;cached=-1;
    }
    GLuint folio(int id,int index){
        const bool references=id==referenceBookId;
        auto& texture=references?referenceFolios[index]:folios[index];
        if(!texture){
            glActiveTexture(gl::texture0);
            texture=references?printReferencePage(index):printPage(id,index);
        }
        return texture;
    }
    void mesh(const Mesh& m,const Mat& transform,Vec c,int mat,GLuint tex=0,GLuint back=0,int overlay=0){
        if(m.count==0)return;
        glUniformMatrix4fv(model,1,0,transform.a.data());glUniform3f(tintLoc,c.x,c.y,c.z);
        glUniform1i(mode,overlay?overlay:back?2:tex?1:0);glUniform1i(material,mat);
        if(back){glActiveTexture(gl::texture0+1);glBindTexture(gl::texture,back);}
        if(tex){glActiveTexture(gl::texture0);glBindTexture(gl::texture,tex);}
        glBindVertexArray(m.vao);glDrawArrays(gl::triangles,0,m.count);
        ++stats.calls;stats.triangles+=m.count/3;
    }
    void rocketExhaust(Vec nozzle,Vec travel,float scale,float seconds,float motion){
        const float fade=1.f-ease((seconds-launchDuration)/trailFadeDuration);
        if(seconds<=0 || fade<=0)return;
        const float ignition=ease(seconds/.16f);
        const Vec ash{.73f,.67f,.64f},amber{1.f,.53f,.17f};
        constexpr int stamps=24;
        for(int i=0;i<stamps;++i){
            const float emitted=launchDuration*i/stamps;
            if(emitted>=seconds)break;
            const float age=seconds-emitted;
            const float spread=std::min(age,2.f);
            const float warmth=1.f-ease(age/.65f);
            const float radius=scale*(.053f+.030f*spread);
            const Vec drift=cameraRight*(scale*.030f*spread*std::sin(2.4f*i))
                +cameraUp*(scale*(.023f*spread-.032f));
            const Vec center=nozzle+travel*ascentProgress(emitted)+drift;
            exhaustCloud(exhaustVertices,center,cameraRight*radius,cameraUp*(radius*1.35f),
                         ash*(1.f-warmth)+amber*warmth,
                         (.19f+.18f*warmth)*fade*motion*ignition);
        }
        if(seconds<launchDuration){
            const Vec head=nozzle+travel*ascentProgress(seconds);
            exhaustCloud(exhaustVertices,head-cameraUp*(scale*.088f),cameraRight*(scale*.057f),
                         cameraUp*(scale*.111f),amber,.82f*motion*ignition);
            exhaustCloud(exhaustVertices,head-cameraUp*(scale*.046f),cameraRight*(scale*.028f),
                         cameraUp*(scale*.065f),{1.f,.94f,.63f},.90f*motion*ignition);
        }
    }
    void diorama(int asset,const std::array<Mat,2>& surfaces,float motion,float seconds,float launchSeconds=0,int pass=0){
        const auto& moving=motions[asset];
        const bool departed=launchSeconds>=launchDuration;
        if(motion==0 || moving.actors.empty()){
            const auto& folded=departed && moving.afterLaunch?moving.afterLaunch->mesh:popups[asset];
            for(int side=0;side<2;++side)
                mesh(folded[side],surfaces[side],white,Card,0,0,pass);
        }else{
            for(int side=0;side<2;++side)
                if(moving.scenery[side].count>0)mesh(moving.scenery[side],surfaces[side],white,Card,0,0,pass);
            for(const auto& part:moving.actors){
                const auto& surface=surfaces[part.side];
                if(part.actor.motion==PopupMotion::Launch){
                    const float progress=ascentProgress(launchSeconds);
                    if(pass==4){
                        if(!departed)mesh(*part.geometry,surface*Mat::move({0,progress*1.6f*motion,0}),
                                          white,Card,0,0,pass);
                        continue;
                    }
                    const Vec travel=rocketTravel(view,surface,part.bounds,cameraUp)*motion;
                    if(!departed)mesh(*part.geometry,Mat::move(travel*progress)*surface,white,Card);
                    if(launchSeconds<launchSequenceDuration){
                        const Vec nozzle=surface.point({part.actor.pivot.x,part.bounds.min.y,part.actor.pivot.z});
                        const Vec axis=surface.point({1,0,0},true);
                        rocketExhaust(nozzle,travel,std::sqrt(dot(axis,axis)),launchSeconds,motion);
                    }
                    continue;
                }
                const Mat actorPose=dynamicPopup(part.actor.motion)
                    ?animatePopup(part.actor,motion,seconds):Mat{};
                mesh(*part.geometry,surface*actorPose,white,Card,0,0,pass);
            }
        }
    }
    bool shadowMask(int asset,float deploy,bool compact,float motion,float seconds,float launchSeconds=0){
        if(deploy<=.025f)return false;
        GLint screenFbo=0;
        glGetIntegerv(gl::framebufferBinding,&screenFbo);
        constexpr int maskSize=256;
        if(!maskFbo){
            glGenFramebuffers(1,&maskFbo);glGenTextures(1,&maskTexture);
            glActiveTexture(gl::texture0+2);glBindTexture(gl::texture,maskTexture);
            glTexParameteri(gl::texture,gl::minFilter,gl::linear);glTexParameteri(gl::texture,gl::magFilter,gl::linear);
            glTexParameteri(gl::texture,gl::wrapS,gl::clampEdge);glTexParameteri(gl::texture,gl::wrapT,gl::clampEdge);
            glTexImage2D(gl::texture,0,gl::rgba8,maskSize,maskSize,0,gl::rgba,gl::byte,nullptr);
            glBindFramebuffer(gl::framebuffer,maskFbo);
            glFramebufferTexture2D(gl::framebuffer,gl::colorAttachment,gl::texture,maskTexture,0);
            maskReady=glCheckFramebufferStatus(gl::framebuffer)==gl::complete;
            glBindFramebuffer(gl::framebuffer,GLuint(screenFbo));
        }
        if(!maskReady)return false;
        const float time=motion>0.f?std::floor(seconds*10.f)*.1f:0.f;
        const float launchTime=std::min(launchDuration,launchSeconds);
        if(maskAsset==asset && maskDeploy==deploy && maskCompact==compact &&
           maskMotion==motion && maskTime==time && maskLaunchTime==launchTime)return true;
        glBindFramebuffer(gl::framebuffer,maskFbo);glViewport(0,0,maskSize,maskSize);
        glDisable(gl::depth);glClearColor(0,0,0,1);glClear(gl::colorBit);
        constexpr float x=1/pageOuter,y=2/pageHeight;
        const Mat project{{x,0,0,0,0,y,0,0,.48f*x,.29f*y,0,0,
                           -.48f*x*pageBlock,-.29f*y*pageBlock,0,1}};
        glUniform1f(fireTime,time);
        diorama(asset,{project*paperPanel(deploy,compact,0),project*paperPanel(deploy,compact,1)},
                motion,time,launchSeconds,4);
        glUniform1f(fireTime,seconds);
        glBindFramebuffer(gl::framebuffer,GLuint(screenFbo));glViewport(0,0,viewport.w,viewport.h);glEnable(gl::depth);
        glActiveTexture(gl::texture0+2);glBindTexture(gl::texture,maskTexture);
        maskAsset=asset;maskDeploy=deploy;maskCompact=compact;
        maskMotion=motion;maskTime=time;maskLaunchTime=launchTime;
        return true;
    }
    void book(int id,const Pose& pose,bool active,int spread=0,int target=0,float progress=1,float seconds=0,float launchSeconds=0){
        const auto root=pose.model();const auto hinge=panels(pose.open);
        const auto rear=root*hinge.rear,front=root*hinge.front;
        const bool sources=id==referenceBookId;
        const auto cloth=tint(sources?color("#3b625d"):bindingColor(id));
        const auto block=tint(color(sources?"#f8e8c8":pageEdge(id)));
        constexpr float w=pageOuter-pageInner,c=(pageOuter+pageInner)/2;
        auto solid=[&](const Mesh& object,const Mat& parent,Vec at,Vec size,Vec pigment,Material kind,GLuint ink=0){
            mesh(object,parent*Mat::move(at)*Mat::scale(size),pigment,kind,ink);
        };
        if(active && !sources && cached!=id){
            clearPages();cached=id;glActiveTexture(gl::texture0);cover=printCover(id);
        }
        solid(cube,rear,{.5f,0,-board/2},{1,coverHeight,board},cloth*.88f,Leather);
        GLuint jacket=0;
        if(!sources){
            if(active)jacket=cover;
            else{
                auto& resting=shelfCovers[id];
                if(!resting){
                    glActiveTexture(gl::texture0);
                    resting=printCover(id,true);
                }
                jacket=resting;
            }
        }
        solid(boardMesh,front,{.5f,0,board/2},{1,coverHeight,board},cloth,Leather,jacket);
        const float spineHeight=coverHeight*(.996f-.07f*pose.open),width=hinge.pivotZ+2*board;
        GLuint spineInk=0;
        if(!sources && pose.open<.52f){
            if(!spines[id]){glActiveTexture(gl::texture0);spines[id]=printSpine(id);}
            spineInk=spines[id];
        }
        solid(spineMesh,root,{hinge.xOffset-.009f,0,-.029f*pose.open},{.052f,spineHeight,width},
            cloth*.96f,Leather,spineInk);

        const auto state=readingState(spread,target,progress,pose.open);
        const bool printed=sources || (active && pose.open>.002f);
        const bool compact=!panoramicBook(id);
        const int figure=active && !sources?popupIndex(id,state.restingBookSpread):0;
        const float visibleLaunchTime=state.restingBookSpread==spread?launchSeconds:0.f;
        const float motion=active && !state.turning
            ?motionStrength(pose.open,progress,spread!=target):0.f;
#ifdef BIBLIOTECA_MOBILE
        const bool shading=false;
#else
        const bool shading=printed && !sources && !state.turning && shadowMask(figure,state.deploy,compact,motion,seconds,visibleLaunchTime);
#endif
        glUniform1i(shaded,shading?1:0);
        solid(printed?folioMesh:cube,rear,{c,0,pageBlock/2},{w,pageHeight,pageBlock},block,Paper,
              printed?folio(id,state.right):0);
        glUniform1i(shaded,shading?2:0);
        const Mat left=front*Mat::move({c,0,-pageBlock/2})*Mat::ry(pi);
        solid(printed?folioMesh:cube,left,{0,0,0},{w,pageHeight,pageBlock},block*.97f,Paper,
              printed?folio(id,state.left):0);
        glUniform1i(shaded,0);

        if(!active || pose.open<=.02f)return;
        const auto leftPage=front*Mat::ry(pi);
        if(state.turning){
            paper.set(turningSheet(state.leaf),gl::streamDraw);
            const GLuint frontInk=folio(id,state.left+1),backInk=folio(id,state.right-1);
            mesh(paper,root,block,Paper,frontInk,backInk);
            if(sources)return;

            const int previous=popupIndex(id,state.left/2),next=popupIndex(id,state.right/2);
            const bool departed=launchSeconds>=launchDuration;
            const auto* leftFold=departed && state.left/2==spread?motions[previous].afterLaunch.get():nullptr;
            const auto* rightFold=departed && state.right/2==spread?motions[next].afterLaunch.get():nullptr;
            mesh(leftFold?leftFold->mesh[0]:popups[previous][0],leftPage*paperPanel(0,compact,0),white,Card);
            mesh(rightFold?rightFold->mesh[1]:popups[next][1],rear*paperPanel(0,compact,1),white,Card);

            const auto& leafFront=leftFold?leftFold->geometry[1]:popupGeometry[previous][1];
            const auto& leafBack=rightFold?rightFold->geometry[0]:popupGeometry[next][0];
            attached.set(attachToSheet(leafFront,state.leaf,compact,1),gl::streamDraw);
            mesh(attached,root,white,Card);
            attached.set(attachToSheet(leafBack,state.leaf,compact,0),gl::streamDraw);
            mesh(attached,root,white,Card);
            return;
        }
        if(sources)return;
        glUniform1f(popupLightMotion,motion);
        diorama(figure,{leftPage*paperPanel(state.deploy,compact,0),
                        rear*paperPanel(state.deploy,compact,1)},motion,seconds,visibleLaunchTime);
        glUniform1f(popupLightMotion,0);
    }
};

Renderer::Renderer():data(std::make_unique<Data>()){}
Renderer::~Renderer()=default;
void Renderer::draw(const Library& lib,Size size,float seconds,float sceneScale){
    auto& d=*data;d.refreshLanguage();d.stats={};
    Size target=size;
#ifdef BIBLIOTECA_MOBILE
    const bool reduced=sceneScale<.995f && d.beginScene({std::max(1,int(size.w*sceneScale)),
        std::max(1,int(size.h*sceneScale))},target);
#else
    (void)sceneScale;
#endif
    d.viewport=target;
    d.exhaustVertices.clear();
    glViewport(0,0,target.w,target.h);glDisable(gl::blend);glEnable(gl::depth);glDepthFunc(gl::lequal);
    glClearColor(.075f,.071f,.067f,1);glClear(gl::colorBit|gl::depthBit);
    glUseProgram(d.program);
    const auto view=lib.viewProjection(size);
    d.view=view;d.cameraUp=lib.up();d.cameraRight=lib.right();
    const Vec eye=lib.eye();
    glUniformMatrix4fv(d.vp,1,0,view.a.data());glUniform3f(d.eye,eye.x,eye.y,eye.z);
    glUniform3f(d.hearth,hearthAt.x,hearthAt.y,hearthAt.z);
    glUniform1f(d.fireTime,seconds);
    const int id=lib.selected();
    float environmentDim=1.f;
    const bool readingStory=id>=0 && id!=referenceBookId;
    const bool readingLayer=readingStory && (lib.stage()==Stage::Reading || lib.stage()==Stage::Closing);
#ifdef BIBLIOTECA_MOBILE
    const bool crispBook=reduced && readingLayer;
#else
    const bool crispBook=false;
#endif
    if(readingStory){
        // Once the book is at the reader, keep its depth separate from room geometry.
        if(readingLayer)gl::depthRange(0,.36f);
        const auto pose=lib.pose();
        glDisable(gl::cullFace);
        glUniform1f(d.dim,1.f);glUniform1f(d.readerLight,1.f);
        glUniform1f(d.popColor,bookSection(id)==Section::Children?1.f:.45f);
        if(!crispBook)d.book(id,pose,true,lib.spread(),lib.target(),lib.turning(),seconds,lib.popupClock());
        environmentDim-=.32f*pose.open;
        if(readingLayer)gl::depthRange(.38f,1);
    }else if(d.cached>=0)d.clearPages();
    glUniform1f(d.dim,environmentDim);
    glUniform1f(d.readerLight,0.f);glUniform1f(d.popColor,0.f);
    const Bounds portal=Bounds::box({roomHalfWidth,(1.57f+roomFloor)*.5f,doorZ},
                                   {.25f,1.57f-roomFloor,doorHalf*2});
    const bool seesOtherRoom=inView(view,portal);
    const auto currentRoom=lib.section();
    glEnable(gl::cullFace);
    for(size_t i=1;i<d.interior.size();++i){
        const auto& section=d.interior[i];
        const bool adult=(section.bounds.min.x+section.bounds.max.x)*.5f>roomHalfWidth;
        if((seesOtherRoom || adult==(currentRoom==Section::Adult)) && inView(view,section.bounds))
            d.mesh(section.geometry,Mat{},white,-1);
    }
    d.mesh(d.interior[0].geometry,Mat{},white,-1);
    if(d.hasContentGeometry && inView(view,d.contentBounds))
        d.mesh(d.contentGeometry,Mat{},white,-1);
    if(inView(view,Bounds::box({roomHalfWidth,1.955f,doorZ},{.3f,.4f,2.5f}))){
        const int side=currentRoom==Section::Adult?1:0;
        d.mesh(d.doorSigns[side],Mat{},white,LightStrip,d.doorLabels[side]);
    }
    glDisable(gl::cullFace);
    if(inView(view,referenceLecternBounds())){
        d.mesh(d.referenceLectern,Mat{},white,-1);
        const bool reading=id==referenceBookId;
        const int spread=lib.lecternSpread();
        glUniform1f(d.readerLight,.72f);
        d.book(referenceBookId,onLectern(),reading,spread,reading?lib.target():spread,
               reading?lib.turning():1.f);
        glUniform1f(d.readerLight,0.f);
    }
    for(const auto& slot:positions()){
        if(slot.id==id || (!seesOtherRoom && bookSection(slot.id)!=currentRoom && slot.cabinet!=-2) ||
           !inView(view,slot.bounds(lib.push(slot.id))))continue;
        d.book(slot.id,onShelf(slot.id,lib.push(slot.id)),false);
    }
    const Bounds fireBounds=Bounds::box(hearthAt+Vec{0,.94f,0},{1.6f,2.15f,1.6f});
    if((currentRoom==Section::Children || seesOtherRoom) && inView(view,fireBounds)){
        glUniform1f(d.dim,environmentDim);
        glEnable(gl::cullFace);glEnable(gl::blend);glDepthMask(0);glBlendFunc(gl::srcAlpha,gl::one);
        d.mesh(d.flames,Mat::move(hearthAt),white,Ember,0,0,5);
        glDepthMask(1);glDisable(gl::blend);glDisable(gl::cullFace);
    }
    const auto drawExhaust=[&]{
        if(d.exhaustVertices.empty())return;
        if(readingLayer)gl::depthRange(0,.36f);
        d.exhaust.set(d.exhaustVertices,gl::streamDraw);
        glEnable(gl::blend);glDepthMask(0);glBlendFunc(gl::srcAlpha,gl::oneMinusAlpha);
        d.mesh(d.exhaust,Mat{},white,Ember,0,0,6);
        glDepthMask(1);glDisable(gl::blend);
    };
    if(!crispBook)drawExhaust();
    gl::depthRange(0,1);
#ifdef BIBLIOTECA_MOBILE
    if(reduced)d.presentScene(size);
    if(crispBook){
        // Only the room is upscaled. Render printed pages and pop-up geometry
        // at native resolution so the reading experience remains sharp.
        glClear(gl::depthBit);glEnable(gl::depth);glDepthFunc(gl::lequal);
        glDisable(gl::cullFace);
        glUniform1f(d.dim,1.f);glUniform1f(d.readerLight,1.f);
        glUniform1f(d.popColor,bookSection(id)==Section::Children?1.f:.45f);
        gl::depthRange(0,.36f);
        d.book(id,lib.pose(),true,lib.spread(),lib.target(),lib.turning(),seconds,lib.popupClock());
        drawExhaust();
        gl::depthRange(0,1);
    }
#endif
}
void Renderer::hud(const Paint* image){
    auto& d=*data;
    if(image){
        if(!d.hudTexture){
            glGenTextures(1,&d.hudTexture);glActiveTexture(gl::texture0);glBindTexture(gl::texture,d.hudTexture);
            glTexParameteri(gl::texture,gl::minFilter,gl::linear);glTexParameteri(gl::texture,gl::magFilter,gl::linear);
            glTexParameteri(gl::texture,gl::wrapS,gl::clampEdge);glTexParameteri(gl::texture,gl::wrapT,gl::clampEdge);
        }
        glActiveTexture(gl::texture0);glBindTexture(gl::texture,d.hudTexture);
        const Size s=image->size;
        if(s.w!=d.hudSize.w || s.h!=d.hudSize.h){
            glTexImage2D(gl::texture,0,gl::rgba8,s.w,s.h,0,gl::paintFormat,gl::byte,image->texturePixels());
            d.hudSize=s;
        }else glTexSubImage2D(gl::texture,0,0,0,s.w,s.h,gl::paintFormat,gl::byte,image->texturePixels());
    }
    if(!d.hudTexture)return;
    glDisable(gl::depth);glEnable(gl::blend);glBlendFunc(gl::one,gl::oneMinusAlpha);
    d.mesh(d.interfacePlane,Mat{},white,Paper,d.hudTexture,0,3);
    glDisable(gl::blend);
}
RenderStats Renderer::stats() const{return data->stats;}
}
