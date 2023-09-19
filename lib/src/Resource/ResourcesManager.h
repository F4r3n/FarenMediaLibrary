#pragma once
#include "Resource.h"
#include <map>
#include <array>
#include <memory>
#include "NonCopyable.h"
#include "Core/FilePath.h"
#include "Resource/FileSystem.h"
#include "Rendering/GraphicsAPI.h"
#include "ResourceLoader.h"
namespace fm {
  
class ResourcesManager : protected fm_system::NonCopyable 
{
	typedef std::map<std::string, std::shared_ptr<Resource>> MapOfResources;
	typedef std::array<MapOfResources, RESOURCE_TYPE::LAST_RESOURCE> ArrayOfResources;
public:


    ResourcesManager();
    ~ResourcesManager();

    template <typename T> std::shared_ptr<T> getResource(const std::string& name)
	{
		MapOfResources::const_iterator it = resources[T::getType()].find(name);
        if(it == resources[T::getType()].cend()) 
			return nullptr;

        return std::dynamic_pointer_cast<T>(it->second);
    }

    template <typename T> bool Exists(const std::string& name)
	{
        return resources[T::getType()].find(name) != resources[T::getType()].end();
    }

    template <typename T>
    void load(const std::string& name, std::shared_ptr<Resource> resource) 
	{
        if(T::getType() > RESOURCE_TYPE::LAST_RESOURCE) return;
        resources[T::getType()].insert(std::pair<std::string, std::shared_ptr<Resource>>(name, resource));
    }

	void load(const std::string& name, std::shared_ptr<Resource> resource)
	{
		if (resource->GetType() > RESOURCE_TYPE::LAST_RESOURCE) return;
		resources[resource->GetType()].insert(std::pair<std::string, std::shared_ptr<Resource>>(name, resource));
	}
    
    template <typename T>
    std::map<std::string, std::shared_ptr<Resource> >& getAll()
	{
        return resources[T::getType()];
    }

	static FilePath GetFilePathResource(LOCATION inLocation);


	bool LoadShaders(GRAPHIC_API inAPI);
	bool LoadMaterials();
	bool LoadFonts();

	void Reload(bool force = false);
	static ResourcesManager& get() {
        return _instance;
    }

private:
	void _LoadInternalShaders(GRAPHIC_API inAPI);
	void _LoadInternalMaterials();

	ArrayOfResources resources;
    static ResourcesManager _instance;

	ResourceLoader	_loader;
};
}
