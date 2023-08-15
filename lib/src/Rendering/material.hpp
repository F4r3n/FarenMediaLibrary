#ifndef MATERIAL_HPP
#define MATERIAL_HPP


#include <memory>
#include <vector>
#include <Rendering/MaterialValue.h>
#include <Resource/Resource.h>
#include <Core/serializer.hpp>
#include <Rendering/Graphics.hpp>
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

	void AddSettings(fm::RENDERING_TYPE inRenderingType, bool value)
	{
		_renderingSettings[inRenderingType] = value;
	}

	const std::vector< MaterialProperty>& GetValues() const{ return _materialProperties; }
private:
	std::unordered_map<fm::RENDERING_TYPE, bool> _renderingSettings;
	std::vector< MaterialProperty> _materialProperties;
};

class Material : public Resource
{

    public:

		Material Clone() const
		{
			Material mat(_name);
			mat._shader = _shader;
			mat._properties = _properties;
		}
        Material(const fm::FilePath &inFilePath);

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



        const std::string& GetName() const {return _name;}
		std::shared_ptr<fm::Shader> GetShader() const { return _shader; }
		void SetShader(std::shared_ptr<fm::Shader> inShader) { _shader = inShader; Compile(); }
		void Compile();
		bool IsReady() const;


		virtual void Save() const override;
		virtual void Load() override;

    private:
		std::shared_ptr<fm::Shader> _shader;

        std::string _name;
		MaterialProperties _properties;
};


}


#endif // MATERIAL_HPP

