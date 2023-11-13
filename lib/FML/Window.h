#pragma once


#include "NonCopyable.h"
#include <string>
#include "Rendering/GraphicsAPI.h"
#include "Core/Math/Vector2.h"
struct SDL_Window;


namespace fm {
class Window : public fm_system::NonCopyable {
    friend class InputManager;

public:
    Window(GRAPHIC_API inAPIFlag, size_t inWindowFlag);
    ~Window();
	Window() = delete;
    void swapBuffers() const;
    void setMSAA(int value);
    bool isClosed();
    void update(size_t fps);
    Window& getInstance();
    SDL_Window* getWindow() { return _window;}
    void setName(const std::string &name);

    bool Init(size_t width, size_t height);
	void* GetContext();
	size_t GetMSAA() const { return _msaa; }
	uint64_t GetTicks() const;
	fm::math::Vector2i GetSize() const;
	bool IsMinimized() const;
private:
	void _FrameLimit();

    bool  _Init();
    void _ErrorDisplay();

	void _OpenGL_SetProfile();

private:
	bool		_isInit;

    SDL_Window* _window;
    void *		_mainContext;

	size_t		_fpsMax;
	double		_waitTime;
	uint64_t	_currFrameTime;
	uint64_t	_frameStart;
	int			_msaa;
	GRAPHIC_API	_api = GRAPHIC_API::OPENGL;
	size_t		_windowFlag;	
};
}
