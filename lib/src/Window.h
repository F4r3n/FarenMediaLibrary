#pragma once


#include "NonCopyable.h"
#include <string>
#include "Rendering/Vulkan/Vulkan.h"
struct SDL_Window;

enum GRAPHIC_API
{
	OPENGL,
	VULKAN
};


namespace fm {
class Window   {
    friend class InputManager;

public:
    Window(size_t width, size_t height, GRAPHIC_API inAPIFlag, size_t inWindowFlag);
    ~Window();
	Window() = delete;
    void swapBuffers() const;
    void setMSAA(int value);
    bool isClosed();
    void update(size_t fps);
    Window& getInstance();
    SDL_Window* getWindow() { return _window;}
    void setName(const std::string &name);

    static size_t kWidth;
    static size_t kHeight;
    static int kX;
    static int kY;
    bool Init();
	void* GetContext();
	size_t GetMSAA() const { return _msaa; }
	double GetTicks() const;
private:
	void _FrameLimit();

    int  _Init();
    void _ErrorDisplay();

	void _OpenGL_SetProfile();

private:
	bool		_isInit;

    SDL_Window* _window;
    void *		_mainContext;

    size_t		_fpsMax;
    double		_waitTime;
	double		_currFrameTime;
    double		_frameStart;
	size_t		_windowFlag;
	GRAPHIC_API	_api = GRAPHIC_API::OPENGL;
	int			_msaa;

	Vulkan		_vulkan;
};
}
