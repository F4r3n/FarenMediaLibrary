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

void PostProcessor::setShader(const std::string &newFrag) {

	simple_fragement = "#version 330 core\n"
		+std::string("in vec2 TexCoords;\n")
		+std::string("out vec4 color;\n")
		+std::string("uniform sampler2D screenTexture;")
		
			+ newFrag
		+std::string("void main(){\n")
		+std::string("color = texture(screenTexture, TexCoords);\n")
		+std::string("color = effect(color, TexCoords);")
		+std::string("}");


	ResourcesManager::loadShader("simple", simple_vertex, simple_fragement);
}