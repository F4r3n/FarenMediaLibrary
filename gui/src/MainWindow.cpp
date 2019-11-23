
#include "MainWindow.h"
#include "Components/CCamera.h"
#include <TimeDef.h>
#include "Components/CMaterial.h"
#include "Components/Body2D.h"
#include "Components/CDirectionalLight.h"
#include "Engine.h"
#include <Components/CTransform.h>
#include <Components/CMaterial.h>
#include <Components/CPointLight.h>
#include "Rendering/RenderingSystem.h"
#include "inspector/inspector.hpp"
#include "inspector/transformInspector.hpp"
#include "inspector/materialInspector.hpp"
#include "inspector/meshInspector.hpp"
#include "Core/SceneManager.h"
#include "Core/Debug.h"
#include "inspector/scriptManagerInspector.hpp"
#include "inspector/pointLightInspector.hpp"
#include "Core/Debug.h"
#include "Core/Scene.h"
#include "Core/GameObject.h"
#include "dialogfilebrowser.h"
#include "Core/application.h"
#include "Resource/ResourcesManager.h"
#include "SaveProjectWindow.h"
#include "inspector/body3DInspector.hpp"
#include "Window.h"
#include "imgui/imgui_internal.h"
#include "GListEntities.h"
#include "PortableFileDialog.h"
#include "PickingSystem.h"
#include <functional>
#include "Window.h"
#include "ImGuizmo/ImGuizmo.h"
#include "ToolBar.hpp"
#include "debuglogger.h"
#include "EditorView.h"
#include <imgui/imgui_internal.h>
#include "Components/CCollider.h"
#include "inspector/colliderInspector.hpp"


MainWindow::MainWindow()
{
	_currentEntity = nullptr;
	_dlight = nullptr;
	_context.currentGameObjectSelected = nullptr;
	for (size_t i = 0; i < WIN_LAST; ++i)
	{
		_windowStates[i] = false;
	}


	fm::Debug::logWarning("Start init");
	_ConfigureStyle();

	_editorScene = fm::SceneManager::get().AddPrivateScene("_editor");
	fm::SceneManager::get().AddNewScene("newScene");
	fm::SceneManager::get().setCurrentScene("newScene", false);

	_InitEditorCamera();

	std::unique_ptr<gui::EditorView> gameView = std::make_unique<gui::EditorView>(_editorCamera, _editorScene);
	gameView->SetPickingSystem(new fms::PickingSystem( _editorScene));


	_windows[WINDOWS::WIN_LIST_ENTITIES] = std::make_unique<gui::GListEntities>();
	_windows[WINDOWS::WIN_EDITOR_VIEW] = std::move(gameView);
	_windows[WINDOWS::WIN_LOGGER] = std::make_unique<gui::DebugLogger>();
	_windows[WINDOWS::WIN_TOOLBAR] = std::make_unique<gui::ToolBar>();
	fm::Debug::log("Init done");

}

void MainWindow::_InitEditorCamera()
{
	_editorCamera = fm::GameObjectHelper::create(_editorScene);
	_editorCamera->addComponent<fmc::CCamera>(fm::Window::kWidth, fm::Window::kHeight,
		fmc::RENDER_MODE::FORWARD, false /*ortho*/, true/*auto*/, fm::Application::Get().GetWindow()->GetMSAA())->Init();
	_editorCamera->addComponent<fmc::CTransform>();
	_editorCamera->get<fmc::CTransform>()->position = fm::math::vec3(0, 0, -1);
	_editorCamera->name = "Camera";
}

MainWindow::~MainWindow()
{
}


void MainWindow::_ClearInspectorComponents()
{
	for (auto&& entities : _inspectorComponents)
	{
		entities.second.clear();
	}
	_inspectorComponents.clear();
}


