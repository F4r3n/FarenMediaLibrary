#define GUI
#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "imgui.h"
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

#include "Core/application.h"

#include "MainWindow.h"
#include <Core/Debug.h>
#include <Input/InputManager.h>
#include "Rendering/Renderer.h"
#include "Window.h"
#include "Editor.h"
#include "ShaderCompiler.h"
#include "Resource/ResourcesManager.h"
#include "Resource/ResourceLoader.h"
void CompileShaders()
{
	fm::ResourceLoader loader;
	loader.Init();

	fm::FilePath path(fm::LOCATION::INTERNAL_SHADERS_LOCATION, "");
	gui::ShaderCompiler compiler;

	fm::Folder(path).Iterate(false, [&compiler, &loader](const fm::Folder* inFolder, const fm::File* inFile)
	{
		if (inFolder)
		{
			if (inFolder->GetPath().GetExtension() == ".vkshader")
			{
				gui::ShaderCompiler::ShaderCompilerSettings settings{};
				settings.api = GRAPHIC_API::VULKAN;
				settings.generatePreprocess = false;
				settings.generateReflection = true;
				settings.generateSPV = true;
				settings.shaderFolder = *inFolder;

				fm::Shader::Reflection reflect;
				compiler.Compile(settings, reflect);
				auto shader = fm::ResourcesManager::get().getResource<fm::Shader>(fm::FileSystem::ConvertPathToFileSystem(inFolder->GetPath()));
				if (shader != nullptr)
				{
					shader->SetReflection(reflect);
				}
				loader.SaveImport(inFolder->GetPath(), false);
			}
			else if (inFolder->GetPath().GetExtension() == ".shader")
			{
				gui::ShaderCompiler::ShaderCompilerSettings settings{};
				settings.api = GRAPHIC_API::OPENGL;
				settings.generatePreprocess = true;
				settings.generateReflection = true;
				settings.generateSPV = false;
				settings.shaderFolder = *inFolder;

				fm::Shader::Reflection reflect;
				compiler.Compile(settings, reflect);
				auto shader = fm::ResourcesManager::get().getResource<fm::Shader>(fm::FileSystem::ConvertPathToFileSystem(inFolder->GetPath()));
				if (shader != nullptr)
				{
					shader->compile();
					shader->SetReflection(reflect);
				}
				loader.SaveImport(inFolder->GetPath(), false);
			}
		}
	});
}


int main()
{
	fm::Config config;
	config.name = "FML Engine";
	config.fpsWanted = 60;
	config.width = 1280;
	config.height = 720;
	config.windowFlag = SDL_WINDOW_RESIZABLE;
	config.graphicAPI = GRAPHIC_API::OPENGL;

	fm::Application& app = fm::Application::Get();
	app.SetConfig(config);
	app.Init();

	app.LoadInternalResources();
	CompileShaders();
	app.InitSystems();

	fm::Window* window = fm::Application::Get().GetWindow();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // Enable Multi-Viewport / Platform Windows
	io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // Enable Multi-Viewport / Platform Windows
	//io.DisplayFramebufferScale = ImVec2(1.25f, 1.25f);
	ImGui_ImplSDL3_InitForOpenGL(window->getWindow(), window->GetContext());
	ImGui_ImplOpenGL3_Init("#version 330");

	std::unique_ptr<MainWindow> mainWindow = std::make_unique<MainWindow>();
	Editor::Get().Subscribe(mainWindow.get());
	bool isMainWindowInitialized = false;

	while (!window->isClosed())
	{
		fm::InputManager::Get().PollEvents([](const SDL_Event& e) {
			ImGui_ImplSDL3_ProcessEvent(&e);
			});

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		
		fm::Application::Get().Update();

		const bool hasFocus = ((SDL_GetWindowFlags(window->getWindow()) & SDL_WINDOW_INPUT_FOCUS) == SDL_WINDOW_INPUT_FOCUS);

		if (isMainWindowInitialized)
		{
			mainWindow->OnUpdate(hasFocus, false);
			mainWindow->OnDraw();
		}
		else
		{
			mainWindow->Init();
			isMainWindowInitialized = true;
		}


		ImGui::Render();
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
			SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
		}

		window->swapBuffers();

	}
	Editor::Get().Unsubscribe(mainWindow.get());

	fm::Application::Get().DeInit();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	return 0;	
}