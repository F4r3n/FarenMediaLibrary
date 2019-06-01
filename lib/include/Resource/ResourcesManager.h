#pragma once
#include "Resource.h"
#include <map>
#include <unordered_map>
#include <array>
#include <memory>
#include "NonCopyable.h"
#include "Core/FilePath.h"
#include "Rendering/Shader.h"
#include "Rendering/Model.hpp"

namespace fm {
  
class ResourcesManager : protected fm_system::NonCopyable 
{
	typedef std::map<std::string, Resource* > MapOfResources;
	typedef std::array<MapOfResources, RESOURCE_TYPE::LAST_RESOURCE> ArrayOfResources;
public:

	enum LOCATION
	{
		INTERNAL_LUA_LOCATION,
		INTERNAL_RESOURCES_LOCATION,
		INTERNAL_FONT_LOCATION,
		USER_LOCATION,
		USER_RESOURCES_LOCATION,
		USER_LUA_LOCATION
	};


    ResourcesManager();
    ~ResourcesManager();

    template <typename T> T* getResource(const std::string& name)
	{
		MapOfResources::const_iterator it = resources[T::getType()].find(name);
        if(it == resources[T::getType()].cend()) 
			return nullptr;

        return dynamic_cast<T*>(it->second);
    }

    template <typename T> bool Exists(const std::string& name)
	{
        return resources[T::getType()].find(name) != resources[T::getType()].end();
    }

    template <typename T>
    void load(const std::string& name, Resource* resource) 
	{
        if(T::getType() > RESOURCE_TYPE::LAST_RESOURCE) return;
        resources[T::getType()].insert(std::pair<std::string, Resource*>(name, resource));
    }
    
    template <typename T>
    std::map<std::string, Resource* >& getAll()
	{
        return resources[T::getType()];
    }

	static FilePath GetFilePathResource(LOCATION inLocation);
 static ResourcesManager& get() {
        return _instance;
    }

private:
	ArrayOfResources resources;
    static ResourcesManager _instance;
};
}
