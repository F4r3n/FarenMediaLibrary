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
	enum class MaterialKind
	{
		STANDARD,
		SHADER
	};

	class MaterialProperties
	{

		using properties_t = std::unordered_map< std::string, MaterialValue>;
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
			_materialProperties.emplace(name, inMaterialValue);
		}

		void UpdateProperty(const std::string& inName, const fm::MaterialValue& inProperty)
		{
			_materialProperties[inName] = inProperty;
		}

		MaterialValue& Get(const std::string& inName)
		{
			return _materialProperties[inName];
		}

		bool Has(const std::string& inName) const
		{
			return _materialProperties.contains(inName);
		}
	private:
		properties_t _materialProperties;
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
			mat._uniforms = _uniforms;
			mat._bufferSize = _bufferSize;
			memcpy(mat._buffer, _buffer, _bufferSize);
		}

		struct MaterialBufferInfo
		{
			unsigned char*		buffer = nullptr;
			uint32_t			bufferSize = 0;
			uint32_t			bindingPoint = 0;
			uint32_t			setPoint = 0;
			fm::SubShader::STAGE	stages = fm::SubShader::STAGE::ALL;
		};

		~Material()
		{
			delete[] _buffer;
			_buffer = nullptr;
		}

		Material(const fm::FilePath& inFilePath);

		template <typename T>
		void SetPropertyValue(const std::string& name, T value) {
			fm::MaterialValue materialValue;
			materialValue = value;

			_properties.AddValue(name, materialValue);

		}
		void SetPropertyValue(const std::string& name, fm::MaterialValue& inMaterialValue) {
			_properties.AddValue(name, inMaterialValue);
		}

		template <typename T>
		void SetUniformValue(const std::string& name, T value) {
			fm::MaterialValue materialValue;
			materialValue = value;

			_uniforms.AddValue(name, materialValue);

		}
		void SetUniformValue(const std::string& name, fm::MaterialValue& inMaterialValue) {
			_uniforms.AddValue(name, inMaterialValue);
		}
		static constexpr fm::RESOURCE_TYPE getType() { return fm::RESOURCE_TYPE::MATERIAL; }
		virtual fm::RESOURCE_TYPE GetType() const override { return getType(); }

		MaterialProperties& GetUniforms() { return _uniforms; }
		MaterialProperties& GetProperties() { return _properties; }

		const std::string& GetName() const { return _name; }
		const fm::FilePath& GetShaderPath() const { return _shaderPath; }
		void SetShaderPath(const fm::FilePath& inPath);

		void Save(nlohmann::json& outJSON) const override;
		void Load(const nlohmann::json& inJSON) override;

		void Save() const override;
		std::optional<fm::SubShader> GetSubShader() const;

		SHADER_KIND		GetShaderKind() const { return _shaderkind; }
		void			SetShaderKind(fm::SHADER_KIND inKind);
		MaterialKind	GetMaterialKind() const;
		void			UpdateProperty(const std::string& inName, fm::MaterialValue& inProperty, uint32_t offset);
		uint32_t		GetBufferSize() const { return _bufferSize; }
		unsigned char*	GetBufferPtr() const { return _buffer; }

		MaterialBufferInfo GetMaterialBufferInfo(GRAPHIC_API inAPI) const;
	private:
		void _FillJSONValue(nlohmann::json& valueJSON, const std::string& inName, const fm::MaterialValue& inValue) const;
		void _GetJSONValue(const nlohmann::json& valueJSON, std::string& outName, fm::MaterialValue& outValue) const;
		bool _GetOffsetFromReflection(const std::string& inName, uint32_t& offset) const;
		std::optional<fm::SubShader::Block>	_GetReflectionBlock(const std::string& inName, GRAPHIC_API inAPI) const;
		std::optional<fm::SubShader::Uniform>	_GetReflectionUniform(const std::string& inName, GRAPHIC_API inAPI) const;

		fm::FilePath		_shaderPath;
		SHADER_KIND			_shaderkind = SHADER_KIND::PLAIN;
		std::string			_name;
		MaterialProperties	_properties;
		MaterialProperties	_uniforms;


		unsigned char*		_buffer = nullptr;
		uint32_t			_bufferSize = 0;

	public:
		uint32_t GetID() const { return _currentID; }

	private:
		inline static uint32_t _ID = 0;
		uint32_t	_currentID = 0;


	};


}

