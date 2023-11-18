#pragma once
#include <vector>
#include <FML/Core/FilePath.h>

namespace gui
{
	class ShaderIncluder
	{
	public:
		ShaderIncluder(const std::vector<fm::Folder>& inListIncludes);
		bool Parse(const fm::File& inFile, std::string& outContent) const;
	private:
		std::optional<fm::File> _FindFile(const std::string_view& inPath) const;

		std::vector<fm::Folder> _listIncludes;
		const std::string _includeKeyword = "#include";

	};
}