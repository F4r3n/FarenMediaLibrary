
#include "Resource/ResourcesManager.h"
#include "Core/application.h"

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
		FilePath p = GetFilePathResource(RESOURCES_LOCATION);
		p.Append("lua");
		return p;
	}
	case LOCATION::FONT_LOCATION:
	{
		FilePath p = GetFilePathResource(RESOURCES_LOCATION);
		p.Append("fonts");
		return p;
	}
	case LOCATION::RESOURCES_LOCATION:
	{
		FilePath p = FilePath::GetWorkingDirectory();
		p.Append("Resources");
		return p;
	}

	case LOCATION::USER_LOCATION:
	{
		return fm::Application::Get().GetUserDirectory();
	}

	case USER_RESOURCES_LOCATION:
	{
		FilePath p = GetFilePathResource(USER_LOCATION);
		p.Append("Resources");
		return p;
	}
	case USER_LUA_LOCATION:
	{
		FilePath p = GetFilePathResource(USER_RESOURCES_LOCATION);
		p.Append("lua");
		return p;
	}
		
	default:
		return FilePath::GetWorkingDirectory();
	}
}

