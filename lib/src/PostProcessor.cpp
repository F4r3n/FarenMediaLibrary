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
		"uniform vec2 screenSize;\n"
		"out vec4 color;\n"
		"uniform sampler2D screenTexture;"
		"void main(){\n"
		"color = texture(screenTexture, TexCoords);\n"
		"}";
}

bool PostProcessor::loadShader(const std::string &name, const std::string &newFrag) {
	 std::ifstream ifs(newFrag);
  	 std::string content( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>()    ) );
  	 return setShader(name, content);

}

PostProcessor::~PostProcessor() {}

bool PostProcessor::activate(const std::string &name) {
	if(post_shaders.find(name) !=  post_shaders.end()) {
		ResourcesManager::loadShader("simple", post_shaders[name]);
		return true;
	}
	return false;
}

std::shared_ptr<Shader> PostProcessor::getShader(const std::string &name) {
	return post_shaders[name];
}

bool PostProcessor::setShader(const std::string &name, const std::string &newFrag) {


	simple_fragement = "#version 330 core\n"
		+ std::string("out vec4 FragColor;\n")
		+ std::string("in vec2 TexCoords;\n")
		+ std::string("uniform sampler2D screenTexture;\n")
		+ std::string("uniform sampler2D bloomBlur;\n")
		+ std::string("uniform vec2 screenSize;\n")
		+ newFrag
		+ std::string("void main(){\n")
		+ std::string("const float gamma = 2.2;\n")
		+ std::string("const float exposure = 1;")
    	+ std::string("vec3 hdrColor = texture(screenTexture, TexCoords).rgb;      \n")
    	+ std::string("vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;\n")
    	+ std::string("hdrColor += bloomColor; // additive blending\n")
    	
    	+ std::string("vec4 color = effect(vec4(hdrColor,1), screenTexture, bloomBlur, TexCoords, screenSize);\n")
    	+ std::string("FragColor = color;\n")
		+ std::string("}");

	std::shared_ptr<Shader> shader = std::make_shared<Shader>(simple_vertex, simple_fragement);
	if(shader->compile()) {
	 	shader->Use()->setInt("screenTexture", 0)->setInt("bloomBlur", 1);
	 	post_shaders[name] = shader;

	 	return true;
	}
	 return false;
}