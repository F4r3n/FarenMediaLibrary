#include "ResourceLoader.h"

#include "Rendering/Image.h"
#include "Rendering/material.hpp"
#include "Rendering/Shader.h"
#include "ResourceImporter.h"

#include "Core/FilePath.h"



using namespace fm;

void ResourceLoader::Init()
{
	//order is important
	_loaders.emplace_back(std::make_shared<fm::ResourceObjectImporter<fm::Shader> >(std::vector<std::string>({ ".shader", ".vkshader" }), false));
	_loaders.emplace_back(std::make_shared<fm::ResourceObjectImporter<fm::Image> >(std::vector<std::string>({".png"}), true));

	//materials use shaders, png, should be loaded last
	_loaders.emplace_back(std::make_shared<fm::ResourceObjectImporter<fm::Material> >(std::vector<std::string>({ ".material" }), false));
}

std::shared_ptr<Resource> ResourceLoader::Load(const fm::FilePath& inPath, bool inRegister)
{
	std::shared_ptr<Resource> resourceLoaded = nullptr;
	for (auto& loader : _loaders)
	{
		resourceLoaded = loader->Load(inPath, inRegister);
		if (resourceLoaded != nullptr)
			break;
	}

	return resourceLoaded;
}

std::shared_ptr<Resource> ResourceLoader::SaveImport(const fm::FilePath& inPath, bool inForce)
{

	const std::string extension = inPath.GetExtension();
	if (extension == ".import")
	{
		return nullptr;
	}

	std::shared_ptr<Resource> resourceLoaded = nullptr;
	for (auto& loader : _loaders)
	{
		resourceLoaded = loader->SaveImport(inPath, inForce);
		if(resourceLoaded != nullptr)
		{
			break;
		}
	}
	return resourceLoaded;
}

ResourceLoader::~ResourceLoader() = default;
