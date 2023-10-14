#pragma once
#include <memory>
#include <map>
#include "glslang/Public/ShaderLang.h"
#include "Rendering/Shader.h"
#include "Rendering/GraphicsAPI.h"
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
		struct ShaderCompilerSettings
		{
			GRAPHIC_API				api;
			fm::Folder				shaderFolder;
			bool					generateReflection = true;
			bool					generatePreprocess = true;
			bool					generateSPV = true;
			std::vector<fm::Folder>	listFoldersToInclude;
		};

		ShaderCompiler();
		bool Compile(const ShaderCompilerSettings& inSettings, fm::Shader::Reflection & outReflection);
	private:
		std::unique_ptr<glslang::TShader> _CompileLang(const ShaderCompilerSettings& inSettings, EShLanguage inLang, const fm::File& inFile, EShMessages messageFlags);
		void							  _WriteSPV(glslang::TIntermediate* intermediateRef, const fm::FilePath& inDestination);
		void							  _BuildReflection(glslang::TProgram& program, fm::Shader::Reflection& outReflection);
		GRAPHIC_API _api = GRAPHIC_API::OPENGL;
	};
}
