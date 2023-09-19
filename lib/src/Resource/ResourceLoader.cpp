#include "ResourceLoader.h"
#include "ResourceImporter.h"
#include "Core/FilePath.h"
#include "Rendering/Image.h"
#include "Rendering/material.hpp"

using namespace fm;

void ResourceLoader::Init()
{
	_loaders.emplace_back(std::make_unique<ResourceObjectImporter<fm::Image>>(std::vector<std::string>({".png"}), true));
	_loaders.emplace_back(std::make_unique<ResourceObjectImporter<fm::Material>>(std::vector<std::string>({ ".material" }), false));

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

