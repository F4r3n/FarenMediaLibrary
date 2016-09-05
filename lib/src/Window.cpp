
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
								"in vec2 TexCoords;\n"
								"out vec4 color;\n"
								"uniform sampler2D text;\n"
								"uniform vec3 textColor;\n"
								"void main(){\n"
								"vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
								"color = vec4(textColor, 1.0) * sampled;}";

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
	std::cout << instancing_vertex << std::endl;				

	std::string simple_vertex = "#version 330 core\n"
		"layout(location = 0) in vec2 position;\n"
		"layout(location = 1) in vec2 texCoords;\n"
		"out vec2 TexCoords;\n"
		"void main(){\n"
		"gl_Position = vec4(position, 0.0f, 1.0f);\n"
		"TexCoords = texCoords;}";

	std::string simple_fragment = "#version 330 core\n"
		"in vec2 TexCoords;\n"
		"out vec4 color;\n"
		"uniform vec2 screenSize;\n"
		"uniform sampler2D screenTexture;"
		"void main(){\n"
		"color = texture(screenTexture, TexCoords);\n"
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
		"in vec3 ourColor;\n"
		"out vec4 color;\n"
		"void main(){\n"
		"color = vec4(ourColor, 1.0f);\n"
		"}";

	std::string default_vertex_sprite = "#version 330 core\n"
		"layout(location = 0) in vec2 position;\n"
		"layout(location = 1) in vec3 color;\n"
		"uniform mat4 view;\n"
		"uniform mat4 model;\n"
		"uniform mat4 projection;\n"
		"out vec3 ourColor;\n"
		"out vec2 ourTexCoord;\n"
		"void main(){\n"
		"gl_Position = projection*view*model*vec4(position, 0.0f, 1.0f);\n"
		"ourTexCoord = position;"
		"ourColor = color;}";


	std::string default_fragment_sprite = "#version 330 core\n"
		"in vec3 ourColor;\n"
		"in vec2 ourTexCoord;\n"
		"uniform sampler2D texture2d;\n"
		"out vec4 color;\n"
		"void main(){\n"
		"color = texture(texture2d, ourTexCoord)*vec4(ourColor,1.0f);\n"
		"if(color.z == 0.0f) discard;\n"
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
		"out vec4 color;\n"
		"void main(){\n"
		"color = texture(texture2d, ourTexCoord)*ourColor;\n"
		"}";

	ResourcesManager::loadShader("text", text_vertex, text_fragment);
	ResourcesManager::loadShader("instancing", instancing_vertex, instancing_fragment);
	ResourcesManager::loadShader("default", default_vertex, default_fragment);
	ResourcesManager::loadShader("simple", simple_vertex, simple_fragment);
	ResourcesManager::loadShader("sprite", default_vertex_sprite, default_fragment_sprite);
	ResourcesManager::loadShader("particle", default_vertex_particle, default_fragment_particle);

}

void Window::update(float fps) {

	events();
	frameLimit(fps);
	
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	clear();
}

void Window::initFrameBuffer() {
	 // Framebuffers
    
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);  
    // Create a color attachment texture
    textureColorbuffer = generateAttachmentTexture(false, false);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // Use a single renderbuffer object for both a depth AND stencil buffer.
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // Now actually attach it
    // Now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int Window::generateAttachmentTexture(bool depth, bool stencil) {

    GLenum attachment_type;
    if(!depth && !stencil)
        attachment_type = GL_RGB;
    else if(depth && !stencil)
        attachment_type = GL_DEPTH_COMPONENT;
    else if(!depth && stencil)
        attachment_type = GL_STENCIL_INDEX;

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    if(!depth && !stencil)
        glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, width, height, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
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
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	postProcess();

	glfwSwapBuffers(window);
}

void Window::postProcess() {
	Shader s = ResourcesManager::getShader("simple");
	s.Use()->setVector2f("screenSize", glm::vec2(width, height));
	glBindVertexArray(quadVAO);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// Use the color attachment texture as the texture of the quad plane
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

	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return 1;
}

void Window::clear() {
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	
}

void Window::draw(Shape &shape) {
	Shader s = ResourcesManager::getShader("default");
	s.Use()->setMatrix("projection", camera.getProjection())->setMatrix("view", glm::mat4());
	shape.draw(s);
}
