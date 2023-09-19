#include "ResourceImporter.h"
#include "Core/FilePath.h"
#include <nlohmann/json.hpp>
#include "Resource/ResourcesManager.h"
#include "Rendering/Image.h"
#include "Rendering/material.hpp"

using namespace fm;

template class ResourceObjectImporter<fm::Image>;
template class ResourceObjectImporter<fm::Material>;

template <typename T>
ResourceObjectImporter<T>::ResourceObjectImporter(const std::vector<std::string>& inListExtensions, bool isImportNeeded)
	:_listExtensions(inListExtensions), _isImportNeeded(isImportNeeded)
{
}

template <typename T>
bool ResourceObjectImporter<T>::IsValidFileExtension(const std::string& inExtension)
{
	for (const auto& extension : _listExtensions)
	{
		if (extension == inExtension)
			return true;
	}
	return false;
}


template <typename T>
std::shared_ptr<Resource> ResourceObjectImporter<T>::SaveImport(const fm::FilePath& inPath, bool inForce)
{
	if (!IsValidFileExtension(inPath.GetExtension()))
		return nullptr;

	if (!_isImportNeeded)
		return nullptr;

	std::shared_ptr<T> imagePtr = fm::ResourcesManager::get().getResource<T>(inPath.GetName(false));
	if (imagePtr == nullptr)
	{
		imagePtr = std::make_shared<T>(inPath);
	}
	fm::FilePath path(inPath.GetPathString() + ".import");
	fm::File newImportFile(path);

	if (!inForce && newImportFile.Exist())
	{
		return imagePtr;
	}

	nlohmann::json object;
	if (imagePtr != nullptr)
	{
		imagePtr->Save(object);
	}
	newImportFile.SetContent(object.dump());

	return imagePtr;
}


template <typename T>
std::shared_ptr<Resource> ResourceObjectImporter<T>::Load(const fm::FilePath& inPath, bool inRegister)
{
	fm::FilePath path = inPath;
	path.AddExtension(".import");

	if (!IsValidFileExtension(inPath.GetExtension()))
		return nullptr;

	if (_isImportNeeded && !fm::File(path).Exist())
		return nullptr;

	std::shared_ptr<T> data = fm::ResourcesManager::get().getResource<T>(inPath.GetName(!_isImportNeeded));
	if (inRegister)
	{
		if (data == nullptr)
		{
			nlohmann::json object;
			std::string content = fm::File(inPath).GetContent();
			object = object.parse(content);
			data = std::make_shared<T>(inPath);
			data->Load(object);
		}
		fm::ResourcesManager::get().load<T>(inPath.GetName(!_isImportNeeded), data);
	}

	return data;
}
