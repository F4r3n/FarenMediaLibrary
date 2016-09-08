
#include "Window.h"
#include "Time.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
using namespace fm;



Window::Window(int width, int height, const std::string &name):width(width), height(height)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
	init(window);

	initFrameBuffer();


	createQuadScreen();
	createShaders();
	//glEnable (GL_BLEND);
	
	camera = Camera(width, height);
	fm::InputManager::getInstance().init(this->window);
}

void Window::createShaders() {
	std::string text_vertex = "#version 330 core\n"
							  "layout (location = 0) in vec4 vertex;\n"
							  "out vec2 TexCoords;\n"
							  "uniform mat4 projection;\n"
							  "void main(){\n"
							  "gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
							  "TexCoords = vertex.zw;}\n";
						
	std::string text_fragment = "#version 330 core\n"
								"layout (location = 0) out vec4 FragColor;\n"
								"layout (location = 1) out vec4 BrightColor;\n"
								"in vec2 TexCoords;\n"
								"uniform sampler2D text;\n"
								"uniform vec3 textColor;\n"
								"uniform float BloomEffect;\n"
								"void main(){\n"
								"BrightColor = vec4(0);FragColor = vec4(0);\n"
								"vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
								"vec4 color = vec4(textColor, 1.0) * sampled;"
								"FragColor = color\n;"
								"float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));\n"
    							"if(brightness >= 0.0 && BloomEffect == 1)\n"
        						"BrightColor = vec4(FragColor.rgb, 1.0);}";

	std::string instancing_vertex = "#version 330 core\n"
							  		"layout (location = 0) in vec2 position;\n"
							  		"layout (location = 1) in vec3 color;\n"
							  		"layout (location = 3) in mat4 instanceMatrix;\n"
							  		"out vec2 TexCoords;\n"
							  		"out vec3 Color;\n"
							  		"uniform mat4 view;\n"
							  		"uniform mat4 projection;\n"
							  		"void main(){\n"
							  		"vec2 colorTest = vec2(1);\n"
							  		"gl_Position = projection*view*instanceMatrix*vec4(position, 0.0f, 1.0f);\n"
							  		"Color = color;\n"
							  		"TexCoords = colorTest;}\n";
						
	std::string instancing_fragment = "#version 330 core\n"
								"in vec2 TexCoords;\n"
								"in vec3 Color;"
								"out vec4 color;\n"
								"void main(){\n"
								"color = vec4(1);}";

	std::string blur_vertex ="";

	std::string blur_fragment="#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec2 TexCoords;\n"
		"\n"
		"uniform sampler2D image;\n"
		"uniform bool horizontal;\n"
		"\n"
		"uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);\n"
		"\n"
		"void main()\n"
		"{             \n"
		"     vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel\n"
		"     vec3 result = texture(image, TexCoords).rgb * weight[0];\n"
		"     if(horizontal)\n"
		"     {\n"
		"         for(int i = 1; i < 5; ++i)\n"
		"         {\n"
		"            result += texture(image, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];\n"
		"            result += texture(image, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];\n"
		"         }\n"
		"     }\n"
		"     else\n"
		"     {\n"
		"         for(int i = 1; i < 5; ++i)\n"
		"         {\n"
		"             result += texture(image, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];\n"
		"             result += texture(image, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];\n"
		"         }\n"
		"     }\n"
		"     FragColor = vec4(result, 1.0);\n"
		"}";
				

	std::string simple_vertex = "#version 330 core\n"
		"layout(location = 0) in vec2 position;\n"
		"layout(location = 1) in vec2 texCoords;\n"
		"out vec2 TexCoords;\n"
		"void main(){\n"
		"gl_Position = vec4(position, 0.0f, 1.0f);\n"
		"TexCoords = texCoords;}";

	std::string simple_fragment = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec2 TexCoords;\n"
		"uniform sampler2D screenTexture;\n"
		"uniform sampler2D bloomBlur;\n"
		"uniform vec2 screenSize;\n"
		"void main(){\n"
		"const float gamma = 2.2;\n"
		"const float exposure = 1;"
    	"vec4 hdrColor = texture(screenTexture, TexCoords);      \n"
    	"vec4 bloomColor = texture(bloomBlur, TexCoords);\n"
    	"hdrColor.rgb += bloomColor.rgb; // additive blending\n"
    	//"vec3 result = vec3(1.0) - exp(-hdrColor.rgb * exposure);\n"
    	//"result = pow(result, vec3(1.0 / gamma));\n"
    	"FragColor = vec4(hdrColor.rgb, 1.0f);\n"
		"}";

	std::string default_vertex = "#version 330 core\n"
		"layout(location = 0) in vec2 position;\n"
		"layout(location = 1) in vec3 color;\n"
		"uniform mat4 view;\n"
		"uniform mat4 model;\n"
		"uniform mat4 projection;\n"
		"out vec3 ourColor;\n"
		"void main(){\n"
		"gl_Position = projection*view*model*vec4(position, 0.0f, 1.0f);\n"
		"ourColor = color;}";

	std::string default_fragment = "#version 330 core\n"
		"layout (location = 0) out vec4 FragColor;\n"
		"layout (location = 1) out vec4 BrightColor;\n"
		"in vec3 ourColor;\n"
		"uniform vec4 mainColor;"
		"uniform float BloomEffect;\n"
		"void main(){\n"
		"vec4 color = mainColor;\n"
		"FragColor = vec4(0); BrightColor = vec4(0);\n"
		"FragColor = color\n;"
		"float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));\n"
    	"if(brightness > 1.0 && BloomEffect == 1)\n"
        "BrightColor = vec4(FragColor.rgb, 1.0);"
		//"if(BloomEffect <= 0) FragColor = color;\n else BrightColor = color;"
		"}";

	std::string default_vertex_sprite = "#version 330 core\n"
		"layout(location = 0) in vec2 position;\n"
		"layout(location = 1) in vec3 color;\n"
		"uniform mat4 view;\n"
		"uniform mat4 model;\n"
		"uniform mat4 projection;\n"
		"uniform float bloomEffect;\n"
		"out vec3 ourColor;\n"
		"out vec2 ourTexCoord;\n"
		"void main(){\n"
		"gl_Position = projection*view*model*vec4(position, 0.0f, 1.0f);\n"
		"ourTexCoord = position;"
		"ourColor = color;}";


	std::string default_fragment_sprite = "#version 330 core\n"
		"layout (location = 0) out vec4 FragColor;\n"
		"layout (location = 1) out vec4 BrightColor;\n"
		"uniform vec3 mainColor;"
		"in vec3 ourColor;\n"
		"in vec2 ourTexCoord;\n"
		"uniform float BloomEffect;"
		"uniform sampler2D texture2d;\n"
		"void main(){\n"
		"vec4 color = texture(texture2d, ourTexCoord)*vec4(ourColor,1.0f);\n"
		"if(color.z == 0.0f) discard;\n"
		"BrightColor = vec4(0);FragColor = vec4(0);\n"
		"if(BloomEffect <= 0) {FragColor = color; }\n"
		"else {BrightColor = color;}\n"
		"}";

	std::string default_vertex_particle = "#version 330 core\n"
		"layout (location = 0) in vec4 vertex;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"uniform float scale;\n"
		"uniform vec2 offset;\n"
		"uniform vec4 particleColor;\n"
		"out vec4 ourColor;\n"
		"out vec2 ourTexCoord;\n"
		"void main(){\n"
		"gl_Position = projection*view*vec4((vertex.xy*scale) + offset, 0.0f, 1.0f);\n"
		"ourTexCoord = vertex.zw;"
		"ourColor = particleColor;}";

	std::string default_fragment_particle = "#version 330 core\n"
		"in vec4 ourColor;\n"
		"in vec2 ourTexCoord;\n"
		"uniform sampler2D texture2d;\n"
		"layout (location = 0) out vec4 FragColor;\n"
		"layout (location = 1) out vec4 BrightColor;\n"
		"void main(){\n"
		"FragColor = texture(texture2d, ourTexCoord)*ourColor;\n"
		"}";

	ResourcesManager::loadShader("blur", simple_vertex, blur_fragment);
	ResourcesManager::loadShader("text", text_vertex, text_fragment);
	ResourcesManager::loadShader("instancing", instancing_vertex, instancing_fragment);
	ResourcesManager::loadShader("default", default_vertex, default_fragment);
	ResourcesManager::loadShader("simple", simple_vertex, simple_fragment);
	ResourcesManager::loadShader("sprite", default_vertex_sprite, default_fragment_sprite);
	ResourcesManager::loadShader("particle", default_vertex_particle, default_fragment_particle);


	Shader s = ResourcesManager::getShader("simple");
	s.Use()->setInt("screenTexture", 0)->setInt("bloomBlur", 1);

}

