#include "stdafx.h"
#include "ResourcesManager.h"
#include "SOIL.h"
using namespace fm;
std::map<std::string, Shader> ResourcesManager::shaders;
std::map<std::string, Texture2D> ResourcesManager::textures;
ResourcesManager::ResourcesManager()
{
}

void ResourcesManager::loadTexture(const std::string &name, const std::string &path) {
	Texture2D texture;
	texture.path = path;
	texture.name = name;

	texture.image = SOIL_load_image(texture.path.c_str(), &texture.width, &texture.height, 0, SOIL_LOAD_RGB);
	//std::cout << "Texture " << texture.path << " Loaded " << texture.width << " " << texture.height << std::endl;
	textures[name] = texture;
}

Texture2D ResourcesManager::getTexture(const std::string &name) {
	if (textures.find(name) != textures.end())
		return textures[name];
	
}

void ResourcesManager::loadShader(const std::string &name, std::string &vertexCode, std::string &fragementCode) {
	Shader shader(vertexCode, fragementCode);
	shaders[name] = shader;
}

Shader ResourcesManager::getShader(const std::string &name) {
	if(shaders.find(name) != shaders.end())
		return shaders[name];
}

ResourcesManager::~ResourcesManager()
{
}
