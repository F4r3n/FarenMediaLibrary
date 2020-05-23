
#include "MainWindow.h"
#include <functional>

#include "TimeDef.h"
#include "Engine.h"
#include "Rendering/RenderingSystem.h"

#include "Components/Components.h"

#include "Core/SceneManager.h"
#include "Core/Debug.h"
#include "Core/Debug.h"
#include "Core/Scene.h"
#include "Core/GameObject.h"
#include "Core/application.h"

#include "GListEntities.h"
#include "PortableFileDialog.h"
#include "PickingSystem.h"
#include "ToolBar.hpp"
#include "debuglogger.h"
#include "EditorView.h"
#include "GameView.h"
#include "Window.h"
#include "ListComponentWindow.hpp"
#include "FileNavigator.h"

#include "Resource/ResourcesManager.h"

#include "SDL_scancode.h"
#include <imgui/imgui_internal.h>

const std::string JSON_KEY = "FML";

MainWindow::MainWindow()
{
	fm::Debug::logWarning("Start init");
	_ConfigureStyle();

	_context.currentSceneName = fm::Application::Get().CreateNewScene("newScene")->getName();
	_editorScene = fm::Application::Get().CreateEditorScene();

	_InitEditorCamera();

	std::unique_ptr<gui::EditorView> gameView = std::make_unique<gui::EditorView>(_editorCamera, _editorScene);
	gameView->SetPickingSystem(new fms::PickingSystem( _editorScene));


	_windows[gui::WINDOWS::WIN_LIST_ENTITIES] = std::make_unique<gui::GListEntities>();
	_windows[gui::WINDOWS::WIN_EDITOR_VIEW] = std::move(gameView);
	_windows[gui::WINDOWS::WIN_LOGGER] = std::make_unique<gui::DebugLogger>();
	_windows[gui::WINDOWS::WIN_TOOLBAR] = std::make_unique<gui::ToolBar>();
	_windows[gui::WINDOWS::WIN_SCENE_VIEW] = std::make_unique<gui::GameView>();
	_windows[gui::WINDOWS::WIN_INSPECTOR] = std::make_unique<gui::ListComponentWindow>();
	_windows[gui::WINDOWS::WIN_FILE_NAVIGATOR] = std::make_unique<gui::GFileNavigator>();


	fm::Debug::log("Init done");
	_needUpdate = false;
}


void MainWindow::_AddEmptyScene()
{
	std::shared_ptr<fm::Scene> currentScene = fm::Application::Get().GetScene(_context.currentSceneName);
	//Add object
	fm::GameObject* go = fm::GameObjectHelper::create(currentScene, true);
	fmc::CTransform* tr = go->get<fmc::CTransform>();
	tr->position = fm::math::vec3(0, 0, -1);
	go->addComponent<fmc::CMaterial>();
	go->addComponent<fmc::CMesh>();

	//Add camera
	go = fm::GameObjectHelper::create(currentScene, true);
	tr = go->get<fmc::CTransform>();
	tr->position = fm::math::vec3(0, 0, 0);
	go->addComponent<fmc::CCamera>()->Init();
	if (IsWindowAvailable(gui::WINDOWS::WIN_SCENE_VIEW))
	{
		gui::GameView* gv = dynamic_cast<gui::GameView*>(_windows[gui::WINDOWS::WIN_SCENE_VIEW].get());
		go->SetName("Main camera");
		gv->AddCamera(go);
	}
}

void MainWindow::Init()
{
	_AddEmptyScene();
}


void MainWindow::_InitEditorCamera()
{
	_editorCamera = fm::GameObjectHelper::create(_editorScene, true);
	_editorCamera->addComponent<fmc::CCamera>(fm::Window::kWidth, fm::Window::kHeight,
		fmc::RENDER_MODE::FORWARD, false /*ortho*/, false/*auto*/, fm::Application::Get().GetWindow()->GetMSAA())->Init();
	_editorCamera->get<fmc::CTransform>()->position = fm::math::vec3(0, 0, -1);
	_editorCamera->SetName("Camera");
}

