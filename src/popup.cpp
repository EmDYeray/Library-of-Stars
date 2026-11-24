#include "popup.hpp"
#include "locale.hpp"
#include "book_art.hpp"
#include "stories.hpp"
#include <charconv>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace biblioteca {

const std::array<PopupAsset,popupCount>& popupAssets(){
    static constexpr std::array<PopupAsset,popupCount> assets{{
        {"satelite.obj","Communications satellite"},
        {"observador.obj","Observation satellite"},
        {"cohete.obj","Launch vehicle"},
        {"estacion.obj","Orbital station"},
        {"antena.obj","Dish antenna"},
        {"vehiculo.obj","Exploration vehicle"},
        {"sonda.obj","Space probe"},
        {"telescopio.obj","Orbital telescope"},
        {"marte.obj","Mars · Exploration rover"},
        {"europa.obj","Europa · Ice probe"},
        {"luna.obj","Moon · Lander"},
        {"saturno.obj","Saturn · Ring probe"},
        {"tierra.obj","Earth · Orbital observation"},
        {"sol.obj","Sun · Solar observatory"},
        {"marte-cantera.obj","Mars · Quarry and drill"},
        {"europa-crevasse.obj","Europa · Crevasse crossing"},
        {"luna-puesto.obj","Moon · Polar outpost"},
        {"saturno-travesia.obj","Saturn · Probe traverse"},
        {"tierra-nocturna.obj","Earth · Night relay"},
        {"sol-corona.obj","Sun · Probe shield"},
        {"orbita-ensamble.obj","Orbit · Assembly platform"},
        {"tierra-incendio.obj","Earth · Valley survey"},
        {"europa-descenso.obj","Europa · Descent rehearsal"},
        {"orbita-atraque.obj","Orbit · Docking port"},
        {"marte-tormenta.obj","Mars · Storm shelter"},
        {"luna-capsula.obj","Moon · Relay capsule"},
        {"saturno-lunas.obj","Saturn · Moons and ice"},
        {"sol-fulguracion.obj","Sun · Solar flare"},
        {"marte-cumbre.obj","Mars · Ridge station"},
        {"apolo-eagle.obj","Apollo 11 · Eagle"},
        {"apolo-intrepid.obj","Apollo 12 · Intrepid"},
        {"apolo-falcon.obj","Apollo 15 · Falcon and rover"},
        {"kepler-telescopio.obj","Kepler · Space telescope"},
        {"kepler-transito.obj","Kepler · Planet transit"},
        {"pioneer-jupiter.obj","Pioneer 10 · Jupiter encounter"},
        {"insight-marte.obj","InSight · Martian seismic station"},
        {"rovers-gemelos.obj","Mars · Spirit and Opportunity"},
        {"pioneer-salida.obj","Pioneer 10 · Earth departure"},
        {"pioneer-asteroides.obj","Pioneer 10 · Asteroid belt"},
        {"pioneer-placa.obj","Pioneer 10 · The plaque"},
        {"pioneer-senal.obj","Pioneer 10 · A distant signal"},
        {"kepler-equilibrio.obj","Kepler · K2 balancing"},
        {"kepler-campo.obj","Kepler · New star field"},
        {"apolo-muestras.obj","Apollo 11 · Lunar samples"},
        {"apolo-red.obj","Apollo · Science station"},
        {"apolo-surveyor.obj","Apollo 12 · Surveyor 3"},
        {"apolo-impacto.obj","Apollo 12 · Recorded impact"},
        {"apolo-hadley.obj","Apollo 15 · Hadley traverse"},
        {"apolo-caida.obj","Apollo 15 · Hammer and feather"},
        {"insight-ondas.obj","InSight · Seismic waves"},
        {"insight-taladro.obj","InSight · Heat probe"},
        {"insight-polvo.obj","InSight · Dust on panels"},
        {"marte-travesia.obj","Mars · Hillside wheels"},
        {"orbita-sistemas.obj","Orbit · Habitat systems"},
        {"apolo-easep.obj","Apollo 11 · Surface experiments"},
        {"insight-capas.obj","InSight · Layers of Mars"}
    }};
    static constexpr std::array<PopupAsset,popupCount> assetsSpanish{{
        {"satelite.obj","Satélite de comunicaciones"},
        {"observador.obj","Satélite de observación"},
        {"cohete.obj","Vehículo de lanzamiento"},
        {"estacion.obj","Estación orbital"},
        {"antena.obj","Antena parabólica"},
        {"vehiculo.obj","Vehículo de exploración"},
        {"sonda.obj","Sonda espacial"},
        {"telescopio.obj","Telescopio orbital"},
        {"marte.obj","Marte · Rover de exploración"},
        {"europa.obj","Europa · Sonda del hielo"},
        {"luna.obj","La Luna · Módulo de descenso"},
        {"saturno.obj","Saturno · Sonda entre los anillos"},
        {"tierra.obj","Tierra · Observación orbital"},
        {"sol.obj","El Sol · Observatorio solar"},
        {"marte-cantera.obj","Marte · Cantera y taladro"},
        {"europa-crevasse.obj","Europa · Paso sobre la grieta"},
        {"luna-puesto.obj","La Luna · Puesto polar"},
        {"saturno-travesia.obj","Saturno · Travesía de la sonda"},
        {"tierra-nocturna.obj","Tierra · Relevo nocturno"},
        {"sol-corona.obj","El Sol · Escudo de la sonda"},
        {"orbita-ensamble.obj","Órbita · Banco de ensamble"},
        {"tierra-incendio.obj","Tierra · Inspección del valle"},
        {"europa-descenso.obj","Europa · Ensayo de descenso"},
        {"orbita-atraque.obj","Órbita · Puerto de atraque"},
        {"marte-tormenta.obj","Marte · Refugio de la tormenta"},
        {"luna-capsula.obj","La Luna · Cápsula de relevo"},
        {"saturno-lunas.obj","Saturno · Lunas y hielo disperso"},
        {"sol-fulguracion.obj","El Sol · Fulguración solar"},
        {"marte-cumbre.obj","Marte · Estación en la cresta"},
        {"apolo-eagle.obj","Apolo 11 · Eagle"},
        {"apolo-intrepid.obj","Apolo 12 · Intrepid"},
        {"apolo-falcon.obj","Apolo 15 · Falcon y el coche lunar"},
        {"kepler-telescopio.obj","Kepler · Telescopio espacial"},
        {"kepler-transito.obj","Kepler · Tránsito de un planeta"},
        {"pioneer-jupiter.obj","Pioneer 10 · Encuentro con Júpiter"},
        {"insight-marte.obj","InSight · Estación sísmica marciana"},
        {"rovers-gemelos.obj","Marte · Spirit y Opportunity"},
        {"pioneer-salida.obj","Pioneer 10 · Salida de la Tierra"},
        {"pioneer-asteroides.obj","Pioneer 10 · Cinturón de asteroides"},
        {"pioneer-placa.obj","Pioneer 10 · La placa"},
        {"pioneer-senal.obj","Pioneer 10 · Una señal lejana"},
        {"kepler-equilibrio.obj","Kepler · El equilibrio de K2"},
        {"kepler-campo.obj","Kepler · Un nuevo campo de estrellas"},
        {"apolo-muestras.obj","Apolo 11 · Muestras lunares"},
        {"apolo-red.obj","Apolo · Una estación científica"},
        {"apolo-surveyor.obj","Apolo 12 · Surveyor 3"},
        {"apolo-impacto.obj","Apolo 12 · Un impacto registrado"},
        {"apolo-hadley.obj","Apolo 15 · Recorrido por Hadley"},
        {"apolo-caida.obj","Apolo 15 · El martillo y la pluma"},
        {"insight-ondas.obj","InSight · Ondas sísmicas"},
        {"insight-taladro.obj","InSight · La sonda térmica"},
        {"insight-polvo.obj","InSight · Polvo en los paneles"},
        {"marte-travesia.obj","Marte · Ruedas en la ladera"},
        {"orbita-sistemas.obj","Órbita · Sistemas del hábitat"},
        {"apolo-easep.obj","Apolo 11 · Experimentos de superficie"},
        {"insight-capas.obj","InSight · Capas de Marte"}
    }};
    return isEnglish()?assets:assetsSpanish;
}
int popupIndex(int book,int spread){
    using A=Illustration;
    using F=Figure;
    if(book<originalCount){
        constexpr int slide=2;
        const int chapterIndex=panoramicBook(book)?spread:spread*2;
        if(destination(book)==Destination::Mars && chapterIndex==slide)return int(F::MarsTraverse);
        return int(chapter(book,chapterIndex).model);
    }
    if(book<firstImport){
        static constexpr F sources[originalAdultCount][adultSpreads]{
            {F::Telescope,F::Dish,F::OrbitalBuild,F::Imager},
            {F::Station,F::OrbitalSystems,F::Satellite,F::OrbitalDock},
            {F::Earth,F::Imager,F::EarthFire,F::EarthNight},
            {F::ApolloHadley,F::Dish,F::Moon,F::MoonCapsule}
        };
        return int(sources[book-originalCount][spread]);
    }
    const auto selected=[&](std::array<F,4> four){return int(four[spread]);};
    switch(illustrationFor(book)){
        case A::Eagle:return book==firstImport
            ?selected({F::ApolloEagle,F::Moon,F::ApolloSamples,F::MoonCapsule})
            :selected({F::ApolloEagle,F::ApolloSamples,F::ApolloEasep,F::MoonCapsule});
        case A::Intrepid:return selected({F::ApolloIntrepid,F::ApolloNetwork,F::ApolloSurveyor,F::ApolloImpact});
        case A::Falcon:return selected({F::ApolloFalcon,F::ApolloHadley,F::ApolloDrop,F::MoonOutpost});
        case A::Kepler:return selected({F::KeplerWatch,F::KeplerTransit,F::KeplerBalance,F::KeplerField});
        case A::Pioneer:return book==firstImport+4
            ?selected({F::PioneerLaunch,F::PioneerBelt,F::PioneerJupiter,F::PioneerSignal})
            :selected({F::PioneerLaunch,F::PioneerJupiter,F::PioneerPlaque,F::PioneerSignal});
        case A::Insight:return book==firstImport+6
            ?selected({F::Insight,F::InsightWaves,F::InsightLayers,F::InsightDust})
            :selected({F::Insight,F::InsightWaves,F::InsightDrill,F::InsightDust});
        case A::Rovers:return selected({F::TwinRovers,F::MarsTraverse,F::MarsQuarry,F::MarsStorm});
        case A::Earth:return selected({F::Earth,F::Imager,F::EarthFire,F::EarthNight});
        case A::Europa:return selected({F::Europa,F::EuropaDescent,F::Probe,F::EuropaCrevasse});
        case A::Orbit:return selected({F::Station,F::OrbitalSystems,F::OrbitalBuild,F::OrbitalDock});
        case A::Saturn:return selected({F::Saturn,F::SaturnMoons,F::Probe,F::SaturnCrossing});
        case A::Sun:return selected({F::Sun,F::Dish,F::SolarFlare,F::SolarCorona});
    }
    return int(F::Earth);
}
std::filesystem::path assetsPath(){
#ifdef __ANDROID__
    const char* path=SDL_AndroidGetInternalStoragePath();
    if(!path)throw std::runtime_error(SDL_GetError());
    return std::filesystem::path(path)/"assets";
#elif defined(BIBLIOTECA_IPAD)
    const char* base=SDL_GetBasePath();
    if(!base)throw std::runtime_error(SDL_GetError());
    return std::filesystem::path(base)/"assets";
#elif defined(_WIN32)
    char* base=SDL_GetBasePath();
    if(!base)throw std::runtime_error(SDL_GetError());
    const std::filesystem::path path(base);
    SDL_free(base);
    return path/"assets";
#else
    std::error_code error;
    auto path=std::filesystem::read_symlink("/proc/self/exe",error);
    return (error?std::filesystem::current_path():path.parent_path())/"assets";
#endif
}

