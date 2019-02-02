#include <SDL.h>

#include "Window.h"
#include <Core/Config.h>

#include <TimeDef.h>
#include <chrono>
#include <thread>
#include "Rendering/Renderer.h"
#include "Input/InputManager.h"
#include "Resource/ResourcesManager.h"
#include "Components/CMesh.h"
#include "Resource/RFont.h"
#include "Rendering/ShaderLibrary.h"

#include "Core/Config.h"
#include "Core/Debug.h"
#include "Rendering/material.hpp"
using namespace fm;
int Window::kWidth = 0;
int Window::kHeight = 0;

int Window::kX = 0;
int Window::kY = 0;

Window::Window(int width, int height, const std::string& name) 
{
    _nameWindow = name;

    Window::kWidth = width;
    Window::kHeight = height;

}

bool Window::Init()
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        return false;
    }

	bool isFullScreen = Window::kWidth == -1 || Window::kHeight == -1;
	if (Window::kWidth <= 0 || Window::kHeight <= 0)
	{
		SDL_DisplayMode DM;
		SDL_GetDesktopDisplayMode(0, &DM);
		
		Window::kWidth = DM.w;
		Window::kHeight = DM.h;
	}

	size_t flags = SDL_WINDOW_OPENGL;
	if (!isFullScreen)
		flags = flags | SDL_WINDOW_MAXIMIZED;
    _window = SDL_CreateWindow(_nameWindow.c_str(),
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
							  Window::kWidth,
                              Window::kHeight,
                              flags);

#if OPENGL_CORE == 1
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#elif OPENGL_ES == 1
    Debug::logWarning(OPENGL_ES_VERSION);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_ES_VERSION);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetSwapInterval(0);
    setMSAA(4);
    if(_Init())
    {
        _CreateShaders();
        _CreateMaterials();

		fm::FilePath p = fm::ResourcesManager::GetFilePathResource(fm::ResourcesManager::INTERNAL_FONT_LOCATION);
		p.Append("Roboto-Medium.ttf");
        ResourcesManager::get().load<RFont>("dejavu", new RFont(p.GetPath()));
        _isInit = true;
        fm::Debug::get().LogError("Init");
        return true;
    }
    return false;
}

void Window::_CreateMaterials()
{
    {
		Material *defaultMat = new fm::Material("default_material");
		defaultMat->shaderName = "default";
		defaultMat->shader = fm::ResourcesManager::get().getResource<fm::Shader>("default");
		fm::ResourcesManager::get().load<Material>("default_material", defaultMat);
    }

	{
		Material *defaultMat = new fm::Material("default_light_material");
		defaultMat->shaderName = "default_light";
		defaultMat->shader = fm::ResourcesManager::get().getResource<fm::Shader>("default_light");
		fm::ResourcesManager::get().load<Material>("default_light_material", defaultMat);
    }
}


void Window::setMSAA(int value) 
{
    if(value > 0)
    {
        glEnable(GL_MULTISAMPLE);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, value);
    }else
    {
        glDisable(GL_MULTISAMPLE);
    }
}


void Window::setName(const std::string& name) 
{
    _nameWindow = name;
    SDL_SetWindowTitle(_window, name.c_str());
}


void Window::_CreateShaders() 
{
    // Create, load and set textures shader
    ShaderLibrary::loadShaders();
}


void Window::update(float fps, bool internalUpdate) 
{
    // events();
    _fpsMax = fps;
    _waitTime = 1.0f / (float)_fpsMax;
    if(internalUpdate)
        fm::InputManager::getInstance().pollEvents();
    frameLimit(fps);

}


void Window::frameLimit(unsigned short fps) 
{
    _currFrameTime = SDL_GetTicks() - _frameStart;
    double dur = (_waitTime * 1000 - _currFrameTime);
    if(dur > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds((int)dur));
    }

    double frame_end = SDL_GetTicks();
    Time::dt = (frame_end - _frameStart) / 1000.0f;
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
#ifndef __EMSCRIPTEN__
    int error = glGetError();
    if(error != 0) {
        fm::Debug::get().logErrorExit(error, __FILE__, __LINE__);
        exit(-1);
    }
#else
    printf("Check error SDL: %s\n", SDL_GetError());

#endif
}


bool Window::isClosed() 
{
    return InputManager::getInstance().isClosed();
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

#if OPENGL_ES == 0
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) 
	{
        return -1;
    }
#endif

    _ErrorDisplay();

    glViewport(kX, kY, kWidth, kHeight);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    return 1;
}


SDL_GLContext Window::GetContext()
{ 
	return (SDL_GLContext)_mainContext; 
}
