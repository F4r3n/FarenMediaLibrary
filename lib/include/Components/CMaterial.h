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

        bool Serialize(json &ioJson) const override;
        bool Read(const json &inJSON) override;
        const std::string& GetName() const override;
        size_t GetType() const override {return kMaterial;}
        void Destroy() override;


        void setValue(const std::string& name, int value);
        const std::map<std::string, fm::MaterialValue>& getValues() const;

        std::string shaderName = "default";
        fm::Color color;
        fm::Shader* shader = nullptr;



        inline void SetFlagHasChanged() {_hasChanged = true;}
        bool Reload();

    private:
        std::map<std::string, fm::MaterialValue> values;
        bool _hasChanged = true;

};
}
