#pragma once
#include <memory>
#include <map>
#include "glslang/Public/ShaderLang.h"
#include "Rendering/Shader.h"
#include "Rendering/GraphicsAPI.h"
#include <set>
namespace fm
{
	class FilePath;
	class File;
}

namespace gui
{
	class ShaderCompiler
	{
	public:

		class ShaderMacro
		{
		public:
			void DefineMacro(const std::string& inMacro, uint32_t inValue);
			void UndefMacro(const std::string& inMacro);
			void RemoveMacro(const std::string& inMacro);
			std::vector<std::string> ToPreprocess() const;
			std::string ToPreamble() const;

		private:
			std::unordered_map<std::string, uint32_t> _macros;
			std::set<std::string>					  _undefMacros;
		};


		struct ShaderCompilerSettings
		{
			GRAPHIC_API				api;
			fm::Folder				shaderFolder;
			std::optional<fm::Folder>shaderOutputFolder;
			bool					generateReflection = true;
			bool					generatePreprocess = true;
			bool					generateSPV = true;
			std::vector<fm::Folder>	listFoldersToInclude;
			ShaderMacro				macros;

			fm::FilePath			GetDestinationPath() const {
				if (shaderOutputFolder.has_value())
					return shaderOutputFolder->GetPath();
				return shaderFolder.GetPath();
			}
		};

		ShaderCompiler();
		bool Compile(const ShaderCompilerSettings& inSettings, fm::SubShader::Reflection & outReflection);
	private:
		std::unique_ptr<glslang::TShader> _CompileLang(const ShaderCompilerSettings& inSettings, EShLanguage inLang, const fm::File& inFile, EShMessages messageFlags);
		void							  _WriteSPV(glslang::TIntermediate* intermediateRef, const fm::FilePath& inDestination);
		void							  _BuildReflection(glslang::TProgram& program, fm::SubShader::Reflection& outReflection);
		GRAPHIC_API _api = GRAPHIC_API::OPENGL;
	};
}
