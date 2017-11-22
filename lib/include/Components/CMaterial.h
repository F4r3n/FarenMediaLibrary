#pragma once
#include "Component.h"
#include "Core/Color.h"
#include "Rendering/Texture.h"
#include <Rendering/Shader.h>
#include <memory>
#include <map>
#include <Rendering/MaterialValue.h>
namespace fmc {

    class CMaterial : public Component<CMaterial> {
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

            fm::Shader* shader = nullptr;

            void setValue(const std::string& name, int value) {
                fm::MaterialValue materialValue;
                materialValue = value;
                values.insert(std::pair<std::string, fm::MaterialValue>(name, value));
            }

            const std::map<std::string, fm::MaterialValue>& getValues() const {
                return values;
            }
        private:
            fm::Texture texture;
            std::map<std::string, fm::MaterialValue> values;
    };
}
