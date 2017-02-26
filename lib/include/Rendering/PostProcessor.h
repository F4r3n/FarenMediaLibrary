#pragma once
#include <string>
#include <map>
#include <memory>
#include "Shader.h"
namespace fm {
class PostProcessor {
	public:
		PostProcessor();
		~PostProcessor();
		bool setShader(const std::string &name, const std::string &newFrag);
		bool loadShader(const std::string &name, const std::string &newFrag);
		bool activate(const std::string &name);
		std::shared_ptr<Shader> getShader(const std::string &name);
		//void setFloat(const std::string &name, float value);
	private:
		std::map<std::string, std::shared_ptr<Shader>> post_shaders;
		std::string simple_vertex;
		std::string simple_fragement;

};
}