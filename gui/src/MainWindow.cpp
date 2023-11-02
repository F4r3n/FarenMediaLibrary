
#include "MainWindow.h"
#include <functional>

#include "TimeDef.h"
#include "Engine.h"
#include "Rendering/RenderingSystem.h"

#include "Components/Components.h"

#include "Core/SceneManager.h"
#include "Core/Debug.h"
#include "Core/Scene.h"
#include "Core/GameObject.h"
#include "Core/application.h"

#include "PortableFileDialog.h"
#include "PickingSystem.h"
#include "Window.h"

#include "Window/GListEntities.h"
#include "Window/GToolBar.hpp"
#include "Window/GDebugLogger.h"
#include "Window/GEditorView.h"
#include "Window/GGameView.h"
#include "Window/GListComponent.hpp"
#include "Window/GFileNavigator.h"
#include "Window/GMaterialEditor.h"
#include "Window/GLauncher.h"

#include "Resource/ResourcesManager.h"
#include "Resource/ResourceLoader.h"
#include <imgui/imgui_internal.h>
#include "Editor.h"
#include "Export.hpp"
#include <nlohmann/json.hpp>

const std::string JSON_KEY = "FML";

MainWindow::MainWindow()
{
	fm::Debug::logWarning("Start init");
	_ConfigureStyle();

	_context.currentSceneName = "";
	_editorScene = Editor::Get().CreateEditorScene();

	_InitEditorCamera();

	std::unique_ptr<gui::GEditorView> editorView = std::make_unique<gui::GEditorView>(_editorCamera, _editorScene);
	editorView->SetPickingSystem(std::make_unique<fms::PickingSystem>(_editorScene));


	_windows[gui::WINDOWS::WIN_LIST_ENTITIES] = std::make_unique<gui::GListEntities>();
	_windows[gui::WINDOWS::WIN_EDITOR_VIEW] = std::move(editorView);
	_windows[gui::WINDOWS::WIN_LOGGER] = std::make_unique<gui::GDebugLogger>();
	_windows[gui::WINDOWS::WIN_TOOLBAR] = std::make_unique<gui::GToolBar>();
	_windows[gui::WINDOWS::WIN_SCENE_VIEW] = std::make_unique<gui::GGameView>();
	_windows[gui::WINDOWS::WIN_INSPECTOR] = std::make_unique<gui::GListComponent>();
	_windows[gui::WINDOWS::WIN_FILE_NAVIGATOR] = std::make_unique<gui::GFileNavigator>();
	_windows[gui::WINDOWS::WIN_MATERIAL_EDITOR] = std::make_unique<gui::GMaterialEditor>();
	_windows[gui::WINDOWS::WIN_LAUNCHER] = std::make_unique<gui::GLauncher>();
	for (auto&& [key, value] : _windows)
	{
		value->Stop();
	}
	_windows[gui::WINDOWS::WIN_LAUNCHER]->Start();
	_windows[gui::WINDOWS::WIN_LAUNCHER]->SetCallBackClosure([this](gui::GWindow* inWindow, std::optional<gui::Context> Context)
		{
			gui::GLauncher* launcher = dynamic_cast<gui::GLauncher*>(inWindow);
			LoadProject(launcher->GetResult());
		});

	fm::Debug::log("Init done");
	_needUpdate = false;
}


void MainWindow::LoadProject(const fm::FilePath& inFilePath)
{
	Editor::Get().NewProject(inFilePath);

	_windows[gui::WINDOWS::WIN_LIST_ENTITIES]->Start();
	_windows[gui::WINDOWS::WIN_EDITOR_VIEW]->Start();
	_windows[gui::WINDOWS::WIN_LOGGER]->Start();
	_windows[gui::WINDOWS::WIN_TOOLBAR]->Start();
	_windows[gui::WINDOWS::WIN_SCENE_VIEW]->Start();
	_windows[gui::WINDOWS::WIN_INSPECTOR]->Start();
	_windows[gui::WINDOWS::WIN_FILE_NAVIGATOR]->Start();
	//_windows[gui::WINDOWS::WIN_MATERIAL_EDITOR]->Start();

	fm::FilePath path(fm::LOCATION::USER_LOCATION, "");
	_RefreshResources(path);
}

