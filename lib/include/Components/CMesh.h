#pragma once
#include <string>
#include <vector>
#include "Core/Math/Vector2.h"

#include <Core/Config.h>

#include <Component.h>
#include "Core/Color.h"
#include "Core/Bounds.h"

#include <Rendering/Model.hpp>
namespace fmc {
enum SHAPE { RECTANGLE, CIRCLE, LAST_SHAPE };
struct Vertex {
    fm::math::Vector2f position;
    fm::math::Vector2f uv;
};

class CMesh : public Component<CMesh> {

public:
    CMesh(std::string type);
    CMesh();
    ~CMesh();
    int* get(int v) {return nullptr;}

    static const std::string name;


    void computeBoundingSize();
    void setType(const std::string &type);
    fm::Bounds bounds;
    bool IsmodelReady();
    const std::string& getModelType() const{return type;}
    void setModelType(const std::string &type) {this->type = type;}
    fm::Model* model = nullptr;

private:
    bool created = false;
    std::string type = "Quad";
};
}