void MainWindow::_DrawComponents(fm::GameObject* currentEntity)
{

	std::vector<BaseComponent*> &&compos = currentEntity->getAllComponents();
	if (_inspectorComponents.find(currentEntity->getID()) == _inspectorComponents.end())
	{
		_inspectorComponents[currentEntity->getID()] = std::unordered_map<size_t, std::unique_ptr<Inspector>>();
	}
	auto &&inspectorComponent = _inspectorComponents[currentEntity->getID()];

	for (auto && c : compos) 
	{
		uint16_t componentType = c->GetType();
		if (inspectorComponent[componentType] == nullptr)
		{
			if (componentType == fmc::ComponentType::kTransform)
			{
				inspectorComponent[componentType] = std::make_unique<gui::TransformInspector>(c);
			}
			else if (componentType == fmc::ComponentType::kMaterial)
			{
				inspectorComponent[componentType] = std::make_unique <gui::MaterialInspector>(c);
			}
			else if (componentType == fmc::ComponentType::KMesh)
			{
				inspectorComponent[componentType] = std::make_unique <gui::MeshInspector>(c);
			}
			else if (componentType == fmc::ComponentType::kScriptManager)
			{
				inspectorComponent[componentType] = std::make_unique <gui::ScriptManagerInspector>(c);
			}
			else if (componentType == fmc::ComponentType::kPointLight)
			{
				inspectorComponent[componentType] = std::make_unique <gui::PointLightInspector>(c);
			}
			else if (componentType == fmc::ComponentType::kBody3D)
			{
				inspectorComponent[componentType] = std::make_unique <gui::Body3DInspector>(c);
			}
			else if (componentType == fmc::ComponentType::kCollider)
			{
				inspectorComponent[componentType] = std::make_unique <gui::ColliderInspector>(c);
			}
		}
		else
		{
			bool value = true;

			inspectorComponent[componentType]->draw(&value);

			if (!value)
			{
				inspectorComponent[componentType].reset();
				inspectorComponent[componentType] = nullptr;
				c->Destroy();
			}
		}
	}
}


void MainWindow::_DisplayWindow_Save()
{
	pfd::select_folder dialog = pfd::select_folder("Save to...", ".");
	std::string&& resultFromDialog = dialog.result();

	if (!resultFromDialog.empty())
	{
		fm::FilePath result(resultFromDialog);
		
		fm::Application::Get().SetUserDirectory(result);
		fm::Application::Get().Serialize(_editorScene);
	}
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
		fm::Application::Get().SetProjectName(result.GetName(true));
		fm::Application::Get().SetUserDirectory(result.GetParent());

		fm::SceneManager::get().ClearAllPublic();
		_ClearInspectorComponents();
		_currentEntity = nullptr;

		fm::Application::Get().Read();
	}
}


