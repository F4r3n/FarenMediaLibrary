#ifndef MATERIAL_HPP
#define MATERIAL_HPP


#include <memory>
#include <vector>
#include <Rendering/MaterialValue.h>
#include <Resource/Resource.h>
#include <Core/serializer.hpp>
namespace fm
{
class Shader;
}



namespace fm
{

struct MaterialProperty
{
    fm::MaterialValue materialValue;
    char name[256] = {'\0'};

    MaterialProperty()
    {
       // materialValue = fm::MaterialValue();
    }

    fm::MaterialProperty& operator=(fm::MaterialProperty &&m)
    {
        strncpy_s(name, m.name, sizeof(name)-1);
		materialValue = m.materialValue;

        return *this;
    }

    MaterialProperty(const fm::MaterialProperty &m)
    {
        materialValue.setType(m.materialValue.getType());
        materialValue.setValue(m.materialValue.getType(), m.materialValue);
        strncpy_s(name, m.name, sizeof(name)-1);
    }
};

class Material : public Resource, public Serializer
{

    public:
        bool Serialize(nlohmann::json &ioJson) const override;
        bool Read(const nlohmann::json &inJSON) override;


        Material(const std::string &id);
        template <typename T>
        void setValue(const std::string& name, T value) {
            fm::MaterialValue materialValue;
            materialValue = value;

            MaterialProperty p;
            p.materialValue = materialValue;
            strncpy_s(p.name, name.c_str(), std::min(name.size() ,sizeof(p.name)-1));
            _properties.push_back(p);
        }

        void setValue(const std::string& name, fm::MaterialValue &inMaterialValue) {
            MaterialProperty p;
            p.materialValue = inMaterialValue;
            strncpy_s(p.name, name.c_str(), std::min(name.size(), sizeof(p.name)-1));
            _properties.push_back(p);
        }
        static constexpr fm::RESOURCE_TYPE getType() {return fm::RESOURCE_TYPE::MATERIAL;}

        std::vector<MaterialProperty>& getValues();

        std::string shaderName = "default";

        fm::Shader* shader = nullptr;

        inline void SetFlagHasChanged() {_hasChanged = true;}
        bool Reload();
        const std::string& GetID() const {return _id;}

    private:
        std::string _id = "none";
        std::vector<MaterialProperty> _properties;
        bool _hasChanged = true;
};


}


#endif // MATERIAL_HPP

