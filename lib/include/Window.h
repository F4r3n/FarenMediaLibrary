#pragma once
// GLEW
//#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include <iostream>
#include "NonCopyable.h"

namespace fm {
class Window : public fm_system::NonCopyable {
    friend class InputManager;

public:
    Window(int width, int height, const std::string& name);
    ~Window();

    void swapBuffers();
    void clear();
    bool isClosed();
    void frameLimit(unsigned short fps);
    void update(float fps);
    Window& getInstance();
    GLFWwindow* getWindow() {
        return window;
    }
    void setName(const std::string &name);
    inline std::string getName() const {
        return nameWindow;
    }
    static int width;
    static int height;
    static int x;
    static int y;
private:
    void events();
    int init(GLFWwindow* window);
    void createQuadScreen();
    void createShaders();
    void postProcess(bool horizontal);
    void errorDisplay();
    void blur();
    static void window_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(x, y, width, height);
        Window::width = width;
        Window::height = height;
    }
    GLFWwindow* window;

    int fpsMax = 60;
    double wait_time = 1.0f / (float)fpsMax;
    double curr_frame_time = 0;
    double dur = 0;
    double frame_start = 0;
    
    std::string nameWindow;
};
}