void MainWindow::_DisplayWindow_ProjectSettings()
{
	ImGui::Begin("Project name", &_windowStates[WINDOWS::WIN_PROJECT_SETTINGS], ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("%s", _projectSettings.name.c_str());

	static char bufferName[256];
	ImGui::InputText("Poject name", bufferName, 256);
	if (ImGui::Button("Valid"))
	{
		_projectSettings.name = std::string(bufferName);
		_windowStates[WINDOWS::WIN_PROJECT_SETTINGS] = false;
		ImGui::CloseCurrentPopup();
	}
	ImGui::End();
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
					_currentEntity = nullptr;
				}
				if (ImGui::MenuItem("Pause"))
				{
					//_engine->Stop();
				}
				if (ImGui::MenuItem("Stop"))
				{
					fm::Application::Get().Stop();
					_currentEntity = nullptr;
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
					fm::Application::Get().Serialize(fm::SceneManager::get().getCurrentScene());
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
			bool enable = _windows[WINDOWS::WIN_LOGGER]->IsEnabled();
			if (ImGui::MenuItem("Logger", "L", &enable, true))
			{
				if (enable)
				{
					_windows[WINDOWS::WIN_LOGGER]->Start();
				}
				else
				{
					_windows[WINDOWS::WIN_LOGGER]->Stop();
				}
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Cameras"))
		{
			_DrawListCamera();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Entity"))
		{
			if (ImGui::MenuItem("Create"))
			{
				_currentEntity = fm::GameObjectHelper::create();
				_currentEntity->addComponent<fmc::CTransform>(
					fm::math::Vector3f(0, 0, 0),
					fm::math::Vector3f(1, 1, 1),
					fm::math::vec3(0, 0, 0), 1);
			}
			if (ImGui::MenuItem("List entity"))
			{
				//_windowListEntity = true;
			}

			ImGui::EndMenu();
		}
	}

	ImGui::EndMainMenuBar();

}

void MainWindow::_DrawListCamera()
{
	static bool value1 = true;
	if (ImGui::MenuItem("Camera editor", "", &value1))
	{
		//_engine->SetMainCamera(_mainCamera);
	}

}

void MainWindow::_DrawComponentsAvailable() 
{
}

void MainWindow::_DrawMenuEntity()
{

	std::string nameWindowInspector("Inspector");

	bool shouldBeDisplayed = _currentEntity != nullptr;
	ImGui::Begin(nameWindowInspector.c_str(), &shouldBeDisplayed);

	if (_currentEntity && _currentEntity->IsActive())
	{
		ImGui::Text(_currentEntity->name.c_str());
		_DrawComponents(_currentEntity);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("popup from button");

		if (ImGui::BeginPopup("popup from button") && _currentEntity && _currentEntity->IsActive())
		{
			ImGui::MenuItem("Components", nullptr, false, false);

			if (!_currentEntity->has<fmc::CTransform>() && ImGui::MenuItem("Transform"))
			{
				_currentEntity->add<fmc::CTransform>();
			}
			if (!_currentEntity->has<fmc::CMesh>() && ImGui::MenuItem("Mesh"))
			{
				_currentEntity->add<fmc::CMesh>();
			}
			if (!_currentEntity->has<fmc::CMaterial>() && ImGui::MenuItem("Material"))
			{
				_currentEntity->add<fmc::CMaterial>();
			}
			if (!_currentEntity->has<fmc::CScriptManager>() && ImGui::MenuItem("ScriptManager"))
			{
				_currentEntity->add<fmc::CScriptManager>();
			}
			if (!_currentEntity->has<fmc::CPointLight>() && ImGui::MenuItem("PointLight"))
			{
				_currentEntity->add<fmc::CPointLight>();
			}
			if (!_currentEntity->has<fmc::CCollider>() && ImGui::MenuItem("Collider"))
			{
				_currentEntity->add<fmc::CCollider>();
			}
			if (!_currentEntity->has<fmc::CBody3D>() && ImGui::MenuItem("Body3D"))
			{
				_currentEntity->add<fmc::CBody3D>();
			}
			ImGui::EndPopup();
		}
	}
	ImGui::End();

}


void MainWindow::_DisplayWindow_WorldLighEdit()
{
	bool value = true;
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_FirstUseEver);
	ImGui::Begin("World light", &_windowStates[WIN_LIGHT_EDIT]);
	Inspector::OnDraw(_dlight->get<fmc::CDirectionalLight>(), &value);
	ImGui::End();
}

void MainWindow::Update()
{

	for (auto& window : _windows)
	{
		window.second->Update(fm::Time::dt, _context);
	}
	_currentEntity = _context.currentGameObjectSelected;
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
		//ImGuiID dock_right_right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.2f, nullptr, &dock_main_id);
		ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.8f, nullptr, &dock_main_id);
		ImGuiID dock_right_id = dock_main_id;

		ImGuiID dock_down_id = ImGui::DockBuilderSplitNode(dock_left_id, ImGuiDir_Down, 0.2f, nullptr, &dock_left_id);
		ImGuiID dock_down_right_id = ImGui::DockBuilderSplitNode(dock_right_id, ImGuiDir_Down, 0.5f, nullptr, &dock_right_id);

		ImGui::DockBuilderDockWindow(_windows[WINDOWS::WIN_TOOLBAR]->GetTitle().c_str(), dock_up_id);
		ImGui::DockBuilderDockWindow(_windows[WINDOWS::WIN_LIST_ENTITIES]->GetTitle().c_str(), dock_right_id);
		ImGui::DockBuilderDockWindow(_windows[WINDOWS::WIN_EDITOR_VIEW]->GetTitle().c_str(), dock_left_id);
		ImGui::DockBuilderDockWindow(_windows[WINDOWS::WIN_LOGGER]->GetTitle().c_str(), dock_down_id);
		ImGui::DockBuilderDockWindow("Inspector", dock_down_right_id);
		ImGui::DockBuilderDockWindow("Scene", dock_main_id);
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
		window.second->Draw();
	}

	_DrawMenu();
	_DrawMenuEntity();
	
	if (_windowStates[WIN_PROJECT_SETTINGS])
	{
		_DisplayWindow_ProjectSettings();
	}

	if (_windowStates[WIN_LIGHT_EDIT])
	{
		_DisplayWindow_WorldLighEdit();
	}

	ImGui::End();

}

void MainWindow::_ConfigureStyle()
{
	// From https://github.com/ocornut/imgui/issues/707#issuecomment-254610737
	// Some colors may have changed
	// Setup style

	ImGuiStyle& style = ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO();

	fm::FilePath p = fm::ResourcesManager::GetFilePathResource(fm::ResourcesManager::INTERNAL_FONT_LOCATION);
	p.Append("Roboto-Medium.ttf");
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