PopupScene loadPopup(const std::filesystem::path& path){
    std::ifstream file(path);
    if(!file)throw std::runtime_error("Missing model "+path.string());
    PopupScene scene;
    MeshData* vertices=&scene.resting;
    struct PointColor {Vec point,color;};
    struct Index {size_t position;std::optional<size_t> normal;};
    std::vector<PointColor> positions;
    std::vector<Vec> normals;
    const auto objIndex=[](std::string_view value,size_t count){
        size_t i=0;
        const auto result=std::from_chars(value.data(),value.data()+value.size(),i);
        if(result.ec!=std::errc{} || result.ptr!=value.data()+value.size() || i==0 || i>count)
            throw std::runtime_error("Invalid OBJ index");
        return i-1;
    };
    Vec pigment=white;
    Material material=Card;
    std::string line;
    while(std::getline(file,line)){
        std::istringstream tokens(line);
        std::string key;
        tokens>>key;
        if(key=="#"){
            tokens>>key;
            if(key=="pop_still")vertices=&scene.resting;
            if(key=="pop_motion"){
                std::string mode;
                Vec pivot;
                static constexpr std::array<std::string_view,11> kinds{
                "fly","launch","spin","ringed","drift","roll","beacon","scan","drop","pulse","slide"};
                tokens>>mode>>pivot.x>>pivot.y>>pivot.z;
                const auto kind=std::find(kinds.begin(),kinds.end(),mode);
                if(!tokens || kind==kinds.end())throw std::runtime_error("Invalid OBJ animation");
                scene.actors.push_back({PopupMotion(kind-kinds.begin()),pivot,{}});
                vertices=&scene.actors.back().mesh;
            }
        }else if(key=="v"){
            Vec pos;
            if(!(tokens>>pos.x>>pos.y>>pos.z))throw std::runtime_error("Invalid OBJ vertex");
            Vec c;
            if(tokens>>c.x){
                if(!(tokens>>c.y>>c.z))throw std::runtime_error("Invalid OBJ vertex color");
                positions.push_back({pos,c});
            }else positions.push_back({pos,white});
        }else if(key=="vn"){
            Vec normal;
            if(!(tokens>>normal.x>>normal.y>>normal.z) || dot(normal,normal)<.5f)
                throw std::runtime_error("Invalid OBJ normal");
            normals.push_back(unit(normal));
        }else if(key=="usemtl"){
            std::string tag;
            if(!(tokens>>tag) || tag.size()<6 || tag.find_first_not_of("0123456789aAbBcCdDeEfF",0)<6)
                throw std::runtime_error("Invalid OBJ color");
            const auto key=std::string_view(tag).substr(6);
            if(key=="_metal")material=Metal;
            else if(key=="_solar")material=SolarCell;
            else if(key=="_lamp")material=Lamp;
            else if(key=="_rubber")material=Rubber;
            else if(key=="_planet")material=Planet;
            else if(key.empty())material=Card;
            else throw std::runtime_error("Unknown OBJ material");
            pigment=tint(color(std::string_view(tag).substr(0,6)));
        }else if(key=="f"){
            std::array<Index,3> indices;
            size_t n=0;
            std::string corner;
            while(tokens>>corner){
                if(n==indices.size())throw std::runtime_error("Invalid OBJ face");
                const std::string_view v=corner;
                const size_t slash=v.find('/');
                Index index{objIndex(v.substr(0,slash),positions.size()),{}};
                if(slash!=v.npos){
                    if(v.substr(slash,2)!="//")throw std::runtime_error("OBJ face missing normal");
                    index.normal=objIndex(v.substr(slash+2),normals.size());
                }
                indices[n++]=index;
            }
            if(n!=3)throw std::runtime_error("OBJ face is not triangular");
            const Vec a=positions[indices[0].position].point,b=positions[indices[1].position].point,
                      c=positions[indices[2].position].point;
            const Vec normal=cross(b-a,c-a);
            if(dot(normal,normal)<1e-12f)continue;
            for(const auto& index:indices){
                const auto& v=positions[index.position];
                const Vec shade{v.color.x*pigment.x,v.color.y*pigment.y,v.color.z*pigment.z};
                auto point=v.point;
                point.y=std::max(0.f,point.y);
                vertices->push_back({point,index.normal?normals[*index.normal]:unit(normal),
                                    shade,v.point.x,v.point.y,float(material)});
            }
        }
    }
    if(scene.resting.empty() && scene.actors.empty())throw std::runtime_error("Empty model "+path.string());
    return scene;
}

