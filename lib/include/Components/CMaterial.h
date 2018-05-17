#pragma once
#include "component.h"
#include "Core/Color.h"
#include "Rendering/Texture.h"

#include <memory>
#include <map>
#include <Rendering/MaterialValue.h>

namespace fm
{
class Shader;
}


namespace fmc {


class CMaterial : public FMComponent<CMaterial> {
    public:
        CMaterial();
        ~CMaterial();
        CMaterial(const fm::Color& color, bool bloom);
        void setTexture(const fm::Texture& texture);
        const fm::Texture& getTexture();
        void setValue(const std::string& name, int value);
        const std::map<std::string, fm::MaterialValue>& getValues() const;

        bool textureReady = false;
        std::string shaderName = "default";
        fm::Color color;
        bool bloom = false;  // TODO to remove bloom from material
        static const std::string name;
        fm::Shader* shader = nullptr;
    private:
        fm::Texture texture;
        std::map<std::string, fm::MaterialValue> values;
};
}
