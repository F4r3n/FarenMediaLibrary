#define GUI
#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "imgui.h"
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include "Core/application.h"

#include "MainWindow.h"
#include <Core/Debug.h>
#include <Input/InputManager.h>
#include "Rendering/Renderer.h"

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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	ImGui_ImplSDL2_InitForOpenGL(window->getWindow(), window->GetContext());
	ImGui_ImplOpenGL3_Init("#version 330");

	std::shared_ptr<MainWindow> mainWindow = std::make_shared<MainWindow>();
	fm::Application::Get().AddApplicationObserver(mainWindow);
	bool isMainWindowInitialized = false;

	while (!window->isClosed())
	{
		while (SDL_PollEvent(&fm::InputManager::Get().getLastEvent()))
		{
			fm::InputManager::Get().processEvents();
			ImGui_ImplSDL2_ProcessEvent(&fm::InputManager::Get().getLastEvent());
		}
		fm::Application::Get().Update(true);
		fm::InputManager::Get().CaptureMousePosition();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window->getWindow());
		ImGui::NewFrame();


		if (isMainWindowInitialized)
		{
			mainWindow->Update();
			mainWindow->Draw();
		}
		else
		{
			mainWindow->Init();
			isMainWindowInitialized = true;
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		window->swapBuffers();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
			SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
		}


	}
	fm::Application::Get().DeInit();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return 0;
}