std::array<MeshData,2> foldablePanels(const PopupScene& scene,bool includeRocket){
    auto geometry=scene.resting;
    for(const auto& actor:scene.actors)
        if(includeRocket || actor.motion!=PopupMotion::Launch)
            geometry.insert(geometry.end(),actor.mesh.begin(),actor.mesh.end());
    return splitPaper(geometry);
}

float motionStrength(float opening,float progress,bool flipping){
    constexpr float start=.92f;
    const float ready=ease((opening-start)/(1.f-start));
    if(!flipping)return ready;
    return ready*(1-ease(progress/.16f)+ease((progress-start)/(1.f-start)));
}

bool dynamicPopup(PopupMotion motion){
    return motion==PopupMotion::Flight || motion==PopupMotion::Launch ||
           motion==PopupMotion::Drift || motion==PopupMotion::Drop ||
           motion==PopupMotion::Roll || motion==PopupMotion::Spin ||
           motion==PopupMotion::Ringed || motion==PopupMotion::Scan || motion==PopupMotion::Slide;
}

Mat animatePopup(const PopupActor& actor,float strength,float seconds){
    const Vec p=actor.pivot;
    const float beat=seconds*.92f+p.x*3.7f+p.y;
    Vec movement{};
    float yaw=0,pitch=0,scale=1;
    switch(actor.motion){
        case PopupMotion::Flight:
            movement={.024f*std::sin(beat),.014f*std::cos(beat*1.23f),.017f*std::sin(beat*.67f)};
            yaw=.095f*std::cos(beat);pitch=.041f*std::sin(beat*.85f);
            break;
        case PopupMotion::Launch: break; // The renderer fits the ascent to the visible frame.
        case PopupMotion::Spin:
            yaw=.18f*std::sin(beat*.72f);
            break;
        case PopupMotion::Ringed:
            yaw=.13f*std::sin(beat*.70f);
            break;
        case PopupMotion::Drift:
            movement={.024f*std::sin(beat),.016f*std::cos(beat),.015f*std::sin(beat*.95f)};
            yaw=.20f*std::sin(beat*.82f);
            break;
        case PopupMotion::Roll:
            movement={.026f*std::sin(beat*.92f),0,.005f*std::sin(beat*.92f)};
            yaw=.029f*std::cos(beat*.92f);
            break;
        case PopupMotion::Beacon:
            yaw=.17f*std::sin(beat);
            break;
        case PopupMotion::Scan:
            yaw=.13f*std::sin(beat*.81f);
            break;
        case PopupMotion::Slide:
            movement.x=.027f*std::sin(beat*.74f);
            break;
        case PopupMotion::Drop: {
            const float cycle=std::fmod(seconds*.42f,1.f);
            movement.y=-.050f*ease(cycle/.42f)*(1-ease((cycle-.61f)/.37f));
            break;
        }
        case PopupMotion::Pulse:
            scale+=strength*.026f*std::sin(beat*1.23f);
            break;
    }
    // Return via the short arc before the page starts to bend.
    return Mat::move(p+movement*strength)*Mat::ry(yaw*strength)*Mat::rx(pitch*strength)*Mat::scale({scale,scale,scale})*Mat::move(p*-1.f);
}

