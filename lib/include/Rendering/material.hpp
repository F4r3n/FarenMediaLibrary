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
    std::string name;

    MaterialProperty()
    {
       // materialValue = fm::MaterialValue();
    }

    fm::MaterialProperty& operator=(fm::MaterialProperty &&m)
    {
        materialValue = m.materialValue;//TODO not good
        m.name.swap(name);
        return *this;
    }

    MaterialProperty(const fm::MaterialProperty &m)
    {
        materialValue.setType(m.materialValue.getType());
        materialValue.setValue(m.materialValue.getType(), m.materialValue);
        name = m.name;
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
            p.name = name;
            _properties.push_back(p);
        }

        void setValue(const std::string& name, fm::MaterialValue &inMaterialValue) {
            MaterialProperty p;
            p.materialValue = inMaterialValue;
            p.name = name;
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