void MainWindow::_RefreshResources(const fm::FilePath& inPath)
{
	fm::Folder folder(inPath);
	fm::ResourceLoader loader;
	loader.Init();
	folder.Iterate(true, [&loader](const fm::Folder* inFolder, const fm::File* inFile) {
		if (inFile != nullptr)
		{
			loader.SaveImport(inFile->GetPath(), false);
			loader.Load(inFile->GetPath(), true);
		}
	});
}


void MainWindow::_AddEmptyScene()
{
	std::shared_ptr<fm::Scene> currentScene = Editor::Get().GetScene(_context.currentSceneName);
	//Add object
	std::shared_ptr <fm::GameObject> go = currentScene->CreateGameObject(true);
	fmc::CTransform* tr = go->get<fmc::CTransform>();
	tr->SetPosition(fm::math::vec3(0, 0, -1));
	go->addComponent<fmc::CMaterial>();
	go->addComponent<fmc::CMesh>();

	//Add camera
	go = currentScene->CreateGameObject(true);
	tr = go->get<fmc::CTransform>();
	tr->SetPosition(fm::math::vec3(0, 0, 0));
	go->addComponent<fmc::CCamera>()->Init();
	if (IsWindowAvailable(gui::WINDOWS::WIN_SCENE_VIEW))
	{
		gui::GGameView* gv = dynamic_cast<gui::GGameView*>(_windows[gui::WINDOWS::WIN_SCENE_VIEW].get());
		go->SetName("Main camera");
		gv->AddCamera(go);
	}

	_AfterLoad();
}

void MainWindow::Init()
{
	//_AddEmptyScene();
}


void MainWindow::_InitEditorCamera()
{
	auto size = fm::Application::Get().GetWindow()->GetSize();
	_editorCamera = _editorScene->CreateGameObject(true);
	_editorCamera->addComponent<fmc::CCamera>(size.x, size.y,
		fmc::RENDER_MODE::FORWARD, false /*ortho*/, false/*auto*/, fm::Application::Get().GetWindow()->GetMSAA())->Init();
	_editorCamera->get<fmc::CTransform>()->SetPosition(fm::math::vec3(0, 0, -1));
	_editorCamera->SetName("Camera");
}

MainWindow::~MainWindow()
{
}

void MainWindow::_DisplayWindow_Create_Scene()
{
	auto scene = Editor::Get().GetCurrentScene();
	if (scene != nullptr)
	{
		scene->Save();
	}
	
	pfd::save_file dialog = pfd::save_file("Create to...", "newScene.scene");
	std::string&& resultFromDialog = dialog.result();

	if (!resultFromDialog.empty())
	{
		fm::FilePath result(resultFromDialog);

		Editor::Get().CreateNewScene(result);
	}
}


