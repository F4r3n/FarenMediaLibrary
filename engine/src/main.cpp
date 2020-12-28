#include "Core/application.h"
#include <Input/InputManager.h>
#include "Window.h"

int main()
{

	fm::Config config;
	config.name = "FML Engine";
	config.fpsWanted = 60;
	config.width = 0;
	config.height = 0;
	config.windowFlag = SDL_WINDOW_OPENGL;

	fm::Application::Get().SetConfig(config);
	fm::Application::Get().Init();
	

	fm::Window *window = fm::Application::Get().GetWindow();


	bool isMainWindowInitialized = false;

	while (!window->isClosed())
	{
		fm::InputManager::Get().PollEvents();
		fm::Application::Get().Update(true);

		bool hasFocus = (SDL_GetWindowFlags(window->getWindow()) & SDL_WINDOW_INPUT_FOCUS) == SDL_WINDOW_INPUT_FOCUS;
		window->swapBuffers();

	}

	fm::Application::Get().DeInit();

	return 0;
}