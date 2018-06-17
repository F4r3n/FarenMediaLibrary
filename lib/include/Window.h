#pragma once

#include <SDL.h>

#include <string>
#include "NonCopyable.h"

namespace fm {
class Window   {
    friend class InputManager;

public:
    Window(int width, int height, const std::string& name);
    ~Window();
    explicit Window() {}
    void swapBuffers() const;
    static void setMSAA(int value);
    bool isClosed();
    void frameLimit(unsigned short fps);
    void update(float fps, bool internalUpdate = true);
    Window& getInstance();
    SDL_Window* getWindow() {
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
    bool isInit = false;
    int init(SDL_Window* window);
    void createShaders();
    void errorDisplay();

    SDL_Window* window = nullptr;
    SDL_GLContext mainContext;

    int fpsMax = 120;
    double wait_time = 1.0f / (double)fpsMax;
    double curr_frame_time = 0;
    double dur = 0;
    double frame_start = 0;
    
    std::string nameWindow = "";
};
}
