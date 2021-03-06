
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
#if _WIN32
#include <shlobj_core.h>
#include <sstream>
#elif __linux__
#include <cstdlib>
#endif

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
		p.SetSystemID(LOCATION::INTERNAL_LUA_LOCATION);
		p.ToSubFolder("lua");
		return p;
	}
	case LOCATION::INTERNAL_FONT_LOCATION:
	{
		FilePath p(GetFilePathResource(LOCATION::INTERNAL_RESOURCES_LOCATION));
		p.SetSystemID(LOCATION::INTERNAL_FONT_LOCATION);
		p.ToSubFolder("fonts");
		return p;
	}
	case LOCATION::INTERNAL_SHADERS_LOCATION:
	{
		FilePath p(GetFilePathResource(LOCATION::INTERNAL_RESOURCES_LOCATION));
		p.SetSystemID(LOCATION::INTERNAL_SHADERS_LOCATION);
		p.ToSubFolder("shaders");
		return p;
	}
	case LOCATION::INTERNAL_RESOURCES_LOCATION:
	{
		FilePath p(FilePath::GetWorkingDirectory());
		p.SetSystemID(LOCATION::INTERNAL_RESOURCES_LOCATION);
		p.ToSubFolder("Resources");
		return p;
	}
	case LOCATION::INTERNAL_MATERIALS_LOCATION:
	{
		FilePath p(GetFilePathResource(LOCATION::INTERNAL_RESOURCES_LOCATION));
		p.SetSystemID(LOCATION::INTERNAL_MATERIALS_LOCATION);
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
		p.SetSystemID(LOCATION::USER_RESOURCES_LOCATION);
		p.ToSubFolder("Resources");
		return p;
	}
	case LOCATION::USER_LUA_LOCATION:
	{
		FilePath p(GetFilePathResource(LOCATION::USER_RESOURCES_LOCATION));
		p.SetSystemID(LOCATION::USER_LUA_LOCATION);
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
		p.SetSystemID(LOCATION::USER_SETTINGS);
		p.ToSubFolder("Settings");
		return p;
	}
	case LOCATION::SETTINGS:
	{
#if _WIN32
		wchar_t* path = 0;

		SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path);
		std::wstringstream ss;
		ss << path << L"\\FML\\";
		CoTaskMemFree(static_cast<void*>(path));
		std::filesystem::path p(ss.str());

		return FilePath(p.u8string());
#elif __linux__
		return FilePath(std::string(getenv("HOME")) + std::string(".config/FML/"));
#endif
	}
	case LOCATION::SETTINGS_LAST_PROJECTS:
	{
		FilePath p(GetFilePathResource(LOCATION::SETTINGS));
		p.ToSubFile("lastProjects.json");
		return p;
	}
	default:
		return FilePath::GetWorkingDirectory();
	}
}

void ResourcesManager::Reload(bool force)
{
	for (size_t i = 0; i < RESOURCE_TYPE::LAST_RESOURCE; ++i)
	{
		auto rKind = resources[i];
		for (auto&& r : rKind)
		{
			r.second->Reload(force);
		}
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

bool ResourcesManager::LoadMaterials()
{
	_LoadInternalMaterials();
	return true;
}


void ResourcesManager::_LoadInternalMaterials()
{
	Folder materials(Folder(GetFilePathResource(fm::LOCATION::INTERNAL_MATERIALS_LOCATION)));

	materials.Iterate(true, [](const fm::Folder* inFolder, const fm::File* inFile)
		{
			if (inFile != nullptr)
			{
				if (inFile->GetPath().GetExtension() == ".material")
				{
					fm::Material* mat = new fm::Material(inFile->GetPath());
					mat->Load();
					fm::ResourcesManager::get().load<Material>(mat->GetName(), mat);
				}
			}
		});
}

bool ResourcesManager::LoadFonts()
{
	fm::FilePath p = fm::ResourcesManager::GetFilePathResource(fm::LOCATION::INTERNAL_FONT_LOCATION);
	p.ToSubFile("Roboto-Medium.ttf");
	ResourcesManager::get().load<RFont>("dejavu", new RFont(p.GetPath()));
	return true;
}