ReadingState readingState(int spread,int target,float progress,float opening){
    const float raise=ease((opening-popStart)/(1-popStart));
    if(spread==target || progress>=1)return {2*spread,2*spread+1,raise,0,false,-1,spread};
    if(progress<=popTurnStart)
        return {2*spread,2*spread+1,raise*(1-ease(progress/popTurnStart)),0,false,-1,spread};
    if(progress>=popTurnEnd)
        return {2*target,2*target+1,raise*ease((progress-popTurnEnd)/(1-popTurnEnd)),0,false,-1,target};
    const float t=ease((progress-popTurnStart)/(popTurnEnd-popTurnStart));
    const int low=std::min(spread,target),high=std::max(spread,target);
    return {2*low,2*high+1,0,target>spread?t:1-t,true,low,high};
}

std::array<MeshData,2> splitPaper(const MeshData& scene){
    std::array<MeshData,2> panels;
    auto cut=[](const Vertex& a,const Vertex& b){
        const float t=a.p.x/(a.p.x-b.p.x);
        return Vertex{a.p+(b.p-a.p)*t,a.n+(b.n-a.n)*t,a.c+(b.c-a.c)*t,
                      std::lerp(a.u,b.u,t),std::lerp(a.v,b.v,t),a.mat};
    };
    for(int side=0;side<2;++side){
        const float direction=side==0?-1.f:1.f;
        auto& panel=panels[side];
        for(size_t i=0;i<scene.size();i+=3){
            std::array<Vertex,4> clipped;
            size_t n=0;
            for(size_t j=0;j<3;++j){
                const auto& a=scene[i+j];
                const auto& b=scene[i+(j+1)%3];
                const bool insideA=a.p.x*direction>=0,insideB=b.p.x*direction>=0;
                if(insideA)clipped[n++]=a;
                if(insideA!=insideB)clipped[n++]=cut(a,b);
            }
            for(size_t j=1;j+1<n;++j)panel.insert(panel.end(),{clipped[0],clipped[j],clipped[j+1]});
        }
    }
    for(auto& panel:panels)
        for(auto& vertex:panel)vertex.p.y=std::max(0.f,vertex.p.y);
    return panels;
}

