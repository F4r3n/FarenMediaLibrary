#pragma once
#include "Shader.h"
#include <map>
namespace fm {
	struct Texture2D {
		int width;
		int height;
		std::string name;
		std::string path;

		unsigned char* image;
	};


	class ResourcesManager
	{
	public:
		ResourcesManager();
		~ResourcesManager();
		static Shader getShader(const std::string &name);
		static Texture2D getTexture(const std::string &name);
		
		static void loadShader(const std::string &name, std::string &vertexCode, std::string &fragementCode);
		static void loadTexture(const std::string &name, const std::string &path);
	private:
		static std::map<std::string, Shader> shaders;
		static std::map<std::string, Texture2D> textures;
	};
}

