#pragma once
#include <memory>
#include <vector>
#include <string>
#include "ResourcesManager.h"
#include <nlohmann/json.hpp>
namespace fm
{
	class FilePath;
	class Resource;

	class ResourceImporter
	{
	public:
		virtual std::shared_ptr<Resource> Load(const fm::FilePath &inPath, bool inRegister) = 0;
		virtual std::shared_ptr<Resource> SaveImport(const fm::FilePath &inPath, bool inForce) = 0;
		virtual ~ResourceImporter() = default;
	};

	template <typename T>
	class ResourceObjectImporter : public ResourceImporter
	{
	public:
		ResourceObjectImporter(const std::vector<std::string> &inListExtensions, bool isImportNeeded)
			: _listExtensions(inListExtensions), _isImportNeeded(isImportNeeded)
		{
		}

		std::shared_ptr<Resource> SaveImport(const fm::FilePath &inPath, bool inForce)
		{
			if (!IsValidFileExtension(inPath.GetExtension()))
				return nullptr;

			if (!_isImportNeeded)
				return nullptr;

			std::shared_ptr<T> dataPtr = fm::ResourcesManager::get().getResource<T>(inPath);
			if (dataPtr == nullptr)
			{
				dataPtr = std::make_shared<T>(inPath);
			}
			fm::FilePath path(inPath.GetPathString() + ".import");
			fm::File newImportFile(path);

			if (!inForce && newImportFile.Exist())
			{
				return dataPtr;
			}

			nlohmann::json object;
			if (dataPtr != nullptr)
			{
				dataPtr->Save(object);
			}
			newImportFile.SetContent(object.dump());

			return dataPtr;
		}

		std::shared_ptr<Resource> Load(const fm::FilePath &inPath, bool inRegister)
		{
			fm::FilePath path = inPath;
			path.AddExtension(".import");

			if (!IsValidFileExtension(inPath.GetExtension()))
				return nullptr;

			if (_isImportNeeded && !fm::File(path).Exist())
				return nullptr;

			std::shared_ptr<T> data = fm::ResourcesManager::get().getResource<T>(inPath);
			if (inRegister)
			{
				fm::FilePath localisationData = _isImportNeeded ? path : inPath;
				if (data == nullptr)
				{
					nlohmann::json object;
					fm::File(localisationData).GetJSONContent(object);
					data = std::make_shared<T>(inPath);
					data->Load(object);

					//nlohmann::json object2;
					//data->Save(object2);
					//fm::File(inPath).SetContent(object2.dump());
				}
				fm::ResourcesManager::get().load<T>(inPath, data);
			}

			return data;
		}

		virtual ~ResourceObjectImporter() { ; }

	private:
		bool IsValidFileExtension(const std::string &inExtension)
		{
			for (const auto &extension : _listExtensions)
			{
				if (extension == inExtension)
					return true;
			}
			return false;
		}
		std::vector<std::string> _listExtensions;
		bool _isImportNeeded = true;
	};
}