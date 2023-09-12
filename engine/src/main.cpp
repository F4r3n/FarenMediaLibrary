#include "Core/application.h"
#include <Input/InputManager.h>
#include "Window.h"
#include "Resource/ResourcesManager.h"
#include <span>
#include <Utilities/CommandLineParser.h>
int main(int argc, char** argv)
{
	fm::FilePath path = fm::ResourcesManager::GetFilePathResource(fm::LOCATION::WORKING_DIRECTORY);
	fm::CommandLineParser::ArgInfo info;
	info.longName = "target";
	info.shortName = "t";

	fm::CommandLineParser lineParser({info});
	if (lineParser.Process(argv, argc))
	{
		std::string targetArg = lineParser.GetArg("target");
		if (!targetArg.empty())
		{
			path = fm::FilePath(targetArg);
		}
	}


	fm::Config config;
	config.name = "FML Engine";
	config.fpsWanted = 75;
	config.width = 0;
	config.height = 0;
	config.standAlone = true;
	config.graphicAPI = GRAPHIC_API::VULKAN;
	config.windowFlag = SDL_WINDOW_RESIZABLE;
	fm::Application& app = fm::Application::Get();
	app.SetConfig(config);
	app.Init();
	app.LoadProject(path);
	app.Start();
	fm::Window *window = app.GetWindow();


	while (!window->isClosed())
	{
		fm::InputManager::Get().PollEvents();
		app.Update();

		window->swapBuffers();
	}

	fm::Application::Get().DeInit();

	return 0;
}