Mat paperPanel(float deployed,bool compact,int side){
    const float paperWidth=compact?.72f:.90f;
    const float angle=pi-.5f*pi*deployed;
    const float cosine=std::abs(std::cos(angle)),sine=std::sin(angle);
    // The cutout keeps the same footprint. Only the accordion pleats lose depth.
    const float thickness=std::lerp(foldedDepth,paperWidth,std::pow(std::max(sine,0.f),10.f));
    const float clearance=std::max(0.f,cosine*thickness*.52f-.0045f)+.007f*std::max(sine,0.f);
    const Vec hinge{side==0?-paperGap:paperGap,compact?.590f:.524f,pageBlock+.0068f+clearance};
    return Mat::move(hinge)*Mat::rx(angle)*Mat::scale({paperWidth,paperWidth,thickness});
}

MeshData attachToSheet(const MeshData& panel,float t,bool compact,int side){
    MeshData moved;
    moved.reserve(panel.size());
    const auto flat=paperPanel(0,compact,side);
    const float sign=side==0?-1.f:1.f;
    for(auto vertex:panel){
        const auto folded=flat.point(vertex.p);
        const float r=folded.x*sign-pageInner;
        const Vec tangent=unit(leafPoint(r+.0005f,0,t)-leafPoint(r-.0005f,0,t));
        const Vec normal=cross(tangent,{0,1,0})*sign;
        vertex.p=leafPoint(r,folded.y,t)+normal*(folded.z-pageBlock);
        const float w=compact?.72f:.90f;
        vertex.n=unit(tangent*(sign*vertex.n.x/w)+Vec{0,-vertex.n.y/w,0}
                      -normal*(vertex.n.z/foldedDepth));
        moved.push_back(vertex);
    }
    return moved;
}

