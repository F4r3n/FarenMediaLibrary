#pragma once


#include <string>
#include "NonCopyable.h"

struct SDL_Window;

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
    SDL_Window* getWindow() { return _window;}
    void setName(const std::string &name);
    inline std::string getName() const {
        return _nameWindow;
    }
    static int kWidth;
    static int kHeight;
    static int kX;
    static int kY;
    bool Init();
	void* GetContext();
private:
    int  _Init();
    void _CreateShaders();
    void _CreateMaterials();
    void _ErrorDisplay();

	bool _isInit = false;

    SDL_Window* _window = nullptr;
    void * _mainContext;

    int _fpsMax = 120;
    double _waitTime = 1.0f / (double)_fpsMax;
    double _currFrameTime = 0;
    double _frameStart = 0;
    
    std::string _nameWindow = "";
};
}
