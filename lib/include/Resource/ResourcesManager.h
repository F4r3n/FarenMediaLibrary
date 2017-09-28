#pragma once
#include "Resource.h"
#include "Rendering/Texture.h"
#include <unordered_map>
#include <array>
#include <memory>
#include "NonCopyable.h"
#include "Components/CMesh.h"
#include "Rendering/Model.hpp"
namespace fm {
    class Shader;


class ResourcesManager : protected fm_system::NonCopyable {

public:
    ResourcesManager();
    ~ResourcesManager();

    std::shared_ptr<Shader> getShader(const std::string& name) {
        return shaders[name];
    }

    std::unordered_map<std::string, std::shared_ptr<Shader> >& getAllShaders() {
        return shaders;
    }
    template <typename T> std::shared_ptr<T> getResource(const std::string& name) {
        return std::dynamic_pointer_cast<T>(resources[T::type][name]);
    }

    template <typename T> void load(const std::string& name, std::unique_ptr<T> resource) {
        resources[T::type][name] = std::move(resource);
    }

    void loadShader(const std::string& name, std::string& vertexCode, std::string& fragementCode);
    void loadShader(const std::string& name, std::shared_ptr<Shader> shader);
    void loadShader(const std::string& name, const std::string& path);
 static ResourcesManager& get() {
        return _instance;
    }

    int registerMesh(fm::Model* m) {
        meshes.push_back(std::unique_ptr<fm::Model>(m));
        return meshes.size() - 1;
    }

    fm::Model* getMeshData(size_t id) {
        if(meshes.size() <= id)
            return nullptr;
        return meshes[id].get();
    }
    
    bool isGenerated(size_t id) {
        return getMeshData(id)->generated;
    }

private:
    std::array<std::unordered_map<std::string, std::shared_ptr<Resource> >, RESOURCE_TYPE::LAST_RESOURCE> resources;
    std::unordered_map<std::string, std::shared_ptr<Shader> > shaders;
    std::vector<std::unique_ptr<Model> > meshes;

    static ResourcesManager _instance;

    // Shader not a resource to avoid dynamic_cast, because it's called lot more than other resources
};
}
