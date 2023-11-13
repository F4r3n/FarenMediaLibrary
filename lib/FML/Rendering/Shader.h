#pragma once
#if defined(_MSC_VER) && _MSC_VER >= 1900
#pragma warning(disable: 4100)
#endif
#include <string>

#include <Core/Config.h>

#include "Resource/Resource.h"
#include "Core/Math/Vector2.h"
#include "Core/Color.h"
#include "Core/Math/Matrix.h"
#include "Core/Math/Vector3.h"
#include <Resource/Resource.h>
#include <filesystem>
#include "MaterialValue.h"
#include "SubShader.hpp"
#include "ShaderKind.hpp"
namespace fm
{

	class MaterialValue;
	class Color;


	class Shader : public Resource
	{
	public:

		Shader(const fm::FilePath& inFilePath);

		~Shader();
		static fm::RESOURCE_TYPE getType() { return fm::RESOURCE_TYPE::SHADER; }
		fm::RESOURCE_TYPE GetType() const override { return getType(); }

		const std::string& GetName() const { return _name; }

		void Save(nlohmann::json& outJSON) const override;
		void Load(const nlohmann::json& inJSON) override;
		void AddSubShader(SHADER_KIND inKind, const SubShader::Reflections& inReflection);
		std::optional<SubShader> GetSubShader(SHADER_KIND inKind) const;
		static SHADER_KIND ConvertStringsToShaderKind(const std::vector<std::string>& inStrings);

		std::vector<SHADER_KIND> GetPossibleShaderKind() const;

	protected:
		virtual std::filesystem::file_time_type GetTimeStamp() const override;

	private:
		std::string _name;
		std::map<SHADER_KIND, SubShader> _subShaders;

	public:
		uint32_t GetID() const { return _currentID; }

	private:
		inline static uint32_t _ID = 0;
		uint32_t	_currentID = 0;

	};
}
