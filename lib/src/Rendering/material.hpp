#pragma once

#include <memory>
#include <vector>
#include <Rendering/MaterialValue.h>
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

	enum STANDARD_MATERIAL_PROPERTIES
	{
		ALBEDO = 0,
		TEXTURE_ALBEDO = 1
	};
	using MaterialValues = std::unordered_map< std::string, MaterialValue>;

	struct MaterialValueInfo
	{
		uint32_t			offset = 0;
		uint32_t			size = 0;
		std::string			name;
		fm::ValuesType		type;
	};

	class Material : public Resource
	{

	public:

		Material Clone() const;

		struct MaterialBufferInfo
		{
			uint32_t				bindingPoint = 0;
			uint32_t				setPoint = 0;
			fm::SubShader::STAGE	stages = fm::SubShader::STAGE::ALL;
		};

		struct MaterialBufferInfo_Buffer
		{
			unsigned char*			buffer = nullptr;
			uint32_t				bufferSize = 0;
			MaterialBufferInfo		info;
		};

		struct MaterialBufferInfo_Texture
		{
			MaterialBufferInfo			info;
			std::weak_ptr<fm::Texture>	texture;
		};

		~Material()
		{
			free(_buffer);
			_buffer = nullptr;
		}

		Material(const fm::FilePath& inFilePath);
		void From(const fm::Material& inMaterial);
		template <typename T>
		void SetPropertyValue(const std::string& name, T value) {
			fm::MaterialValue materialValue;
			materialValue = value;

			_properties[name] = materialValue;

		}
		void SetPropertyValue(const std::string& name, fm::MaterialValue& inMaterialValue) {
			_properties[name] = inMaterialValue;
		}

		template <typename T>
		void SetUniformValue(const std::string& name, T value) {
			fm::MaterialValue materialValue;
			materialValue = value;

			_uniforms[name] = materialValue;

		}
		void SetUniformValue(const std::string& name, fm::MaterialValue& inMaterialValue) {
			_uniforms[name] = inMaterialValue;
		}
		static constexpr fm::RESOURCE_TYPE getType() { return fm::RESOURCE_TYPE::MATERIAL; }
		virtual fm::RESOURCE_TYPE GetType() const override { return getType(); }

		MaterialValues& GetUniforms() { return _uniforms; }
		MaterialValues& GetProperties() { return _properties; }

		const std::string& GetName() const { return _name; }
		const fm::FilePath& GetShaderPath() const { return _shaderPath; }
		void SetShaderPath(const fm::FilePath& inPath, fm::SHADER_KIND inKind = fm::SHADER_KIND::PLAIN);

		void Save(nlohmann::json& outJSON) const override;
		void Load(const nlohmann::json& inJSON) override;

		void Save() const;
		bool CanBeModified() const;
		std::optional<fm::SubShader> GetSubShader() const;

		SHADER_KIND		GetShaderKind() const { return _shaderkind; }
		MaterialKind	GetMaterialKind() const;
		void			UpdateProperty(const std::string& inName, const fm::MaterialValue& inProperty, uint32_t offset);
		uint32_t		GetBufferSize() const { return _bufferSize; }
		unsigned char*	GetBufferPtr() const { return _buffer; }

		MaterialBufferInfo_Buffer GetMaterialBufferInfo_Buffer(GRAPHIC_API inAPI) const;
		std::vector<MaterialBufferInfo_Texture> GetMaterialBufferInfo_Textures(GRAPHIC_API inAPI) const;

		static std::shared_ptr<fm::Material> GetDefaultStandardMaterial();
		std::unordered_map<std::string, MaterialValueInfo> GetMaterialPropertiesInfo() const;
		bool HasAlbedoTexture() const;
		void SetAlbedoTexture(const fm::FilePath& inPath);
	private:
		void _BuildMaterialPropertiesInfo();
		void _UpdateInternalBuffWithProperties();
		void _FillJSONValue(nlohmann::json& valueJSON, const std::string& inName, const fm::MaterialValue& inValue) const;
		void _GetJSONValue(const nlohmann::json& valueJSON, std::string& outName, fm::MaterialValue& outValue) const;
		bool _GetOffsetFromReflection(const std::string& inName, uint32_t& offset) const;
		std::optional<fm::SubShader::Block>	_GetReflectionBlock(const std::string& inName, GRAPHIC_API inAPI) const;
		std::optional<fm::SubShader::Uniform>	_GetReflectionUniform(const std::string& inName, GRAPHIC_API inAPI) const;
		std::vector<fm::SubShader::Uniform>		_GetReflectionTextures(GRAPHIC_API inAPI) const;

		fm::FilePath		_shaderPath;
		SHADER_KIND			_shaderkind = SHADER_KIND::PLAIN;
		std::string			_name;
		MaterialValues		_properties;
		MaterialValues		_uniforms;
		std::unordered_map<std::string, MaterialValueInfo> _materialPropertiesInfos;


		unsigned char*		_buffer = nullptr;
		uint32_t			_bufferSize = 0;

	public:
		uint32_t GetID() const { return _currentID; }

	private:
		inline static uint32_t _ID = 0;
		uint32_t	_currentID = 0;


	};


}

