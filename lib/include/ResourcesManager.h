#pragma once
#include "Shader.h"
#include "Texture.h"
#include <map>
#include "NonCopyable.h"
namespace fm {



	class ResourcesManager : protected fm_system::NonCopyable
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

