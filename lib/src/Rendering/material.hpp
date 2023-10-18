#pragma once

#include <memory>
#include <vector>
#include <Rendering/MaterialValue.h>
#include <Resource/Resource.h>
#include <Core/serializer.hpp>
#include <Rendering/Graphics.hpp>
#include "Rendering/Shader.h"
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

		fm::MaterialProperty& operator=(fm::MaterialProperty&& m)
		{
			name = m.name;
			materialValue = m.materialValue;

			return *this;
		}


		fm::MaterialProperty& operator=(const fm::MaterialProperty& m)
		{
			name = m.name;
			materialValue = m.materialValue;

			return *this;
		}

		MaterialProperty(const fm::MaterialProperty& m)
		{
			materialValue.setValue(m.materialValue.getType(), m.materialValue);
			name = m.name;
		}

		MaterialProperty(const std::string& inName, const fm::MaterialValue& inMaterialValue)
		{
			materialValue = inMaterialValue;
			name = inName;
		}
	};


	class MaterialProperties
	{

		using properties_t = std::vector< MaterialProperty>;
	public:
		using iterator = properties_t::iterator;
		using const_iterator = properties_t::const_iterator;

		iterator begin() { return _materialProperties.begin(); }
		iterator end() { return _materialProperties.end(); }
		const_iterator begin() const { return _materialProperties.begin(); }
		const_iterator end() const { return _materialProperties.end(); }
		const_iterator cbegin() const { return _materialProperties.cbegin(); }
		const_iterator cend() const { return _materialProperties.cend(); }

		void AddValue(const std::string& name, const fm::MaterialValue& inMaterialValue)
		{
			MaterialProperty p(name, inMaterialValue);
			_materialProperties.push_back(p);
		}

		void UpdateProperty(size_t index, const fm::MaterialProperty& inProperty)
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
	private:
		std::unordered_map<fm::RENDERING_TYPE, bool> _renderingSettings;
		std::vector< MaterialProperty> _materialProperties;
	};

	class Material : public Resource
	{

	public:

		Material Clone() const
		{
			fm::FilePath p(_name);
			Material mat(p);
			mat._shaderPath = _shaderPath;
			mat._properties = _properties;
		}
		Material(const fm::FilePath& inFilePath);

		template <typename T>
		void setValue(const std::string& name, T value) {
			fm::MaterialValue materialValue;
			materialValue = value;

			_properties.AddValue(name, materialValue);

		}

		void UpdateProperty(size_t inIndex, const fm::MaterialProperty& inProperty)
		{
			_properties.UpdateProperty(inIndex, inProperty);
		}

		void setValue(const std::string& name, fm::MaterialValue& inMaterialValue) {
			_properties.AddValue(name, inMaterialValue);
		}
		static constexpr fm::RESOURCE_TYPE getType() { return fm::RESOURCE_TYPE::MATERIAL; }
		virtual fm::RESOURCE_TYPE GetType() const override { return getType(); }

		MaterialProperties& GetProperties() { return _properties; }



		const std::string& GetName() const { return _name; }
		const fm::FilePath& GetShaderPath() const { return _shaderPath; }
		void SetShaderPath(const fm::FilePath& inPath) { _shaderPath = inPath; }
		//void Compile();
		//bool IsReady() const;


		void Save(nlohmann::json& outJSON) const override;
		void Load(const nlohmann::json& inJSON) override;

		void Save() const override;
		std::optional<fm::SubShader> GetSubShader();
		SHADER_KIND GetShaderKind() const { return _kind; }
		
	private:
		fm::FilePath _shaderPath;
		SHADER_KIND _kind = SHADER_KIND::PLAIN;
		std::string _name;
		MaterialProperties _properties;

	public:
		uint32_t GetID() const { return _currentID; }

	private:
		inline static uint32_t _ID = 0;
		uint32_t	_currentID = 0;


	};


}

