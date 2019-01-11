#define GUI
#include <imgui/imgui.h>
#include "imgui_impl_sdl_gl3.h"
#include "Core/application.h"

#include "MainWindow.h"
#include <Core/Debug.h>
#include <Input/InputManager.h>


int main()
{

	fm::Config config;
	config.name = "FML Engine";
	config.fpsWanted = 60;
	config.width = 1280;
	config.height = 720;
	fm::Application::Get().SetConfig(config);
	fm::Application::Get().Init();
	

	fm::Window *window = fm::Application::Get().GetWindow();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplSdlGL3_Init(window->getWindow());

	MainWindow mainWindow;

	while (!window->isClosed())
	{
		while (SDL_PollEvent(&fm::InputManager::getInstance().getLastEvent()))
		{
			fm::InputManager::getInstance().processEvents();
			ImGui_ImplSdlGL3_ProcessEvent(&fm::InputManager::getInstance().getLastEvent());
		}

		fm::Application::Get().Update();
		ImGui_ImplSdlGL3_NewFrame(window->getWindow());
		mainWindow.Update();
		mainWindow.Draw();

		ImGui::Render();
		ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
		window->swapBuffers();

	}
	ImGui_ImplSdlGL3_Shutdown();

	fm::Application::Get().DeInit();
	return 0;
}