bool MainWindow::IsWindowAvailable(gui::WINDOWS inWindow)
{
	gui::GWindow* window = _windows[inWindow].get();
	return window != nullptr;
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
					Editor::Get().Start();
					_currentEntity.reset();
				}
				if (ImGui::MenuItem("Pause"))
				{
					//_engine->Stop();
				}
				if (ImGui::MenuItem("Stop"))
				{
					Editor::Get().Stop();
					_currentEntity.reset();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Save"))
			{

				if (ImGui::MenuItem("Scene"))
				{
					auto scene = Editor::Get().GetCurrentScene();

					if (scene != nullptr && scene->GetName() == scene->GetPath().GetPath())
					{
						pfd::save_file dialog = pfd::save_file("Save to...", scene->GetName() + ".scene");
						std::string&& resultFromDialog = dialog.result();

						if (!resultFromDialog.empty())
						{
							fm::FilePath result(resultFromDialog);
							fm::FilePath p = fm::ResourcesManager::GetFilePathResource(fm::LOCATION::USER_LOCATION);

							std::string relative;
							fm::FilePath::GetRelativeFromRoot(p, result, relative);

							if (!relative.empty())
								result = fm::FilePath(fm::LOCATION::USER_LOCATION, relative);

							scene = Editor::Get().RenameScene(scene, result);
							Editor::Get().SetCurrentScene(scene->GetName());
						}
					}

					Editor::Get().SerializeCurrentScene();
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("New"))
			{
				//if (ImGui::MenuItem("Project"))
				//{
				//	_DisplayWindow_Create_Project();
				//}
				if (ImGui::MenuItem("Scene"))
				{
					_DisplayWindow_Create_Scene();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Load"))
			{
				//if (ImGui::MenuItem("Project ..."))
				//{
				//	_DisplayWindow_Load_Project();
				//}
				if (ImGui::MenuItem("Scene ..."))
				{
					pfd::open_file dialog = pfd::open_file("Choose files to read", ".",
						{ "scene files", "*.scene",
						  "All Files", "*" },
						false);

					std::vector<std::string> resultFromDialog = dialog.result();

					if (!resultFromDialog.empty())
					{
						fm::FilePath result(resultFromDialog.front());
						auto s = Editor::Get().LoadScene(result);

						if (s != nullptr)
						{
							Editor::Get().SetCurrentScene(s->GetName());
						}
					}
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Export"))
			{

				pfd::select_folder dialog = pfd::select_folder("Choose where to export", ".");

				std::string resultFromDialog = dialog.result();

				if (!resultFromDialog.empty())
				{
					fm::FilePath result(resultFromDialog);
					gui::ExportManager::ExportSettings settings;
					settings.destination = result;
					gui::ExportManager exportm(settings);
					exportm.Run();
				}
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
				_currentEntity = Editor::Get().GetScene(_context.currentSceneName)->CreateGameObject(true)->getID();

			}
			if (ImGui::MenuItem("List entity"))
			{
				_SetWindowPosition(_windows[gui::WINDOWS::WIN_LIST_ENTITIES].get(), MainWindow::dock_right_id);
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
		std::shared_ptr<fm::GameObject> go = Editor::Get().GetCurrentScene()->GetGameObjectByID(_currentEntity.value());
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
				std::shared_ptr <fm::GameObject> go = Editor::Get().GetScene(_context.currentSceneName)->CreateGameObject(false);
				go->Read(json);
			}
		}
	}

}


void MainWindow::OnUpdate(bool hasFocus, bool force)
{

	if (!force)
	{
		if(!_events.empty())
		{
			_events.front()();
			_events.pop();
		}
	}

	if (hasFocus && !_hasFocus)
	{
		fm::FilePath path(fm::LOCATION::USER_LOCATION, "");
		_RefreshResources(path);
	}
	_hasFocus = hasFocus;
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

	if (_context.currentWindowToDisplay.has_value())
	{
		_SetWindowPosition(_windows[_context.currentWindowToDisplay.value()].get(), MainWindow::dock_right_id);
		_context.currentWindowToDisplay = std::nullopt;
	}

}

void MainWindow::_AddDock(gui::WINDOWS inWindow, ImGuiID inID)
{
	
	if (_windows[inWindow] != nullptr)
	{
		ImGui::DockBuilderDockWindow(_windows[inWindow]->GetTitle().c_str(), inID);
	}
}



void MainWindow::OnDraw()
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
	window_flags |= ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));


	dockspace_id = ImGui::GetID("MyDockspace");

	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	if (!ImGui::DockBuilderGetNode(dockspace_id))
	{
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

		_docks[DOCK_ID::dock_main_id] = dock_main_id;
		_docks[DOCK_ID::dock_up_id] = dock_up_id;
		_docks[DOCK_ID::dock_right_id] = dock_right_id;
		_docks[DOCK_ID::dock_left_id] = dock_left_id;
		_docks[DOCK_ID::dock_left_right_id] = dock_left_right_id;
		_docks[DOCK_ID::dock_down_id] = dock_down_id;
		_docks[DOCK_ID::dock_down_right_id] = dock_down_right_id;

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

	for (auto& window : _windows)
	{
		if (window.second != nullptr)
		{
			window.second->Draw();
		}
	}



	if (_needUpdate)
	{
		OnUpdate(_hasFocus, true);
	}



	if (!_windows[gui::WINDOWS::WIN_LAUNCHER]->IsEnabled())
	{
		_DrawMenu();
	}

	ImGuiIO io = ImGui::GetIO();
	if (_context.currentWindowFocused == gui::WINDOWS::WIN_EDITOR_VIEW)
	{
		if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_C, false))
		{
			_Copy();
		}
		if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_V, false))
		{
			_Paste();
		}
		if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_S, false))
		{
			Editor::Get().SerializeCurrentScene();
		}
	}

	ImGui::End();
	
}

