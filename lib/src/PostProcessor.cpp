#include "PostProcessor.h"
#include "ResourcesManager.h"
using namespace fm;
PostProcessor::PostProcessor() {

	simple_vertex = "#version 330 core\n"
		"layout(location = 0) in vec2 position;\n"
		"layout(location = 1) in vec2 texCoords;\n"
		"out vec2 TexCoords;\n"
		"void main(){\n"
		"gl_Position = vec4(position, 0.0f, 1.0f);\n"
		"TexCoords = texCoords;}";

	simple_fragement = "#version 330 core\n"
		"in vec2 TexCoords;\n"
		"out vec4 color;\n"
		"uniform sampler2D screenTexture;"
		"void main(){\n"
		"color = texture(screenTexture, TexCoords);\n"
		"}";
}

PostProcessor::~PostProcessor() {}

bool PostProcessor::activate(const std::string &name) {
	if(post_shaders.find(name) !=  post_shaders.end()) {
		ResourcesManager::loadShader("simple", post_shaders[name]);
		return true;
	}
	return false;
}

Shader& PostProcessor::getShader(const std::string &name) {
	return post_shaders[name];
}

bool PostProcessor::setShader(const std::string &name, const std::string &newFrag) {

	simple_fragement = "#version 330 core\n"
		+std::string("in vec2 TexCoords;\n")
		+std::string("out vec4 color;\n")
		+std::string("uniform sampler2D screenTexture;")

			+ newFrag
		+std::string("void main(){\n")
		+std::string("color = texture(screenTexture, TexCoords);\n")
		+std::string("color = effect(color, TexCoords);")
		+std::string("}");

	Shader shader(simple_vertex, simple_fragement);
	if(shader.compile()) {
	 	post_shaders[name] = shader;
	 	return true;
	}
	 return false;
}