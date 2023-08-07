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
	fm::Application& app = fm::Application::Get();
	app.SetConfig(config);
	app.Init();
	app.LoadProject(fm::ResourcesManager::GetFilePathResource(fm::LOCATION::WORKING_DIRECTORY));
	app.Start();
	fm::Window *window = app.GetWindow();


	while (!window->isClosed())
	{
		fm::InputManager::Get().PollEvents();
		app.Update(true);

		window->swapBuffers();
	}

	fm::Application::Get().DeInit();

	return 0;
}