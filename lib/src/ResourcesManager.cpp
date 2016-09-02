
#include "ResourcesManager.h"

using namespace fm;
std::map<std::string, Shader> ResourcesManager::shaders;
std::map<std::string, Texture> ResourcesManager::textures;
ResourcesManager::ResourcesManager()
{
}


void ResourcesManager::loadShader(const std::string &name, std::string &vertexCode, std::string &fragementCode) {
	Shader shader(vertexCode, fragementCode);
	shaders[name] = shader;
}

Shader& ResourcesManager::getShader(const std::string &name) {
	if(shaders.find(name) != shaders.end())
		return shaders[name];
	//return nullptr;
}

ResourcesManager::~ResourcesManager()
{
}
