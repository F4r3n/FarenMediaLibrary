
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
#elif __linux__ || __APPLE__
#include <cstdlib>
#endif
#include "Rendering/OpenGL/OGLShader.hpp"
#if WITH_VULKAN
#include "Rendering/Vulkan/VkShader.hpp"
#endif
#include "ResourceLoader.h"
using namespace fm;
ResourcesManager ResourcesManager::_instance;

ResourcesManager::ResourcesManager() 
{
	_loader = std::make_unique<fm::ResourceLoader>();
	_loader->Init();
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
		p.ToSub("lua");
		return p;
	}
	case LOCATION::INTERNAL_FONT_LOCATION:
	{
		FilePath p(GetFilePathResource(LOCATION::INTERNAL_RESOURCES_LOCATION));
		p.SetSystemID(LOCATION::INTERNAL_FONT_LOCATION);
		p.ToSub("fonts");
		return p;
	}
	case LOCATION::INTERNAL_SHADERS_LOCATION:
	{
		FilePath p(GetFilePathResource(LOCATION::INTERNAL_RESOURCES_LOCATION));
		p.SetSystemID(LOCATION::INTERNAL_SHADERS_LOCATION);
		p.ToSub("shaders");
		return p;
	}
	case LOCATION::INTERNAL_MODELS_LOCATION:
	{
		FilePath p(GetFilePathResource(LOCATION::INTERNAL_RESOURCES_LOCATION));
		p.SetSystemID(LOCATION::INTERNAL_MODELS_LOCATION);
		p.ToSub("models");
		return p;
	}
	case LOCATION::INTERNAL_RESOURCES_LOCATION:
	{
		fm::FilePath _internal = fm::Application::Get().GetInternalResources().GetPath();
		FilePath p = !_internal.IsValid() ? FilePath::GetWorkingDirectory() : _internal;
		p.SetSystemID(LOCATION::INTERNAL_RESOURCES_LOCATION);
		p.ToSub("_Resources_");
		return p;
	}
	case LOCATION::INTERNAL_IMAGES_LOCATION:
	{
		FilePath p(GetFilePathResource(LOCATION::INTERNAL_RESOURCES_LOCATION));
		p.SetSystemID(LOCATION::INTERNAL_MODELS_LOCATION);
		p.ToSub("images");
		return p;
	}
	case LOCATION::INTERNAL_MATERIALS_LOCATION:
	{
		FilePath p(GetFilePathResource(LOCATION::INTERNAL_RESOURCES_LOCATION));
		p.SetSystemID(LOCATION::INTERNAL_MATERIALS_LOCATION);
		p.ToSub("materials");
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
		p.ToSub("Resources");
		return p;
	}
	case LOCATION::USER_LUA_LOCATION:
	{
		FilePath p(GetFilePathResource(LOCATION::USER_RESOURCES_LOCATION));
		p.SetSystemID(LOCATION::USER_LUA_LOCATION);
		p.ToSub("lua");
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
		p.ToSub("Settings");
		return p;
	}
	case LOCATION::SETTINGS:
	{
#if _WIN32
		wchar_t* path = 0;

		SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path);
		std::wstringstream ss;
		ss << path << L"\\FML";
		CoTaskMemFree(static_cast<void*>(path));
		std::filesystem::path p(ss.str());

		return FilePath(p.u8string());
#elif __linux__ || __APPLE__
		return FilePath(std::string(getenv("HOME")) + std::string(".config/FML"));
#endif
	}
	case LOCATION::SETTINGS_LAST_PROJECTS:
	{
		FilePath p(GetFilePathResource(LOCATION::SETTINGS));
		p.ToSub("lastProjects.json");
		return p;
	}
	default:
		return FilePath::GetWorkingDirectory();
	}
}


void ResourcesManager::_LoadInternalShaders()
{

	Folder shaders(Folder(GetFilePathResource(fm::LOCATION::INTERNAL_SHADERS_LOCATION)));

	shaders.Iterate(false, [this](const fm::Folder* inFolder, const fm::File* inFile)
	{
		if (inFolder != nullptr)
		{
			_loader->Load(inFolder->GetPath(), true);
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

	materials.Iterate(true, [this](const fm::Folder* inFolder, const fm::File* inFile)
		{
			if (inFile != nullptr)
			{
				_loader->Load(inFile->GetPath(), true);
			}
		});
}

bool ResourcesManager::LoadFonts()
{
	fm::FilePath p = fm::ResourcesManager::GetFilePathResource(fm::LOCATION::INTERNAL_FONT_LOCATION);
	p.ToSub("Roboto-Medium.ttf");
	//ResourcesManager::get().load<RFont>("dejavu", std::make_shared<RFont>(p.GetPath()));
	return true;
}