void MainWindow::_SetWindowPosition(gui::GWindow* window, DOCK_ID inID)
{
	if (!window->IsEnabled())
	{
		window->Start();
		ImGui::DockBuilderDockWindow(window->GetTitle().c_str(), _docks[inID]);
	}

	ImGui::SetWindowFocus(window->GetTitle().c_str());
}


void MainWindow::_ClearBeforeSceneChange()
{
	std::string currentSceneName = _context.currentSceneName;

	if (IsWindowAvailable(gui::WINDOWS::WIN_SCENE_VIEW))
	{
		_windows[gui::WINDOWS::WIN_SCENE_VIEW]->AddEvent([](gui::GWindow* inWindow, std::optional<gui::Context> Context) {
			dynamic_cast<gui::GGameView*>(inWindow)->Clear();
		});
	}

	if (IsWindowAvailable(gui::WINDOWS::WIN_INSPECTOR))
	{
		_windows[gui::WINDOWS::WIN_INSPECTOR]->AddEvent([currentSceneName](gui::GWindow* inWindow, std::optional<gui::Context> Context) {
			dynamic_cast<gui::GListComponent*>(inWindow)->OnBeforeLoad(currentSceneName);
		});
	}

	if (IsWindowAvailable(gui::WINDOWS::WIN_LIST_ENTITIES))
	{
		_windows[gui::WINDOWS::WIN_LIST_ENTITIES]->AddEvent([currentSceneName](gui::GWindow* inWindow, std::optional<gui::Context> Context) {
			dynamic_cast<gui::GListEntities*>(inWindow)->OnBeforeLoad(currentSceneName);
			});
	}

	_currentEntity.reset();
	_context.currentGameObjectSelected.reset();
	_context.currentSceneName = "";


	_needUpdate = true;
}


void MainWindow::_InitGameView()
{
	std::shared_ptr<fm::Scene> currentScene = Editor::Get().GetScene(_context.currentSceneName);

	auto&& v = currentScene->GetAllGameObjects();
	for (auto &&o : v)
	{
		std::shared_ptr<fm::GameObject> go = o.second;
		if (o.second->has<fmc::CCamera>())
		{
			if (IsWindowAvailable(gui::WINDOWS::WIN_SCENE_VIEW))
			{
				_windows[gui::WINDOWS::WIN_SCENE_VIEW]->AddEvent([go](gui::GWindow* inWindow, std::optional<gui::Context> Context) {
					dynamic_cast<gui::GGameView*>(inWindow)->AddCamera(go);
				});
			}
			break;
		}
	}
}

void MainWindow::Notify(fm::Observable* o, const fm::EventObserver& inEvent)
{
	if (o != nullptr && o->GetName() == "Editor")
	{
		switch ((Editor::Event)inEvent.eventKind)
		{
		case Editor::Event::ON_AFTER_LOAD:
			_OnAfterLoad(inEvent.value);
			break;

		case Editor::Event::ON_AFTER_START:
			_OnAfterStart(inEvent.value);
			break;

		case Editor::Event::ON_AFTER_STOP:
			_OnAfterStop(inEvent.value);
			break;

		case Editor::Event::ON_PRE_LOAD:
			_OnPreLoad(inEvent.value);
			break;

		case Editor::Event::ON_PRE_START:
			_OnPreStart(inEvent.value);
			break;

		case Editor::Event::ON_PRE_STOP:
			_OnPreLoad(inEvent.value);
			break;

		case Editor::Event::ON_PRE_SCENE_LOAD:
			_OnPreStop(inEvent.value);
			break;

		case Editor::Event::ON_AFTER_SCENE_LOAD:
			_OnAfterLoad(inEvent.value);
			break;

		default:
			break;
		}
	}
}



