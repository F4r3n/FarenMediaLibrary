#pragma once
#include "Core/FilePath.h"
#include "nlohmann/json_fwd.hpp"
namespace fm
{

	class Texture
	{
	public:
		Texture() = default;
		Texture(const fm::FilePath& inPath);
		const fm::FilePath& GetPath() const { return _path; }

		fm::FilePath _path;
	};
}