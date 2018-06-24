#pragma once
#include "component.h"
#include "Rendering/material.hpp"
#include <vector>
namespace fm
{
class Shader;
}


namespace fmc {


class CMaterial : public FMComponent<CMaterial> {
    public:
        CMaterial();
        ~CMaterial();

        bool Serialize(json &ioJson) const override;
        bool Read(const json &inJSON) override;
        const std::string& GetName() const override;
        size_t GetType() const override {return kMaterial;}
        void Destroy() override;


        inline void SetFlagHasChanged() {for(auto &s: _materials) s.SetFlagHasChanged();}
        bool Reload();
        std::vector<fm::Material>& GetAllMaterials() {return _materials;}

    private:
        std::vector<fm::Material> _materials;
        bool _hasChanged = false;

};
}
