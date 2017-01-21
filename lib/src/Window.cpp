
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

    createQuadScreen();
    createShaders();
    // glEnable (GL_BLEND);
    glfwSetWindowSizeCallback(window, window_size_callback);
    fm::InputManager::getInstance().init(this->window);
}

void Window::setName(const std::string &name) {
    this->nameWindow = name;
    glfwSetWindowTitle(window, name.c_str());
}

void Window::createShaders() {
    ShaderLibrary::loadShaders();

    std::shared_ptr<Shader> s = ResourcesManager::get().getShader("simple");
    s->Use()->setInt("screenTexture", 0)->setInt("bloomBlur", 1);

    std::shared_ptr<Shader> light = ResourcesManager::get().getShader("light");
    light->Use()->setInt("screenTexture", 0)->setInt("posTexture", 1);

    // Initialize all the shapes
    for(int i = 0; i < fmc::SHAPE::LAST_SHAPE; ++i) {
        fmc::CMesh rect;
        rect.setShape(i);
    }


    ResourcesManager::get().load("dejavu", std::make_unique<RFont>("assets/fonts/dejavu/DejaVuSansMono.ttf"));

}


void Window::update(float fps) {

    events();
    this->fpsMax = fps;
    wait_time = 1.0f / (float)fpsMax;
    frameLimit(fps);

    clear();
}


void Window::createQuadScreen() {
    Renderer::getInstance().createQuadScreen();
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

void Window::postProcess(bool horizontal) {
    std::shared_ptr<Shader> s = ResourcesManager::get().getShader("simple");
    s->Use()->setVector2f("screenSize", glm::vec2(width, height));
    //Renderer::getInstance().postProcess(horizontal);
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

void Window::clear() {
    Renderer::getInstance().clear();
}