MainWindow::~MainWindow()
{
}



void MainWindow::_DisplayWindow_Save()
{
	pfd::select_folder dialog = pfd::select_folder("Save to...", ".");
	std::string&& resultFromDialog = dialog.result();

	if (!resultFromDialog.empty())
	{
		fm::FilePath result(resultFromDialog + fm::FilePath::GetFolderSeparator());
		
		fm::Application::Get().SetUserDirectory(fm::Folder(result));
		fm::Application::Get().Serialize();
	}
}

bool MainWindow::IsWindowAvailable(gui::WINDOWS inWindow)
{
	gui::GWindow* window = _windows[inWindow].get();
	return window != nullptr;
}


void MainWindow::_DisplayWindow_Load()
{
	pfd::open_file dialog = pfd::open_file("Choose files to read", ".",
		{ "Fml files", "*.fml",
		  "All Files", "*" },
		false);

	std::vector<std::string> resultFromDialog = dialog.result();

	if (!resultFromDialog.empty())
	{
		fm::FilePath result(resultFromDialog.front());
		fm::Application::Get().LoadProject(result);

	}
}


void MainWindow::_DrawMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::BeginMenu("Run"))
			{
				if (ImGui::MenuItem("Start"))
				{
					fm::Application::Get().Start(true);
					_currentEntity.reset();
				}
				if (ImGui::MenuItem("Pause"))
				{
					//_engine->Stop();
				}
				if (ImGui::MenuItem("Stop"))
				{
					fm::Application::Get().Stop();
					_currentEntity.reset();
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Project Settings"))
			{
				//_windowStates[WINDOWS::WIN_PROJECT_SETTINGS] = true;
			}
			if (ImGui::BeginMenu("Save"))
			{
				if (ImGui::MenuItem("Save to ..."))
				{
					_DisplayWindow_Save();
				}
				if (ImGui::MenuItem("Save"))
				{
					fm::Application::Get().SerializeCurrentScene();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Load"))
			{

				if (ImGui::MenuItem("Load from ..."))
				{
					_DisplayWindow_Load();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();

		}
		if (ImGui::BeginMenu("World"))
		{
			if (ImGui::MenuItem("Light"))
			{
				//_windowStates[WINDOWS::WIN_LIGHT_EDIT] = true;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Options"))
		{
			bool enable = _windows[gui::WINDOWS::WIN_LOGGER]->IsEnabled();
			if (ImGui::MenuItem("Logger", "L", &enable, true))
			{
				if (enable)
				{
					_windows[gui::WINDOWS::WIN_LOGGER]->Start();
				}
				else
				{
					_windows[gui::WINDOWS::WIN_LOGGER]->Stop();
				}
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Cameras"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Entity"))
		{
			if (ImGui::MenuItem("Create"))
			{
				_currentEntity = fm::GameObjectHelper::create(fm::Application::Get().GetScene(_context.currentSceneName), true)->getID();

			}
			if (ImGui::MenuItem("List entity"))
			{
				//_windowListEntity = true;
			}
			if (ImGui::MenuItem("Copy", "CTRL+C"))
			{
				_Copy();
			}
			if (ImGui::MenuItem("Paste", "CTRL+V"))
			{
				_Paste();
			}

			ImGui::EndMenu();
		}
	}

	ImGui::EndMainMenuBar();

}

void MainWindow::_Copy()
{
	if (_currentEntity.has_value())
	{
		nlohmann::json j;
		nlohmann::json json;
		fm::GameObject* go = fm::Application::Get().GetCurrentScene()->GetGameObjectByID(_currentEntity.value());
		go->Serialize(json);
		j[JSON_KEY] = json;

		ImGui::SetClipboardText(j.dump().c_str());
	}
}


void MainWindow::_Paste()
{
	std::string  s(ImGui::GetClipboardText());
	if (!s.empty())
	{
		nlohmann::json j = nlohmann::json::parse(s);
		if (j.is_object())
		{
			nlohmann::json json = j[JSON_KEY];
			if (json.is_object())
			{
				fm::GameObject* go = fm::GameObjectHelper::create(fm::Application::Get().GetScene(_context.currentSceneName), false);
				go->Read(json);
			}
		}
	}

}


void MainWindow::Update()
{
	_needUpdate = false;

	for (auto& window : _windows)
	{
		if (window.second != nullptr)
		{
			window.second->Update(fm::Time::dt, _context);
		}
	}
	if (_context.currentGameObjectSelected.has_value())
	{
		_currentEntity = _context.currentGameObjectSelected;
	}
}

void MainWindow::_AddDock(gui::WINDOWS inWindow, ImGuiID inID)
{
	
	if (_windows[inWindow] != nullptr)
	{
		ImGui::DockBuilderDockWindow(_windows[inWindow]->GetTitle().c_str(), inID);
	}
}



void MainWindow::Draw()
{

	bool p_open = true;
	ImGuiID dockspace_id;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::SetNextWindowBgAlpha(1.0f);


	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));


	dockspace_id = ImGui::GetID("MyDockspace");

	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	if (!ImGui::DockBuilderGetNode(dockspace_id)) {
		ImGui::DockBuilderRemoveNode(dockspace_id);
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_None);
		ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);
		ImGuiID dock_main_id = dockspace_id;
		ImGuiID dock_up_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.05f, nullptr, &dock_main_id);
		ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.8f, nullptr, &dock_main_id);
		ImGuiID dock_left_right_id = ImGui::DockBuilderSplitNode(dock_left_id, ImGuiDir_Right, 0.2f, nullptr, &dock_left_id);

		ImGuiID dock_right_id = dock_main_id;

		ImGuiID dock_down_id = ImGui::DockBuilderSplitNode(dock_left_id, ImGuiDir_Down, 0.2f, nullptr, &dock_left_id);
		ImGuiID dock_down_right_id = ImGui::DockBuilderSplitNode(dock_right_id, ImGuiDir_Down, 0.5f, nullptr, &dock_right_id);

		_AddDock(gui::WINDOWS::WIN_TOOLBAR, dock_up_id);
		_AddDock(gui::WINDOWS::WIN_LIST_ENTITIES, dock_right_id);
		_AddDock(gui::WINDOWS::WIN_EDITOR_VIEW, dock_left_id);
		_AddDock(gui::WINDOWS::WIN_FILE_NAVIGATOR, dock_down_id);
		_AddDock(gui::WINDOWS::WIN_SCENE_VIEW, dock_left_right_id);
		_AddDock(gui::WINDOWS::WIN_INSPECTOR, dock_down_right_id);
		_AddDock(gui::WINDOWS::WIN_LOGGER, dock_down_id);


		//ImGuiDockNodeFlags_
		// Disable tab bar for custom toolbar
		ImGuiDockNode* node = ImGui::DockBuilderGetNode(dock_up_id);
		node->LocalFlags |= ImGuiDockNodeFlagsPrivate_::ImGuiDockNodeFlags_NoTabBar;

		ImGui::DockBuilderFinish(dock_main_id);
	}
	ImGui::Begin("DockSpace", &p_open, window_flags);
	ImGui::PopStyleVar(3);
	ImGui::DockSpace(dockspace_id, viewport->Size, dockspace_flags);

	//bool show_demo_window = true;
	//ImGui::ShowDemoWindow(&show_demo_window);


	if (_needUpdate)
	{
		Update();
	}
	for (auto& window : _windows)
	{
		if (window.second != nullptr)
		{
			window.second->Draw();
		}
	}
	
	_DrawMenu();

	ImGuiIO io = ImGui::GetIO();
	if (_context.currentWindowFocused == gui::WINDOWS::WIN_EDITOR_VIEW)
	{
		if (io.KeyCtrl && ImGui::IsKeyPressed(SDL_Scancode::SDL_SCANCODE_C, false))
		{
			_Copy();
		}
		if (io.KeyCtrl && ImGui::IsKeyPressed(SDL_Scancode::SDL_SCANCODE_V, false))
		{
			_Paste();
		}
		if (io.KeyCtrl && ImGui::IsKeyPressed(SDL_Scancode::SDL_SCANCODE_S, false))
		{
			fm::Application::Get().SerializeCurrentScene();
		}
	}

	ImGui::End();

}

