#pragma once
#include <string>
namespace fm
{
	enum class LOCATION
	{
		INTERNAL_LUA_LOCATION,
		INTERNAL_RESOURCES_LOCATION,
		INTERNAL_FONT_LOCATION,
		INTERNAL_SHADERS_LOCATION,
		INTERNAL_MATERIALS,
		USER_LOCATION,
		USER_RESOURCES_LOCATION,
		USER_LUA_LOCATION,
		USER_SETTINGS,
		WORKING_DIRECTORY
	};

	class FileSystem
	{
	public:
		static std::string ConvertFileSystemToPath(const std::string& inPath);
	};

}