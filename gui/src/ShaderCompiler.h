#pragma once
#include <memory>
#include <map>
#include "glslang/Public/ShaderLang.h"
#include "Rendering/MaterialValue.h"
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
		struct Uniform
		{
			std::string name;
			int binding = -1;
			int set = -1;
			fm::ValuesType type = fm::ValuesType::VALUE_NONE;
		};

		struct Reflection
		{
			std::map<std::string, Uniform> uniforms;
		};


		ShaderCompiler();
		bool Compile(const fm::FilePath& inPath, Reflection & outReflection);
	private:
		std::unique_ptr<glslang::TShader> _CompileLang(EShLanguage inLang, const fm::File& inFile, EShMessages messageFlags);
		
	};
}
