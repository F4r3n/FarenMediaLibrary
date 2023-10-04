#pragma once

namespace fm
{
	class FilePath;
}

namespace gui
{
	class ShaderCompiler
	{
	public:
		static bool Compile(const fm::FilePath& inPath);

	};
}
