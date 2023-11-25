#define GUI
#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "imgui.h"
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

#include <FML/Core/application.h>

#include "MainWindow.h"
#include <FML/Core/Debug.h>
#include <FML/Input/InputManager.h>

#include <FML/Window.h>
#include "Editor.h"
#include "Shader/ShaderCompiler.h"
#include <FML/Resource/ResourcesManager.h>
#include <FML/Resource/ResourceLoader.h>
#include <algorithm>
#include <FML/Rendering/ShaderKind.hpp>
#include <bitset>
#include <nlohmann/json.hpp>
#include <GL/glew.h>

using Combinations = std::vector<std::vector<std::string>>;

Combinations GetPossibleCombinations(const fm::Folder& inFolder, std::vector<std::string>& outListDrivers)
{
	Combinations comb;
	nlohmann::json j;
	fm::File file(fm::FilePath(inFolder.GetPath()).ToSub("combinations.json"));
	if (!file.Exist())
	{
		return comb;
	}
	file.GetJSONContent(j);
	std::vector<std::string> listDrivers = j["drivers"];
	outListDrivers = std::move(listDrivers);

	if (j.contains("combination"))
	{
		std::vector<std::string> combination = j["combination"];
		const fm::ShaderID numberOfCombination = (uint64_t)1 << (uint32_t)combination.size();
		for (size_t i = 0; i < numberOfCombination; ++i)
		{
			std::vector<std::string> currentCombi;
			std::bitset<32> bits = i;
			for (int j = 0; j < bits.count(); ++j)
			{
				if (bits.test(j))
				{
					currentCombi.emplace_back(combination[j]);
				}
			}
			comb.emplace_back(currentCombi);
		}
	}
	else
	{
		comb.push_back({});
	}


	return comb;
}

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
				if (inFolder->GetPath().GetExtension() == ".shader")
				{
					std::vector<std::string> listDrivers;
					Combinations combinations = GetPossibleCombinations(*inFolder, listDrivers);
					gui::ShaderCompiler::ShaderCompilerSettings settings{};
					settings.api = GRAPHIC_API::OPENGL;
					settings.generateReflection = true;
					settings.shaderFolder = *inFolder;
					settings.listFoldersToInclude = { fm::Folder(fm::FilePath(fm::LOCATION::INTERNAL_SHADERS_LOCATION, "includes")) };

					std::string shaderPath = fm::FileSystem::ConvertPathToFileSystem(inFolder->GetPath());
					auto shader = fm::ResourcesManager::get().getResource<fm::Shader>(shaderPath);
					if (shader == nullptr)
					{
						shader = std::make_shared<fm::Shader>(inFolder->GetPath());
						fm::ResourcesManager::get().load<fm::Shader>(shaderPath, shader);
					}

					if (!shader->NeedReload())
						return;

					for (const auto& combination : combinations)
					{
						fm::SubShader::Reflections reflections;
						fm::SHADER_KIND kind = fm::Shader::ConvertStringsToShaderKind(combination);

						gui::ShaderCompiler::ShaderMacro macros;

						for (const auto& value : combination)
						{
							std::string v(value);
							std::transform(value.begin(), value.end(), v.begin(), ::toupper);

							macros.DefineMacro("_" + v + "_", 1);
						}

						for (const auto& driver : listDrivers)
						{
							if (driver == "opengl")
							{
								settings.generateSPV = false;
								settings.generatePreprocess = true;
								settings.api = GRAPHIC_API::OPENGL;
							}
							else
							{
								settings.api = GRAPHIC_API::VULKAN;
								settings.generateSPV = true;
								settings.generatePreprocess = false;
							}
							if (driver == "opengl")
							{
								macros.DefineMacro("_VULKAN_", 0);
								macros.DefineMacro("_OPENGL_", 1);
							}
							else
							{
								macros.DefineMacro("_VULKAN_", 1);
								macros.DefineMacro("_OPENGL_", 0);
							}
							

							settings.macros = macros;
							settings.shaderOutputFolder = fm::Folder(fm::FilePath(settings.shaderFolder.GetPath()).ToSub(std::to_string(kind)));
							settings.shaderOutputFolder->CreateFolder();

							fm::SubShader::Reflection reflect;
							compiler.Compile(settings, reflect);
							reflections[settings.api] = reflect;
						}
						
						if (shader != nullptr)
						{
							shader->AddSubShader(kind, reflections);
						}

					}

					loader.SaveImport(inFolder->GetPath(), true);

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
	config.graphicAPI = RENDERING_MODE(RENDERING_MODE_OPENGL);

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

	std::shared_ptr<Editor> editor = std::make_shared<Editor>();
	std::unique_ptr<MainWindow> mainWindow = std::make_unique<MainWindow>(editor);
	//Editor::Get().Subscribe(mainWindow.get());
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

		app.SwapBuffers();

	}
	//Editor::Get().Unsubscribe(mainWindow.get());
	mainWindow.reset();
	editor.reset();
	fm::Application::Get().DeInit();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	return 0;
}