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
        memcpy(name, m.name, sizeof(name));
		materialValue = m.materialValue;

        return *this;
    }


	fm::MaterialProperty& operator=(const fm::MaterialProperty &m)
	{
		memcpy(name, m.name, sizeof(name));
		materialValue = m.materialValue;

		return *this;
	}

    MaterialProperty(const fm::MaterialProperty &m)
    {
        materialValue.setType(m.materialValue.getType());
        materialValue.setValue(m.materialValue.getType(), m.materialValue);
		memcpy(name, m.name, sizeof(name));
    }

	MaterialProperty(const std::string& inName, const fm::MaterialValue &inMaterialValue)
	{
		materialValue = inMaterialValue;
		memcpy(name, inName.c_str(), sizeof(inName));
	}



};


class MaterialProperties
{
public:
	void AddValue(const std::string& name, const fm::MaterialValue &inMaterialValue)
	{
		MaterialProperty p(name, inMaterialValue);
		_materialProperties.push_back(p);
	}

	void UpdateProperty(size_t index, const fm::MaterialProperty &inProperty)
	{
		if (index >= 0 && index < _materialProperties.size())
		{
			_materialProperties[index] = inProperty;
		}
	}

	const std::vector< MaterialProperty>& GetValues() const{ return _materialProperties; }
private:
	std::vector< MaterialProperty> _materialProperties;
};

class Material : public Resource, public Serializer
{

    public:
        bool Serialize(nlohmann::json &ioJson) const override;
        bool Read(const nlohmann::json &inJSON) override;

		Material Clone() const
		{
			Material mat(_id);
			mat._properties = _properties;
			mat._hasChanged = _hasChanged;
			mat.shader = shader;
			mat.shaderName = shaderName;
		}
        Material(const std::string &id);
        template <typename T>
        void setValue(const std::string& name, T value) {
            fm::MaterialValue materialValue;
            materialValue = value;

			_properties.AddValue(name, materialValue);

        }

		void UpdateProperty(size_t inIndex, const fm::MaterialProperty &inProperty)
		{
			_properties.UpdateProperty(inIndex, inProperty);
		}

        void setValue(const std::string& name, fm::MaterialValue &inMaterialValue) {
			_properties.AddValue(name, inMaterialValue);
        }
        static constexpr fm::RESOURCE_TYPE getType() {return fm::RESOURCE_TYPE::MATERIAL;}

        const std::vector<MaterialProperty>& getValues() const;
		const MaterialProperties& GetProperties() const { return _properties; }

        std::string shaderName = "default";

        fm::Shader* shader = nullptr;

        inline void SetFlagHasChanged() {_hasChanged = true;}
        bool Reload();
        const std::string& GetID() const {return _id;}

    private:
        std::string _id = "none";
		MaterialProperties _properties;
        bool _hasChanged = true;
};


}


#endif // MATERIAL_HPP

