#include "Core/application.h"
#include <Input/InputManager.h>
#include "Window.h"
#include "Resource/ResourcesManager.h"
int main()
{

	fm::Config config;
	config.name = "FML Engine";
	config.fpsWanted = 60;
	config.width = 0;
	config.height = 0;
	config.standAlone = true;
	config.windowFlag = SDL_WINDOW_OPENGL;

	fm::Application::Get().SetConfig(config);
	fm::Application::Get().Init();
	fm::Application::Get().LoadProject(fm::ResourcesManager::GetFilePathResource(fm::LOCATION::WORKING_DIRECTORY));
	fm::Application::Get().Start();
	fm::Window *window = fm::Application::Get().GetWindow();


	while (!window->isClosed())
	{
		fm::InputManager::Get().PollEvents();
		fm::Application::Get().Update(true);

		window->swapBuffers();
	}

	fm::Application::Get().DeInit();

	return 0;
}