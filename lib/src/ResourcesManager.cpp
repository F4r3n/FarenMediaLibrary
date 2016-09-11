
#include "ResourcesManager.h"

using namespace fm;
std::array<std::unordered_map<std::string, std::shared_ptr<Resource>>, RESOURCE_TYPE::LAST_RESOURCE> ResourcesManager::resources;
std::unordered_map<std::string, std::shared_ptr<Shader>> ResourcesManager::shaders;
ResourcesManager::ResourcesManager()
{
}


void ResourcesManager::loadShader(const std::string &name, std::string &vertexCode, std::string &fragementCode) {
	std::shared_ptr<Shader> shader = std::make_shared<Shader>(vertexCode, fragementCode);
	shader->compile();
	shaders[name] = shader;
}

void ResourcesManager::loadShader(const std::string &name, std::shared_ptr<Shader> shader) {
	shaders[name] = shader;
}

ResourcesManager::~ResourcesManager()
{
}
