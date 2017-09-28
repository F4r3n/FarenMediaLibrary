#pragma once
#include <string>
#include <vector>
#include "Core/Math/Vector2.h"
/*#ifndef __EMSCRIPTEN__
#define USE_GLEW 0
#endif

#if USE_GLEW
#include "GL/glew.h"
#endif

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#if !USE_GLEW
#include "SDL/SDL_opengl.h"
#endif*/
#include <Core/Config.h>

#include <Component.h>
#include "Core/Color.h"
#include "Core/Bounds.h"

#include "Serializer.h"
#include <Rendering/Model.hpp>
namespace fmc {
enum SHAPE { RECTANGLE, CIRCLE, LAST_SHAPE };
struct Vertex {
    fm::math::Vector2f position;
    fm::math::Vector2f uv;
};

class CMesh : public Component<CMesh>, public Serializer {

public:
    CMesh(std::string type);
    CMesh();
    ~CMesh();

    static const std::string name;
    int currentShape = -1;

    void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override {
    }
    void parse(rapidjson::Value& value) override {
    }

    void computeBoundingSize();
    void setType(const std::string &type);
    fm::Bounds bounds;
    bool IsmodelReady();
    std::string getModelType() {return type;} 
        fm::Model* model = nullptr;

private:
    bool created = false;
    std::string type = "Quad";
};
}