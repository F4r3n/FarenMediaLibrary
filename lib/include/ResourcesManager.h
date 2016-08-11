#pragma once
#include "Shader.h"
#include "Texture.h"
#include <map>
namespace fm {



	class ResourcesManager
	{
		
	public:


		ResourcesManager();
		~ResourcesManager();
		static Shader& getShader(const std::string &name);
		
		static void loadShader(const std::string &name, std::string &vertexCode, std::string &fragementCode);
	private:
		static std::map<std::string, Shader> shaders;
		static std::map<std::string, Texture> textures;
	};
}

