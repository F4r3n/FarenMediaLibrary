#include <SDL.h>

#include "Window.h"
#include <Core/Config.h>

#include <TimeDef.h>
#include <chrono>
#include <thread>
#include "Rendering/Renderer.h"
#include "Input/InputManager.h"
#include "Resource/ResourcesManager.h"

#include "Core/Config.h"
#include "Core/Debug.h"
#include <string>

using namespace fm;
size_t Window::kWidth = 0;
size_t Window::kHeight = 0;

int Window::kX = 0;
int Window::kY = 0;


Window::Window(size_t width, size_t height, size_t inWindowFlag)
:_isInit(false),
_window(nullptr),
_mainContext(nullptr),
_fpsMax(120),
_waitTime(1.0f/_fpsMax),
_currFrameTime(0),
_frameStart(0),
_msaa(0),
_windowFlag(inWindowFlag)
{
    Window::kWidth = width;
    Window::kHeight = height;

}

bool Window::Init()
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        return false;
    }

	//bool isFullScreen = Window::kWidth == -1 || Window::kHeight == -1;
	if (Window::kWidth <= 0 || Window::kHeight <= 0)
	{
		SDL_Rect rect;

		if (_windowFlag & SDL_WINDOW_FULLSCREEN || _windowFlag & SDL_WINDOW_FULLSCREEN_DESKTOP)
		{
			SDL_GetDisplayBounds(0, &rect);
		}
		else if (_windowFlag & SDL_WINDOW_MAXIMIZED)
		{
			SDL_GetDisplayUsableBounds(0, &rect);
#if _WIN32
			rect.h -= 20;
#endif
		}
		else
		{
			SDL_GetDisplayUsableBounds(0, &rect);
#if _WIN32
			rect.h -= 20;
#endif
		}
		Window::kWidth = rect.w;
		Window::kHeight = rect.h;

	}

    _window = SDL_CreateWindow("FML engine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
							  Window::kWidth,
                              Window::kHeight,
                              (Uint32)_windowFlag);


    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);


    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetSwapInterval(0);

    if(_Init())
    {

		
        _isInit = true;
        fm::Debug::get().LogError("Init");
        return true;
    }
    return false;
}




void Window::setMSAA(int value) 
{
	_msaa = value;
    if(_msaa > 0)
    {
        glEnable(GL_MULTISAMPLE);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, _msaa);
    }else
    {
        glDisable(GL_MULTISAMPLE);
    }
}


void Window::setName(const std::string& name) 
{
    SDL_SetWindowTitle(_window, name.c_str());
}



void Window::update(size_t fps) 
{
    _fpsMax = fps;
    _waitTime = 1.0f / (float)_fpsMax;
	_FrameLimit();
}


void Window::_FrameLimit() 
{
    _currFrameTime = SDL_GetTicks() - _frameStart;
    double dur = (_waitTime * 1000 - _currFrameTime);
    if(dur > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds((int)dur));
    }

    double frame_end = SDL_GetTicks();
    Time::dt = (float)((frame_end - _frameStart) / 1000.0);
    Time::timeStamp += Time::dt;
    _frameStart = frame_end;
}


void Window::swapBuffers() const
{
    if(_window)
        SDL_GL_SwapWindow(_window);
}


void Window::_ErrorDisplay() 
{
    const int error = glGetError();
    if(error != 0) {
        fm::Debug::get().logErrorExit(error, __FILE__, __LINE__);
    }
}


bool Window::isClosed() 
{
    return InputManager::Get().isClosed();
}


Window::~Window() 
{
    if(_isInit) {
        SDL_GL_DeleteContext(_mainContext);

        // Destroy our window
        SDL_DestroyWindow(_window);

        // Shutdown SDL 2
        SDL_Quit();
    }
}


int Window::_Init()
{
    if(_window == nullptr) 
	{
        return -1;
    }
    _mainContext = SDL_GL_CreateContext(_window);

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) 
	{
        return -1;
    }

    _ErrorDisplay();

    glViewport(kX, kY, kWidth, kHeight);

    return 1;
}


SDL_GLContext Window::GetContext()
{ 
	return (SDL_GLContext)_mainContext; 
}