void MainWindow::_OnPreStart(const std::any& inAny)
{
	_events.push([this, inAny]() {
		if (inAny.has_value())
			std::any_cast<std::function<void(void)>>(inAny)();
		_ClearBeforeSceneChange();

		});

}
void MainWindow::_OnAfterStart(const std::any& inAny)
{
	_events.push([this, inAny]() {
		if (inAny.has_value())
			std::any_cast<std::function<void(void)>>(inAny)();
		_context.currentSceneName = Editor::Get().GetCurrentSceneName();
		_InitGameView();
		_needUpdate = true;
		std::string currentSceneName = _context.currentSceneName;
		if (IsWindowAvailable(gui::WINDOWS::WIN_LIST_ENTITIES))
		{
			_windows[gui::WINDOWS::WIN_LIST_ENTITIES]->AddEvent([currentSceneName](gui::GWindow* inWindow, std::optional<gui::Context> Context) {
				dynamic_cast<gui::GListEntities*>(inWindow)->OnAfterLoad(currentSceneName);
				});
		}
		});

	
}
void MainWindow::_OnPreStop(const std::any& inAny)
{
	_events.push([this, inAny]() {
		if (inAny.has_value())
			std::any_cast<std::function<void(void)>>(inAny)();
		_ClearBeforeSceneChange();
		});
}
void MainWindow::_OnAfterStop(const std::any& inAny)
{

	_events.push([this, inAny]() {
		if (inAny.has_value())
			std::any_cast<std::function<void(void)>>(inAny)();
		_context.currentSceneName = Editor::Get().GetCurrentSceneName();
		_InitGameView();
		_needUpdate = true;
		std::string currentSceneName = _context.currentSceneName;
		if (IsWindowAvailable(gui::WINDOWS::WIN_LIST_ENTITIES))
		{
			_windows[gui::WINDOWS::WIN_LIST_ENTITIES]->AddEvent([currentSceneName](gui::GWindow* inWindow, std::optional<gui::Context> Context) {
				dynamic_cast<gui::GListEntities*>(inWindow)->OnAfterLoad(currentSceneName);
				});
		}
		}
	);

}

void MainWindow::_OnPreLoad(const std::any& inAny)
{
	_events.push([this, inAny]() {
		if (inAny.has_value())
			std::any_cast<std::function<void(void)>>(inAny)();
		_ClearBeforeSceneChange();
		});
}

void MainWindow::_AfterLoad()
{
	_context.currentSceneName = Editor::Get().GetCurrentSceneName();
	_InitGameView();

	if (IsWindowAvailable(gui::WINDOWS::WIN_INSPECTOR))
	{
		std::string currentSceneName = _context.currentSceneName;
		_windows[gui::WINDOWS::WIN_INSPECTOR]->AddEvent([currentSceneName](gui::GWindow* inWindow, std::optional<gui::Context> Context) {
			dynamic_cast<gui::GListComponent*>(inWindow)->OnAfterLoad(currentSceneName);
			});
	}
	if (IsWindowAvailable(gui::WINDOWS::WIN_LIST_ENTITIES))
	{
		std::string currentSceneName = _context.currentSceneName;
		_windows[gui::WINDOWS::WIN_LIST_ENTITIES]->AddEvent([currentSceneName](gui::GWindow* inWindow, std::optional<gui::Context> Context) {
			dynamic_cast<gui::GListEntities*>(inWindow)->OnAfterLoad(currentSceneName);
			});
	}

	_needUpdate = true;
}


void MainWindow::_OnAfterLoad(const std::any& inAny)
{
	_events.push([this, inAny]() {
		if (inAny.has_value())
			std::any_cast<std::function<void(void)>>(inAny)();
		_AfterLoad();
		});
}

void MainWindow::_ConfigureStyle()
{
	// From https://github.com/ocornut/imgui/issues/707#issuecomment-254610737
	// Some colors may have changed
	// Setup style

	ImGuiStyle& style = ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO();

	fm::FilePath p = fm::ResourcesManager::GetFilePathResource(fm::LOCATION::INTERNAL_FONT_LOCATION);
	p.ToSub("Roboto-Medium.ttf");
	io.Fonts->AddFontFromFileTTF(p.GetPathString().c_str(), 14.0f);
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
	//style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
}
