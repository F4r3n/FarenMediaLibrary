
#include "Resource/ResourcesManager.h"


using namespace fm;
ResourcesManager ResourcesManager::_instance;

ResourcesManager::ResourcesManager() {
}


ResourcesManager::~ResourcesManager() {
}


FilePath ResourcesManager::GetFilePathResource(LOCATION inLocation)
{
	switch (inLocation)
	{
	case LOCATION::INTERNAL_LUA_LOCATION:
	{
		FilePath p = FilePath::GetWorkingDirectory();
		p.Append("Resources");
		p.Append("lua");
		return p;
	}
	case LOCATION::FONT_LOCATION:
	{
		FilePath p = FilePath::GetWorkingDirectory();
		p.Append("Resources");
		p.Append("fonts");
		return p;
	}
	case LOCATION::RESOURCES_LOCATION:
	{
		FilePath p = FilePath::GetWorkingDirectory();
		p.Append("Resources");
		return p;
	}
	default:
		return FilePath::GetWorkingDirectory();
	}
}

