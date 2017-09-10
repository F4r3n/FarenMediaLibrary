
#include "Window.h"
#include <Core/Config.h>

#include "Time.h"
#include <chrono>
#include <thread>
#include "Rendering/Renderer.h"
#include "Input/InputManager.h"
#include "Resource/ResourcesManager.h"
#include "Components/CMesh.h"
#include "Resource/RFont.h"
#include "Rendering/ShaderLibrary.h"

#include <iostream>
#include "Core/Config.h"


using namespace fm;
int Window::width;
int Window::height;

int Window::x = 0;
int Window::y = 0;
Window::Window(int width, int height, const std::string& name) {
    this->nameWindow = name;

    Window::width = width;

    Window::height = height;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        return;
    }
    std::cout << OPENGL_ES_VERSION << std::endl;



    window = SDL_CreateWindow(
        name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
#if OPENGL_CORE == 1
std::cout << "CORE" << std::endl;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#elif OPENGL_ES == 1
std::cout << "ES" << std::endl;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_ES_VERSION);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif


    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    init(window);
    std::cout << "Create shaders" << std::endl;
    createShaders();
    std::cout << "Shaders created" << std::endl;
    ResourcesManager::get().load("dejavu", std::make_unique<RFont>("assets/fonts/dejavu/DejaVuSansMono.ttf"));
        isInit = true;
}

void Window::setMSAA(int value) {
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, value);
}

void Window::setName(const std::string& name) {
    this->nameWindow = name;
    SDL_SetWindowTitle(window, name.c_str());
}

void Window::createShaders() {
    // Create, load and set textures shader
    ShaderLibrary::loadShaders();

    // Initialize all the shapes
    for(int i = 0; i < fmc::SHAPE::LAST_SHAPE; ++i) {
        fmc::CMesh rect;
        rect.setShape(i);
    }
}

void Window::update(float fps) {
    // events();
    this->fpsMax = fps;
    wait_time = 1.0f / (float)fpsMax;
    frameLimit(fps);
    fm::InputManager::getInstance().pollEvents();
}

void Window::frameLimit(unsigned short fps) {

    curr_frame_time = SDL_GetTicks() - frame_start;
    double dur = (wait_time * 1000 - curr_frame_time);
    if(dur > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds((int)dur));
    }

    double frame_end = SDL_GetTicks();
    Time::dt = (frame_end - frame_start) / 1000.0f;
    Time::timeStamp += Time::dt;
    frame_start = frame_end;
}

void Window::swapBuffers() {
    // At the end of the current frame check if an error occured
    errorDisplay();
    SDL_GL_SwapWindow(window);
}

void Window::errorDisplay() {
    #ifndef __EMSCRIPTEN__
    int error = glGetError();
    if(error != 0) {
        std::cerr << "ERROR OPENGL " << error << std::endl;
        exit(-1);
    }
    #else
            printf("Check error SDL: %s\n", SDL_GetError());

        #endif
}

bool Window::isClosed() {
    return InputManager::getInstance().isClosed();
}

Window::~Window() {
    if(isInit) {
    SDL_GL_DeleteContext(mainContext);

    // Destroy our window
    SDL_DestroyWindow(window);

    // Shutdown SDL 2
    SDL_Quit();
    
    
    }
}

int Window::init(SDL_Window* window) {
    if(window == nullptr) {

        // glfwTerminate();
        return -1;
    }
    mainContext = SDL_GL_CreateContext(window);
    std::cout << glGetString(GL_VERSION) << std::endl;
    #ifndef __EMSCRIPTEN__
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        return -1;
    }
    #endif
    #ifndef __EMSCRIPTEN__
    glGetError();
    #endif
    errorDisplay();

    glViewport(x, y, width, height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    return 1;
}
