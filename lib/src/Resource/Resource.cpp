#include "Resource/Resource.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include "Core/FilePath.h"
#include "Core/UUID.h"

using namespace fm;

void Resource::GetMetaPath(const fm::FilePath& inFilePath, fm::FilePath& outFilePath) const
{
	outFilePath = fm::FilePath(inFilePath.GetParent());
	std::string&& name = inFilePath.GetName(false);

	name += ".meta";
	outFilePath.ToSubFile(name);
}

Resource::Resource(const fm::FilePath& inFilePath)
	:_path(inFilePath)
{

}


void Resource::CreateMeta(const fm::FilePath& inFilePath)
{

}
void Resource::ReadMeta(const fm::FilePath& inFilePath)
{
}

bool Resource::Serialize(nlohmann::json& ioJson) const
{
	return false;
}

bool Resource::Read(const nlohmann::json& inJSON)
{
	return false;
}

bool Resource::Save()
{
	nlohmann::json j;
	const bool ok = Serialize(j);
	if (ok)
	{
		std::ofstream o(_path.GetPath(), std::ofstream::out);
		o << j << std::endl;
		o.close();
	}
	return ok;
}


bool Resource::Load()
{
	std::ifstream i(_path.GetPath());
	nlohmann::json j;
	i >> j;
	const bool ok = Read(j);
	return ok;
}