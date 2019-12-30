
#include "Resource/ResourcesManager.h"
#include "Core/application.h"
#include "Rendering/material.hpp"
#include "Resource/RFont.h"
#include <filesystem>

#include <string>
#include <fstream>
#include <streambuf>
#include <unordered_set>
#include "Rendering/Shader.h"

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
		FilePath p(GetFilePathResource(LOCATION::INTERNAL_RESOURCES_LOCATION));
		p.ToSubFolder("lua");
		return p;
	}
	case LOCATION::INTERNAL_FONT_LOCATION:
	{
		FilePath p(GetFilePathResource(LOCATION::INTERNAL_RESOURCES_LOCATION));
		p.ToSubFolder("fonts");
		return p;
	}
	case LOCATION::INTERNAL_SHADERS_LOCATION:
	{
		FilePath p(GetFilePathResource(LOCATION::INTERNAL_RESOURCES_LOCATION));
		p.ToSubFolder("shaders");
		return p;
	}
	case LOCATION::INTERNAL_RESOURCES_LOCATION:
	{
		FilePath p(FilePath::GetWorkingDirectory());
		p.ToSubFolder("Resources");
		return p;
	}
	case LOCATION::INTERNAL_MATERIALS:
	{
		FilePath p(GetFilePathResource(LOCATION::INTERNAL_RESOURCES_LOCATION));
		p.ToSubFolder("materials");
		return p;
	}
	case LOCATION::USER_LOCATION:
	{
		return fm::Application::Get().GetUserDirectory().GetPath();
	}

	case LOCATION::USER_RESOURCES_LOCATION:
	{
		FilePath p(GetFilePathResource(LOCATION::USER_LOCATION));
		p.ToSubFolder("Resources");
		return p;
	}
	case LOCATION::USER_LUA_LOCATION:
	{
		FilePath p(GetFilePathResource(LOCATION::USER_RESOURCES_LOCATION));
		p.ToSubFolder("lua");
		return p;
	}

	case LOCATION::WORKING_DIRECTORY:
	{
		return FilePath::GetWorkingDirectory();
	}
	case LOCATION::USER_SETTINGS:
	{
		FilePath p(GetFilePathResource(LOCATION::USER_LOCATION));
		p.ToSubFolder("Settings");
		return p;
	}

	default:
		return FilePath::GetWorkingDirectory();
	}
}

void ResourcesManager::_LoadInternalShaders()
{

	Folder shaders(Folder(GetFilePathResource(fm::LOCATION::INTERNAL_SHADERS_LOCATION)));

	shaders.Iterate(false, [](const fm::Folder* inFolder, const fm::File* inFile)
	{
		if (inFolder != nullptr)
		{
			if (inFolder->GetPath().GetExtension() == ".shader")
			{
				const std::string name = inFolder->GetPath().GetName(true);
				Shader* shader = new Shader(inFolder->GetPath(), name);
				shader->compile();
				fm::ResourcesManager::get().load<fm::Shader>(name, shader);
			}
		}
	});

}



bool ResourcesManager::LoadShaders()
{
	_LoadInternalShaders();
	return true;
}

bool ResourcesManager::CreateMaterials()
{
	{
		fm::Material *defaultMat = new fm::Material(fm::FilePath(fm::LOCATION::INTERNAL_MATERIALS, "default_material"), fm::ResourcesManager::get().getResource<fm::Shader>("default"));
		fm::ResourcesManager::get().load<Material>("default_material", defaultMat);
	}

	{
		Material *defaultMat = new fm::Material(fm::FilePath(fm::LOCATION::INTERNAL_MATERIALS, "default_light_material"), fm::ResourcesManager::get().getResource<fm::Shader>("default_light"));
		fm::ResourcesManager::get().load<Material>("default_light_material", defaultMat);
	}
	return true;
}

bool ResourcesManager::LoadFonts()
{
	fm::FilePath p = fm::ResourcesManager::GetFilePathResource(fm::LOCATION::INTERNAL_FONT_LOCATION);
	p.ToSubFile("Roboto-Medium.ttf");
	ResourcesManager::get().load<RFont>("dejavu", new RFont(p.GetPath()));
	return true;
}

