#include "Resource/Resource.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include "Core/FilePath.h"

using namespace fm;


Resource::Resource(const fm::FilePath& inFilePath)
	:_path(inFilePath)
{
}

bool Resource::NeedReload() const
{
	return _timeStamp != GetTimeStamp().time_since_epoch().count();
}


void Resource::Load(const nlohmann::json& inJSON)
{
	if (inJSON.is_null())
		return;

	_path = fm::FilePath(std::string(inJSON["deps"]["path"]));
	if (inJSON["deps"].contains("timeStamp"))
	{
		_timeStamp = inJSON["deps"]["timeStamp"];
	}
	else
	{
		_timeStamp = 0;
	}
}
void Resource::Save(nlohmann::json& outJSON) const
{
	outJSON["deps"]["path"] = fm::FileSystem::ConvertPathToFileSystem(_path);
	outJSON["deps"]["timeStamp"] = GetTimeStamp().time_since_epoch().count();

}

std::filesystem::file_time_type Resource::GetTimeStamp() const
{
	return fm::File(_path).GetTimeStamp();
}
