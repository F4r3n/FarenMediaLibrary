#pragma once
#include "Core/FilePath.h"
#include "nlohmann/json_fwd.hpp"
namespace fm
{

	class Texture
	{
	public:
		Texture() = default;
		Texture(const fm::FilePath& inPath, const std::string& inName);
		const fm::FilePath& GetPath() const { return _path; }
		const std::string& GetName() const { return _name; }

		std::string _name;
		fm::FilePath _path;
	};
}