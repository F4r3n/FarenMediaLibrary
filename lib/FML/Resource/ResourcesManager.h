#pragma once
#include "Resource.h"
#include <map>
#include <array>
#include <memory>
#include "NonCopyable.h"
#include "Resource/FileSystem.h"
#include "Rendering/GraphicsAPI.h"
#include "Core/FilePath.h"

namespace fm {
	class ResourceLoader;
	class ResourcesManager : protected fm_system::NonCopyable
	{
		typedef std::map<std::string, std::shared_ptr<Resource>> MapOfResources;
	public:


		ResourcesManager();
		~ResourcesManager();


		template <typename T> std::shared_ptr<T> getResource(const fm::FilePath& inPath)
		{
			MapOfResources::const_iterator it = resources.find(fm::FileSystem::ConvertPathToFileSystem(inPath));
			if (it == resources.cend())
				return nullptr;

			return std::dynamic_pointer_cast<T>(it->second);
		}

		template <typename T>
		void load(const fm::FilePath& inPath, std::shared_ptr<Resource> resource)
		{
			if (T::getType() > RESOURCE_TYPE::LAST_RESOURCE) return;
			resources.insert(std::pair<std::string, std::shared_ptr<Resource>>(fm::FileSystem::ConvertPathToFileSystem(inPath), resource));
		}

		void load(const fm::FilePath& inPath, std::shared_ptr<Resource> resource)
		{
			if (resource->GetType() > RESOURCE_TYPE::LAST_RESOURCE) return;
			resources.insert(std::pair<std::string, std::shared_ptr<Resource>>(fm::FileSystem::ConvertPathToFileSystem(inPath), resource));
		}

		void load(std::shared_ptr<Resource> resource)
		{
			if (resource->GetType() > RESOURCE_TYPE::LAST_RESOURCE) return;
			resources.insert(std::pair<std::string, std::shared_ptr<Resource>>(fm::FileSystem::ConvertPathToFileSystem(resource->GetPath()), resource));
		}

		void Purge(const fm::FilePath& inPath)
		{

			MapOfResources::iterator it = resources.find(fm::FileSystem::ConvertPathToFileSystem(inPath));
			if (it != resources.cend())
			{
				resources.erase(it);
			}

		}

		void Purge(fm::LOCATION inLocation)
		{

			std::erase_if(resources,
				[inLocation](const auto& item) {
					auto const& [key, value] = item;
					return fm::FilePath(key).GetFileSystemID() == inLocation;
				});

		}

		void PurgeAll()
		{
			resources.clear();
		}

		template <typename T>
		std::map<std::string, std::shared_ptr<Resource> >& getAll()
		{
			return resources;
		}

		static FilePath GetFilePathResource(LOCATION inLocation);


		bool LoadShaders();
		bool LoadMaterials();
		bool LoadFonts();

		static ResourcesManager& get() {
			return _instance;
		}

	private:
		void _LoadInternalShaders();
		void _LoadInternalMaterials();

		MapOfResources resources;
		static ResourcesManager _instance;

		std::unique_ptr<ResourceLoader>	_loader;
	};
}
