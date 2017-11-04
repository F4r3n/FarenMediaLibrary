#pragma once
#include "Component.h"
#include "Core/Color.h"
#include "Rendering/Texture.h"
#include "Serializer.h"
#include <Rendering/Shader.h>
#include <memory>
#include <map>
#include <Rendering/MaterialValue.h>
namespace fmc {

class CMaterial : public Component<CMaterial>, public Serializer {
   public:
    CMaterial();
    ~CMaterial();
    CMaterial(const fm::Color& color, bool bloom);
    void setTexture(const fm::Texture& texture);
    const fm::Texture& getTexture();
    bool textureReady = false;
    std::string shaderName = "default";
    fm::Color color;
    bool bloom = false;  // TODO to remove bloom from material

    static const std::string name;
    void serialize(
        rapidjson::Writer<rapidjson::StringBuffer>& writer) override {
    }
    void parse(rapidjson::Value& value) override {
    }

    fm::Shader* shader = nullptr;

    void setValue(const std::string& name, int value) {
        fm::MaterialValue materialValue;
        materialValue = value;
        values.insert(std::pair<std::string, fm::MaterialValue>(name, value));
    }

    const std::map<std::string, fm::MaterialValue>& getValues() const {
        return values;
    }
    int* get(int v) override{
        //std::cout << "Called" << std::endl;
        return nullptr;}
   private:
    fm::Texture texture;
    std::map<std::string, fm::MaterialValue> values;
};
}