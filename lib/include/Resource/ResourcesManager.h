#pragma once
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include <unordered_map>
#include <array>
#include <memory>
#include "NonCopyable.h"
#include "Components/CMesh.h"
namespace fm {
 class MeshData{
    public:
     MeshData() {}
        MeshData(unsigned int VAO, unsigned int VBO, unsigned int EBO, unsigned int size) {
            this->VAO = VAO;
            this->VBO = VBO;
            this->EBO = EBO;
            this->size = size;
        }
        ~MeshData() {
            if(VAO != 0) {
                glDeleteVertexArrays(1, &VAO);
                glDeleteBuffers(1, &VBO);
                glDeleteBuffers(1, &EBO);
            }
        }
        unsigned int VAO, VBO, EBO, size;
    };
    
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
    
    template <typename T> void load(const std::string &name, std::unique_ptr<T> resource) {
        resources[T::type][name] = std::move(resource);
    }

    void loadShader(const std::string& name, std::string& vertexCode, std::string& fragementCode);
    void loadShader(const std::string& name, std::shared_ptr<Shader> shader);

    static ResourcesManager& get() {
        return _instance;
    }
    
    int registerMesh(MeshData* m) {
        meshes.push_back(std::unique_ptr<MeshData>(m));
        return meshes.size() - 1;
    }
    
    MeshData* getMeshData(size_t id) {
        if(meshes.size() <= id ) return nullptr;
        return meshes[id].get();
    }

private:
    std::array<std::unordered_map<std::string, std::shared_ptr<Resource> >, RESOURCE_TYPE::LAST_RESOURCE> resources;
    std::unordered_map<std::string, std::shared_ptr<Shader> > shaders;
    std::vector<std::unique_ptr<MeshData>> meshes;

    static ResourcesManager _instance;

    // Shader not a resource to avoid dynamic_cast, because it's called lot more than other resources
};
}
