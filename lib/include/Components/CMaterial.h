#pragma once
#include "Component.h"
#include "Core/Color.h"
#include "Rendering/Texture.h"
#include "Serializer.h"
namespace fmc {
class CMaterial : public Component<CMaterial>, public Serializer {
public:
    CMaterial();
    ~CMaterial();
    CMaterial(const fm::Color &color, bool bloom);
    void setTexture(const fm::Texture& texture);
    const fm::Texture& getTexture();
    bool textureReady = false;
    std::string shaderName = "default";
    fm::Color color;
    bool bloom = false;//TODO to remove bloom from material
    

    static const std::string name;
    void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override{}
    void parse(rapidjson::Value &value) override {}
private:
    fm::Texture texture;
};
}