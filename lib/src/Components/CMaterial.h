#pragma once
#include "component.h"
#include <vector>
namespace fm
{
class Shader;
class Material;
}

namespace fm
{
	using Materials = std::vector<fm::Material*>;
}


namespace fmc {

class CMaterial : public FMComponent<CMaterial> {
    public:
        CMaterial();
        ~CMaterial();

        bool Serialize(nlohmann::json &ioJson) const override;
        bool Read(const nlohmann::json &inJSON) override;
        const std::string& GetName() const override;
		uint16_t GetType() const override {return kMaterial;}
        void Destroy() override;
		
        const fm::Materials& GetAllMaterials() {return _materials;}
		fm::Material* GetMainMaterial() const { if (!_materials.empty()) return _materials[0]; else return nullptr; }
    private:
        fm::Materials _materials;
        bool _hasChanged = false;

};
}