void Window::update(float fps) {

	events();
	frameLimit(fps);


	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	clear();
}

void Window::initFrameBuffer() {
    
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);  
    // Create a color attachment texture
	glGenTextures(2, textureColorbuffer);
    for(int i = 0; i < 2; i++) {
    	glBindTexture(GL_TEXTURE_2D, textureColorbuffer[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // We clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // attach texture to framebuffer
    	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureColorbuffer[i], 0);
	}

    GLuint rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);     
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Ping pong framebuffer for blurring

    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongColorbuffers);
    for (GLuint i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // We clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
        // Also check if framebuffers are complete (no need for depth buffer)
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "Framebuffer not complete!" << std::endl;
    }
 	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
}

int Window::generateAttachmentTexture(bool depth, bool stencil) {

    GLenum attachment_type;
    if(!depth && !stencil)
        attachment_type = GL_RGB16F;
    else if(depth && !stencil)
        attachment_type = GL_DEPTH_COMPONENT;
    else if(!depth && stencil)
        attachment_type = GL_STENCIL_INDEX;

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    if(!depth && !stencil)
        glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, width, height, 0, attachment_type, GL_FLOAT, NULL);
    else 
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;

}

void Window::createQuadScreen() {
    GLfloat quadVertices[] = {   // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // Positions   // TexCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    GLuint quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glBindVertexArray(0);
}

void Window::frameLimit(unsigned short fps) {
	
	curr_frame_time = glfwGetTime() - frame_start;
	double dur = 1000.0 * ( wait_time - curr_frame_time ) + 0.5;
	if( dur > 0 ) // ensures that we don't have a dur > 0.0 which converts to a durDW of 0.
	{
	    std::this_thread::sleep_for(std::chrono::milliseconds((int)dur));
	}
	
	double frame_end = glfwGetTime();
	Time::dt = frame_end - frame_start;
	// here you could print to file or OutputDebugStream curr_frame_time, frame_end - frame_start and dur.
	frame_start = frame_end;
}

void Window::swapBuffers() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

  	GLboolean horizontal = true, first_iteration = true;
    GLuint amount = 10;
    Shader s = ResourcesManager::getShader("blur");
    s.Use();
    for (GLuint i = 0; i < amount; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
        s.setFloat("horizontal", horizontal);
        glBindTexture(GL_TEXTURE_2D, first_iteration ? textureColorbuffer[1] : pingpongColorbuffers[!horizontal]);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        horizontal = !horizontal;
        if (first_iteration)
            first_iteration = false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

  	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	postProcess(true);
	errorDisplay();
	glfwSwapBuffers(window);
}

void Window::errorDisplay() {
	int error = glGetError();
	if(error != 0) {
		std::cerr <<"ERROR OPENGL " << error << std::endl;
		exit(-1);
	}
}

void Window::postProcess(bool horizontal) {
	Shader s = ResourcesManager::getShader("simple");
	s.Use()->setVector2f("screenSize", glm::vec2(width, height));
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);

	glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

bool Window::isClosed() {
	return glfwWindowShouldClose(window);
}

void Window::events() {
	glfwPollEvents();
}


Window::~Window()
{
	glfwTerminate();
}

int Window::init(GLFWwindow *window) {
	if (window == nullptr)
	{
		
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;

	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		return -1;
	}
	glGetError();
	errorDisplay();


	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    return 1;
}

void Window::clear() {
 	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	
}

void Window::draw(Shape &shape) {
	Shader s = ResourcesManager::getShader("default");
	s.Use()->setMatrix("projection", camera.getProjection())->setMatrix("view", glm::mat4());
	shape.draw(s);
}
