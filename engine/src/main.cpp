#include <FML/core/application.h>
#include <FML/Input/InputManager.h>
#include "Window.h"
#include <FML/Resource/ResourcesManager.h>
#include <span>
#include <Utilities/CommandLineParser.h>
#include <FML/Resource/ResourceLoader.h>


int main(int argc, char** argv)
{
	fm::FilePath path = fm::ResourcesManager::GetFilePathResource(fm::LOCATION::WORKING_DIRECTORY);
	fm::CommandLineParser::ArgInfo target;
	target.longName = "target";
	target.shortName = "t";

	fm::CommandLineParser::ArgInfo internalArg;
	internalArg.longName = "internal";
	internalArg.shortName = "i";

	fm::CommandLineParser lineParser({ target, internalArg });
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
	config.graphicAPI = RENDERING_MODE_VULKAN;
	config.windowFlag = SDL_WINDOW_RESIZABLE;
	config.internalResourcesDirectory = fm::Folder(lineParser.GetArg("internal"));

	fm::Application& app = fm::Application::Get();
	app.SetConfig(config);
	app.Init();
	app.LoadInternalResources();

	app.InitSystems();


	app.LoadProject(path);



	app.Start();
	fm::Window* window = app.GetWindow();



	while (!window->isClosed())
	{
		fm::InputManager::Get().PollEvents();
		app.Update();

		app.SwapBuffers();
	}

	fm::Application::Get().DeInit();

	return 0;
}