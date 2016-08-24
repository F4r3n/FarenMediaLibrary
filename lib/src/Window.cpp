
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
	
	//glEnable (GL_BLEND);
	std::string default_vertex = "#version 330 core\n"
		+ std::string("layout(location = 0) in vec2 position;\n")
		+ std::string("layout(location = 1) in vec3 color;\n")
		+ std::string("uniform mat4 view;\n")
		+ std::string("uniform mat4 model;\n")
		+ std::string("uniform mat4 projection;\n")
		+ std::string("out vec3 ourColor;\n")
		+ std::string("void main(){\n")
		+ std::string("gl_Position = projection*view*model*vec4(position, 0.0f, 1.0f);\n")
		+ std::string("ourColor = color;}");


	std::string default_fragement = "#version 330 core\n"
		+ std::string("in vec3 ourColor;\n")
		+ std::string("out vec4 color;\n")
		+ std::string("void main(){\n")
		+ std::string("color = vec4(ourColor, 1.0f);\n")
		+ std::string("}");

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


	std::string default_fragement_sprite = "#version 330 core\n"
		"in vec3 ourColor;\n"
		"in vec2 ourTexCoord;\n"
		"uniform sampler2D texture2d;\n"
		"out vec4 color;\n"
		"void main(){\n"
		"color = texture(texture2d, ourTexCoord)*vec4(ourColor,1.0f);\n"
		"if(color.z == 0.0f) discard;\n"
		"}";

		//std::string test = "R"


	std::string default_vertex_particle = "#version 330 core\n"
		+ std::string("layout (location = 0) in vec4 vertex;\n")
		+ std::string("uniform mat4 view;\n")
		+ std::string("uniform mat4 projection;\n")
		+ std::string("uniform float scale;\n")
		+ std::string("uniform vec2 offset;\n")
		+ std::string("uniform vec4 particleColor;\n")

		+ std::string("out vec4 ourColor;\n")
		+ std::string("out vec2 ourTexCoord;\n")
		+ std::string("void main(){\n")
		+ std::string("gl_Position = projection*view*vec4((vertex.xy*scale) + offset, 0.0f, 1.0f);\n")
		+ std::string("ourTexCoord = vertex.zw;")
		+ std::string("ourColor = particleColor;}");


	std::string default_fragement_particle = "#version 330 core\n"
		+ std::string("in vec4 ourColor;\n")
		+ std::string("in vec2 ourTexCoord;\n")
		+ std::string("uniform sampler2D texture2d;\n")
		+ std::string("out vec4 color;\n")
		+ std::string("void main(){\n")
		+ std::string("color = texture(texture2d, ourTexCoord)*ourColor;\n")
		+ std::string("}");

	ResourcesManager::loadShader("default", default_vertex, default_fragement);
	ResourcesManager::loadShader("sprite", default_vertex_sprite, default_fragement_sprite);
	ResourcesManager::loadShader("particle", default_vertex_particle, default_fragement_particle);

	camera = Camera(width, height);
}

void Window::update(float fps) {
	events();
	frameLimit(fps);
	clear();
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
	glfwSwapBuffers(window);
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
	glClear(GL_COLOR_BUFFER_BIT);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void Window::draw(Shape &shape) {
	Shader s = ResourcesManager::getShader("default");
	s.Use();
	GLint projLoc = glGetUniformLocation(s.Program, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera.getProjection()));
	GLint view = glGetUniformLocation(s.Program, "view");
	glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(glm::mat4()));
	shape.draw(s);
}
