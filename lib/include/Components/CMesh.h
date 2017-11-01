#pragma once
#include <string>
#include <vector>
#include "Core/Math/Vector2.h"

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

    void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override {
    }
    void parse(rapidjson::Value& value) override {
    }

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