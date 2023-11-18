#include "ShaderIncluder.hpp"


using namespace gui;

ShaderIncluder::ShaderIncluder(const std::vector<fm::Folder>& inListIncludes)
	: _listIncludes(inListIncludes)
{

}

std::optional<fm::File> ShaderIncluder::_FindFile(const std::string_view& inPath) const
{
	for (const auto& folder : _listIncludes)
	{
		fm::FilePath p(folder.GetPath());
		fm::File f(p.ToSub(std::string(inPath)));
		if (f.Exist())
		{
			return f;
		}
	}
	return std::nullopt;
}


bool ShaderIncluder::Parse(const fm::File& inFile, std::string& outContent) const
{
	std::string& out = outContent;

	bool result = inFile.ReadLineByLine([this, &out, inFile](const std::string_view& line, size_t lineNumber)
	{
			bool r = true;
			if (line.starts_with(_includeKeyword))
			{
				const size_t first = line.find("\"") + 1;
				const size_t last = line.rfind("\"");

				std::string_view path = line.substr(first, last - first);
				auto file = _FindFile(path);
				if (!file.has_value())
				{
					return false;
				}
				else
				{
					//out += "#line " + std::to_string(1) + " " + std::to_string(1) + "\n";
					r = Parse(file.value(), out);
					out += "#line " + std::to_string(lineNumber + 1) + " " + std::to_string(1) + "\n";
					return r;
				}
			}
			out += line;
			out += "\n";
		return r;
	});
	return result;
}