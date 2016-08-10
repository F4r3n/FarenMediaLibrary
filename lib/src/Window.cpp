#include "stdafx.h"
#include "Window.h"

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
		+ std::string("layout(location = 0) in vec2 position;\n")
		+ std::string("layout(location = 1) in vec3 color;\n")
		+ std::string("uniform mat4 view;\n")
		+ std::string("uniform mat4 model;\n")
		+ std::string("uniform mat4 projection;\n")
		+ std::string("out vec3 ourColor;\n")
		+ std::string("out vec2 ourTexCoord;\n")
		+ std::string("void main(){\n")
		+ std::string("gl_Position = projection*view*model*vec4(position, 0.0f, 1.0f);\n")
		+ std::string("ourTexCoord = position;")
		+ std::string("ourColor = color;}");


	std::string default_fragement_sprite = "#version 330 core\n"
		+ std::string("in vec3 ourColor;\n")
		+ std::string("in vec2 ourTexCoord;\n")
		+ std::string("uniform sampler2D texture;\n")
		+ std::string("out vec4 color;\n")
		+ std::string("void main(){\n")
		+ std::string("color = texture(texture, ourTexCoord)*ourColor;\n")
		+ std::string("}");

	ResourcesManager::loadShader("default", default_vertex, default_fragement);
	ResourcesManager::loadShader("sprite", default_vertex_sprite, default_fragement_sprite);
	camera = Camera(width, height);
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
}

void Window::clear() {
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
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