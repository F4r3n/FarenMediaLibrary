#include "Resource/Resource.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include "Core/FilePath.h"

using namespace fm;


Resource::Resource(const fm::FilePath& inFilePath)
	:_path(inFilePath)
{
}


void Resource::Load(const nlohmann::json& inJSON)
{
	_path = fm::FilePath(std::string(inJSON["deps"]["path"]));
}
void Resource::Save(nlohmann::json& outJSON) const
{
	outJSON["deps"]["path"] = fm::FileSystem::ConvertPathToFileSystem(_path);
}