void MainWindow::_ClearBeforeSceneChange()
{
	if (IsWindowAvailable(gui::WINDOWS::WIN_SCENE_VIEW))
	{
		_windows[gui::WINDOWS::WIN_SCENE_VIEW]->AddEvent([](gui::GWindow* inWindow) {
			dynamic_cast<gui::GameView*>(inWindow)->Clear();
		});
	}

	if (IsWindowAvailable(gui::WINDOWS::WIN_INSPECTOR))
	{
		_windows[gui::WINDOWS::WIN_INSPECTOR]->AddEvent([](gui::GWindow* inWindow) {
			dynamic_cast<gui::ListComponentWindow*>(inWindow)->ClearInspectorComponents();
		});
	}

	_currentEntity.reset();
	_context.currentGameObjectSelected.reset();
	_context.currentSceneName = "";
	_needUpdate = true;
}
void MainWindow::_InitGameView()
{
	std::shared_ptr<fm::Scene> currentScene = fm::Application::Get().GetScene(_context.currentSceneName);

	auto&& v = currentScene->getAllGameObjects();
	for (auto &&o : v)
	{
		fm::GameObject* go = o.second;
		if (o.second->has<fmc::CCamera>())
		{
			if (IsWindowAvailable(gui::WINDOWS::WIN_SCENE_VIEW))
			{
				_windows[gui::WINDOWS::WIN_SCENE_VIEW]->AddEvent([go](gui::GWindow* inWindow) {
					dynamic_cast<gui::GameView*>(inWindow)->AddCamera(go);
				});
			}
			break;
		}
	}
}
void MainWindow::OnPreStart()
{
	_ClearBeforeSceneChange();
	
}
void MainWindow::OnAfterStart()
{
	_context.currentSceneName = fm::Application::Get().GetCurrentSceneName();
	_InitGameView();
	_needUpdate = true;
	
}
void MainWindow::OnPreStop()
{
	_ClearBeforeSceneChange();
}
void MainWindow::OnAfterStop()
{
	_context.currentSceneName = fm::Application::Get().GetCurrentSceneName();
	_InitGameView();
	_needUpdate = true;
}

void MainWindow::OnPreLoad()
{
	_ClearBeforeSceneChange();
}
void MainWindow::OnAfterLoad()
{
	_context.currentSceneName = fm::Application::Get().GetCurrentSceneName();
	_InitGameView();
	_needUpdate = true;
}

void MainWindow::_ConfigureStyle()
{
	// From https://github.com/ocornut/imgui/issues/707#issuecomment-254610737
	// Some colors may have changed
	// Setup style

	ImGuiStyle& style = ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO();

	fm::FilePath p = fm::ResourcesManager::GetFilePathResource(fm::LOCATION::INTERNAL_FONT_LOCATION);
	p.ToSubFile("Roboto-Medium.ttf");
	io.Fonts->AddFontFromFileTTF(p.GetPath().c_str(), 14.0f);
	style.GrabRounding = 0.f;
	style.WindowRounding = 0.f;
	style.ScrollbarRounding = 3.f;
	style.FrameRounding = 3.f;
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

	style.Colors[ImGuiCol_Text] = ImVec4(0.73f, 0.73f, 0.73f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.26f, 0.26f, 0.26f, 0.95f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);

	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.32f, 0.52f, 0.65f, 1.00f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
}
