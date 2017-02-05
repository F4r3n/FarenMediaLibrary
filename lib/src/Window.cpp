
#include "Window.h"
#include "Time.h"
#include <chrono>
#include <thread>
#include "Renderer.h"
#include "InputManager.h"
#include "ResourcesManager.h"
#include "Components/CMesh.h"
#include "RFont.h"
#include "ShaderLibrary.h"
using namespace fm;
int Window::width;
int Window::height;

int Window::x = 0;
int Window::y = 0;
Window::Window(int width, int height, const std::string& name) {
    this->nameWindow = name;
    Window::width = width;
    Window::height = height;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Create a GLFWwindow object that we can use for GLFW's functions
    window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    init(window);

    createShaders();
    ResourcesManager::get().load("dejavu", std::make_unique<RFont>("assets/fonts/dejavu/DejaVuSansMono.ttf"));

    // glEnable (GL_BLEND);
    glfwSetWindowSizeCallback(window, window_size_callback);
    fm::InputManager::getInstance().init(this->window);
}

void Window::setMSAA(int value) {
    glfwWindowHint(GLFW_SAMPLES, value);
    glEnable(GL_MULTISAMPLE);  
}

void Window::setName(const std::string &name) {
    this->nameWindow = name;
    glfwSetWindowTitle(window, name.c_str());
}

void Window::createShaders() {
    //Create, load and set textures shader
    ShaderLibrary::loadShaders();


    // Initialize all the shapes
    for(int i = 0; i < fmc::SHAPE::LAST_SHAPE; ++i) {
        fmc::CMesh rect;
        rect.setShape(i);
    }
}


void Window::update(float fps) {

    events();
    this->fpsMax = fps;
    wait_time = 1.0f / (float)fpsMax;
    frameLimit(fps);
}

void Window::frameLimit(unsigned short fps) {

    curr_frame_time = glfwGetTime() - frame_start;
    double dur = 1000.0 * (wait_time - curr_frame_time) + 0.5;
    if(dur > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds((int)dur));
    }

    double frame_end = glfwGetTime();
    Time::dt = frame_end - frame_start;
    Time::timeStamp += Time::dt;
    frame_start = frame_end;
}


void Window::swapBuffers() {
    //At the end of the current frame check if an error occured
    errorDisplay();
    glfwSwapBuffers(window);
}

void Window::errorDisplay() {
    int error = glGetError();
    if(error != 0) {
        std::cerr << "ERROR OPENGL " << error << std::endl;
        exit(-1);
    }
}

bool Window::isClosed() {
    return glfwWindowShouldClose(window);
}

void Window::events() {
    glfwPollEvents();
}

Window::~Window() {
    glfwTerminate();
}

int Window::init(GLFWwindow* window) {
    if(window == nullptr) {

        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK) {
        return -1;
    }
    glGetError();
    errorDisplay();

    glViewport(x, y, width, height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glCullFace( GL_FRONT_AND_BACK);
    return 1;
}


