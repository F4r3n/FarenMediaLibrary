
#include "Resource/ResourcesManager.h"
#include "Core/application.h"
#include "Rendering/ShaderLibrary.h"
#include "Rendering/material.hpp"
#include "Resource/RFont.h"
using namespace fm;
ResourcesManager ResourcesManager::_instance;

ResourcesManager::ResourcesManager() 
{
}


ResourcesManager::~ResourcesManager() 
{
}


FilePath ResourcesManager::GetFilePathResource(LOCATION inLocation)
{
	switch (inLocation)
	{
	case LOCATION::INTERNAL_LUA_LOCATION:
	{
		FilePath p = GetFilePathResource(INTERNAL_RESOURCES_LOCATION);
		p.Append("lua");
		return p;
	}
	case LOCATION::INTERNAL_FONT_LOCATION:
	{
		FilePath p = GetFilePathResource(INTERNAL_RESOURCES_LOCATION);
		p.Append("fonts");
		return p;
	}
	case LOCATION::INTERNAL_SHADERS_LOCATION:
	{
		FilePath p = GetFilePathResource(INTERNAL_RESOURCES_LOCATION);
		p.Append("shaders");
		return p;
	}
	case LOCATION::INTERNAL_RESOURCES_LOCATION:
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

bool ResourcesManager::LoadShaders()
{
	ShaderLibrary::LoadShaders();
	return true;
}

bool ResourcesManager::CreateMaterials()
{
	{
		fm::Material *defaultMat = new fm::Material("default_material");
		defaultMat->shaderName = "default";
		defaultMat->shader = fm::ResourcesManager::get().getResource<fm::Shader>("default");
		fm::ResourcesManager::get().load<Material>("default_material", defaultMat);
	}

	{
		Material *defaultMat = new fm::Material("default_light_material");
		defaultMat->shaderName = "default_light";
		defaultMat->shader = fm::ResourcesManager::get().getResource<fm::Shader>("default_light");
		fm::ResourcesManager::get().load<Material>("default_light_material", defaultMat);
	}
	return true;
}

bool ResourcesManager::LoadFonts()
{
	fm::FilePath p = fm::ResourcesManager::GetFilePathResource(fm::ResourcesManager::INTERNAL_FONT_LOCATION);
	p.Append("Roboto-Medium.ttf");
	ResourcesManager::get().load<RFont>("dejavu", new RFont(p.GetPath()));
	return true;
}

