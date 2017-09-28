
#include "Resource/ResourcesManager.h"
#include "Rendering/ShaderParser.h"

using namespace fm;
ResourcesManager ResourcesManager::_instance;

ResourcesManager::ResourcesManager() {
}

void ResourcesManager::loadShader(const std::string& name, std::string& vertexCode, std::string& fragementCode) {
    std::shared_ptr<Shader> shader = std::make_shared<Shader>(vertexCode, fragementCode);
    shader->compile();
    shaders[name] = shader;
}

void ResourcesManager::loadShader(const std::string& name, const std::string& path) {
    ShaderParser parser;
    std::tuple<std::string, std::string> partShader = parser.parse(path);
    std::shared_ptr<Shader> shader = std::make_shared<Shader>(std::get<0>(partShader), std::get<1>(partShader));

    shader->compile();
    shaders[name] = shader;
}

void ResourcesManager::loadShader(const std::string& name, std::shared_ptr<Shader> shader) {
    shaders[name] = shader;
}

ResourcesManager::~ResourcesManager() {
}
