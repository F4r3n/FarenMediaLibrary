#pragma once
#include "Shader.h"
#include "Texture.h"
#include <unordered_map>
#include <array>
#include <memory>
#include "NonCopyable.h"
namespace fm
{
    

class ResourcesManager : protected fm_system::NonCopyable
{

public:
    ResourcesManager();
    ~ResourcesManager();
    
    static std::shared_ptr<Shader> getShader(const std::string &name) {
        return shaders[name];
    }
    template <typename T>
    static std::shared_ptr<T> getResource(const std::string& name) {
        return std::dynamic_pointer_cast<T>(resources[T::type][name]);
    }

    static void loadShader(const std::string& name, std::string& vertexCode, std::string& fragementCode);
    static void loadShader(const std::string& name,  std::shared_ptr<Shader> shader);

private:
    static std::array<std::unordered_map<std::string, std::shared_ptr<Resource>>, RESOURCE_TYPE::LAST_RESOURCE> resources;
    static std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
    //Shader not a resource to avoid dynamic_cast, because it's called lot more than other resources
};
}