MeshData turningSheet(float t){
    MeshData data;constexpr int segments=36;
    constexpr float half=.00125f;
    data.reserve(segments*12+segments*12+12);
    auto vertex=[&](int i,bool top,bool front,int material){
        const float u=float(i)/segments,r=u*(pageOuter-pageInner);
        const Vec p=leafPoint(r,top?pageHeight/2:-pageHeight/2,t);
        const Vec tangent=unit(leafPoint(r+.0005f,0,t)-leafPoint(r-.0005f,0,t));
        const Vec normal=cross(tangent,{0,1,0})*(front?1.f:-1.f);
        return Vertex{p+normal*half,normal,white,front?u:1-u,top?0.f:1.f,float(material)};
    };
    auto edge=[&](std::array<Vertex,4> q){
        const Vec n=unit(cross(q[1].p-q[0].p,q[2].p-q[0].p));
        for(auto& v:q){v.n=n;v.c={.74f,.64f,.48f};}
        quad(data,q);
    };
    for(int i=0;i<segments;++i){
        for(bool front:{true,false}){
            const int type=front?printedFront:printedBack;
            const auto a=vertex(i,false,front,type),b=vertex(i+1,false,front,type),
                       c=vertex(i+1,true,front,type),d=vertex(i,true,front,type);
            if(front)quad(data,{a,b,c,d});else quad(data,{d,c,b,a});
        }
        for(bool top:{true,false})
            edge({vertex(i,top,true,Paper),vertex(i,top,false,Paper),
                  vertex(i+1,top,false,Paper),vertex(i+1,top,true,Paper)});
    }
    for(int end:{0,segments})
        edge({vertex(end,false,true,Paper),vertex(end,true,true,Paper),
              vertex(end,true,false,Paper),vertex(end,false,false,Paper)});
    return data;
}
}
