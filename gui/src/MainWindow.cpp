
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
#include <iomanip>      // std::setw
#include "Resource/ResourcesManager.h"
#include "SaveProjectWindow.h"
#include "inspector/body3DInspector.hpp"
#include "Window.h"
#include "imgui/imgui_internal.h"


MainWindow::MainWindow() 
{
	_windows.insert(std::make_pair<size_t, std::unique_ptr<gui::SaveProjectWindow>>(WIN_CREATE_PROJECT,
		std::make_unique<gui::SaveProjectWindow>("SaveWindow", [this] {fm::Application::Get().Serialize(this->_editorScene); })));

    fm::Debug::logWarning("Start init");
    _configureStyle();

	_editorScene = fm::SceneManager::get().AddPrivateScene("_editor");
    fm::SceneManager::get().AddNewScene("newScene");
    fm::SceneManager::get().setCurrentScene("newScene", false);

    _mainCamera = fm::GameObjectHelper::create(_editorScene);
    _mainCamera->addComponent<fmc::CCamera>(fm::Window::kWidth, fm::Window::kHeight, fmc::RENDER_MODE::FORWARD, false, 4);

    _mainCamera->addComponent<fmc::CTransform>();
    _mainCamera->name = "Camera";

	_gameView.AddCamera(_mainCamera);
	_gameView.SetMainCamera(_mainCamera);


    fm::Debug::log("Init done");

    for(size_t i = 0; i < WIN_LAST; ++i)
        _windowStates[i] = false;


}

void MainWindow::_ClearInspectorComponents()
{
	for (auto& entities : _inspectorComponents) 
	{
		entities.second.clear();
	}
	_inspectorComponents.clear();
}


void MainWindow::displayComponents(fm::GameObject* currentEntity) {

    std::vector<BaseComponent*> &&compos = currentEntity->getAllComponents();
    if(_inspectorComponents.find(currentEntity->getID()) == _inspectorComponents.end())
    {

		_inspectorComponents[currentEntity->getID()] = std::unordered_map<size_t, std::unique_ptr<Inspector>>();
               
    }

    for(auto c : compos) {
        if(_inspectorComponents[currentEntity->getID()][c->GetType()] == nullptr)
        {
            if(c->GetType()== fmc::ComponentType::kTransform)
            {
                _inspectorComponents[currentEntity->getID()][c->GetType()] = std::make_unique<gui::TransformInspector>(c);
            } else if(c->GetType()== fmc::ComponentType::kMaterial)
            {
                _inspectorComponents[currentEntity->getID()][c->GetType()] = std::make_unique <gui::MaterialInspector>(c);
            }
            else if(c->GetType()== fmc::ComponentType::KMesh)
            {
                _inspectorComponents[currentEntity->getID()][c->GetType()] = std::make_unique <gui::MeshInspector>(c);
            }
            else if(c->GetType()== fmc::ComponentType::kScriptManager)
            {
                _inspectorComponents[currentEntity->getID()][c->GetType()] = std::make_unique <gui::ScriptManagerInspector>(c);
            }
            else if(c->GetType()== fmc::ComponentType::kPointLight)
            {
                _inspectorComponents[currentEntity->getID()][c->GetType()] = std::make_unique <gui::PointLightInspector>(c);
            }
			else if (c->GetType() == fmc::ComponentType::kBody3D)
			{
				_inspectorComponents[currentEntity->getID()][c->GetType()] = std::make_unique <gui::Body3DInspector>(c);
			}
        } 
		else 
		{
            bool value = true;

            _inspectorComponents[currentEntity->getID()][c->GetType()]->draw(&value);

            if(!value)
            {
				_inspectorComponents[currentEntity->getID()][c->GetType()].reset();
                _inspectorComponents[currentEntity->getID()][c->GetType()] = nullptr;
                c->Destroy();
            }
        }
    }
}

void MainWindow::DisplayWindow_Save()
{

    
}


void MainWindow::DisplayWindow_Load()
{
    ImGui::Begin("Project name##Load", &_windowStates[WIN_PROJECT_LOAD], ImGuiWindowFlags_AlwaysAutoResize);

    if(ImGui::Button("Location") || DialogFileBrowser::Get().IsVisible())
    {
        _windowStates[WIN_FILE_BROWSER_LOCATION] = true;
        DialogFileBrowser::Get().Import(".", "Load", &_windowStates[WIN_FILE_BROWSER_LOCATION]);


        if(DialogFileBrowser::Get().IsValid())
        {			
			_windowStates[WIN_FILE_BROWSER_LOCATION] = false;
			fm::FilePath result;

			DialogFileBrowser::Get().GetResult(_projectSettings.name, result);
			fm::Application::Get().SetProjectName(result.GetName(true));

			fm::Application::Get().SetUserDirectory(result);
        }
    }

	if (ImGui::Button("Valid"))
	{

		fm::SceneManager::get().ClearAllPublic();
		_ClearInspectorComponents();
		_currentEntity = nullptr;

		fm::Application::Get().Read();

		_windowStates[WIN_PROJECT_LOAD] = false;
		
		ImGui::CloseCurrentPopup();
	}

    
    ImGui::End();
}


void MainWindow::DisplayWindow_ProjectSettings()
{
    ImGui::Begin("Project name", &_windowStates[WIN_PROJECT_SETTINGS], ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("%s", _projectSettings.name.c_str());

    static char bufferName[256];
    ImGui::InputText("Poject name", bufferName, 256);
    if(ImGui::Button("Valid"))
    {
        _projectSettings.name = std::string(bufferName);
        _windowStates[WIN_PROJECT_SETTINGS] = false;
        ImGui::CloseCurrentPopup();
    }
    ImGui::End();
}


void MainWindow::DrawMenu() 
{
    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("File"))
        {
            if(ImGui::BeginMenu("Run"))
            {
                if(ImGui::MenuItem("Start"))
                {
					fm::Application::Get().Start(true);
					_currentEntity = nullptr;
					//_ClearInspectorComponents();
                }
                if(ImGui::MenuItem("Pause"))
                {
                    //_engine->Stop();
                }
                if(ImGui::MenuItem("Stop"))
                {
					fm::Application::Get().Stop();
					_currentEntity = nullptr;
					//_ClearInspectorComponents();

                }
                ImGui::EndMenu();
            }
            if(ImGui::MenuItem("Project Settings"))
            {
                _windowStates[WIN_PROJECT_SETTINGS] = true;
            }
            if(ImGui::BeginMenu("Save"))
            {
				if (ImGui::MenuItem("Save to ..."))
				{
					_windows[WIN_CREATE_PROJECT]->SetStatus(true);					
				}
				if (ImGui::MenuItem("Save"))
				{
					fm::Application::Get().Serialize(_editorScene);
				}

				ImGui::EndMenu();
            }

			if (ImGui::BeginMenu("Load"))
			{
				
				if (ImGui::MenuItem("Load from ..."))
				{
					_windowStates[WIN_PROJECT_LOAD] = true;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();

        }
        if(ImGui::BeginMenu("World"))
        {
            if(ImGui::MenuItem("Light"))
            {
                _windowStates[WIN_LIGHT_EDIT] = true;
            }
            ImGui::EndMenu();
        }
        //static bool debugLoggerMenu = _activateDebugLogger;
        if(ImGui::BeginMenu("Options")) {
            if(ImGui::MenuItem("Logger","L", &_windowStates[WIN_LOGGER]))
            {
                _windowStates[WIN_LOGGER] = true;
            }
            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Cameras"))
        {
            displayListCamera();
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Entity"))
        {
            if(ImGui::MenuItem("Create"))
            {
                _windowCurrentEntity = true;
                _currentEntity = fm::GameObjectHelper::create();
                _currentEntity->addComponent<fmc::CTransform>(
                            fm::math::Vector3f(0, 0, 0),
                            fm::math::Vector3f(1, 1, 1),
                            fm::math::vec3(0,0,0), 1);
            }
            if(ImGui::MenuItem("List entity")) {
                _windowListEntity = true;
            }

            ImGui::EndMenu();
        }
    }
    ImGui::EndMainMenuBar();
}

void MainWindow::displayListCamera()
{
    static bool value1 = true;
    if(ImGui::MenuItem("Camera editor", "", &value1))
    {
        //_engine->SetMainCamera(_mainCamera);
    }

}

void MainWindow::displayComponentsAvailable() {
}

void MainWindow::menuEntity()
{

    std::string nameWindowInspector("Inspector");

    /*if(_currentEntity && _currentEntity->IsActive())
    {
         nameWindowInspector = nameWindowInspector + " " + std::to_string(_currentEntity->getID()) + "###" + nameWindowInspector;
    }*/
	
    ImGui::Begin(nameWindowInspector.c_str(),&_windowCurrentEntity);

    if(_currentEntity && _currentEntity->IsActive())
    {
		ImGui::Text(_currentEntity->name.c_str());
        displayComponents(_currentEntity);

        if(ImGui::Button("Add Component"))
            ImGui::OpenPopup("popup from button");

        if(ImGui::BeginPopup("popup from button") && _currentEntity && _currentEntity->IsActive())
        {
            ImGui::MenuItem("Components", NULL, false, false);

            if(!_currentEntity->has<fmc::CTransform>() && ImGui::MenuItem("Transform"))
            {
                _currentEntity->add<fmc::CTransform>();
            }

            if(!_currentEntity->has<fmc::CMesh>() && ImGui::MenuItem("Mesh"))
            {
                _currentEntity->add<fmc::CMesh>();
            }

            if(!_currentEntity->has<fmc::CMaterial>() && ImGui::MenuItem("Material"))
            {
                _currentEntity->add<fmc::CMaterial>();
            }
            if(!_currentEntity->has<fmc::CScriptManager>() && ImGui::MenuItem("ScriptManager"))
            {
                _currentEntity->add<fmc::CScriptManager>();
            }
            if(!_currentEntity->has<fmc::CPointLight>() && ImGui::MenuItem("PointLight"))
            {
                _currentEntity->add<fmc::CPointLight>();
            }
			if (!_currentEntity->has<fmc::CBody3D>() && ImGui::MenuItem("Body3D"))
			{
				fmc::CTransform* t = _currentEntity->get<fmc::CTransform>();
				_currentEntity->add<fmc::CBody3D>(new fmc::CBody3D(t->scale));
			}
            ImGui::EndPopup();
        }
    }
    ImGui::End();

}

void MainWindow::listEntity()
{

	static std::vector<const char*> namesEntities;

	ImGui::Begin("List entities", &_windowListEntity);

	if (_timerListEntityUpdate > 1)
	{
		namesEntities = getAllEntities();
		_timerListEntityUpdate = 0;
	}
	else
	{
		_timerListEntityUpdate += fm::Time::dt;
	}
	static int number = -1;

	for (size_t i = 0; i < namesEntities.size(); i++)
	{
		fm::GameObject *o = fm::SceneManager::get().getCurrentScene()->GetGameObject(i);
		fmc::CTransform *transform = o->get<fmc::CTransform>();
		// Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
		ImGuiTreeNodeFlags node_flags = ((number == i) ? ImGuiTreeNodeFlags_Selected : 0);

		// Node
		//ImGui::PushStyleColor(ImGuiCol_HeaderSelected, (ImVec4)ImColor::HSV(0.9f, 0.8f, 0.8f));

		bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, namesEntities[i], i);

		if (ImGui::IsItemClicked())
			number = i;
		if (node_open)
		{
			//ImGui::Text("Blah blah\nBlah Blah");
			ImGui::TreePop();

		}

		//ImGui::PopStyleColor(1);
		


	}
	if (number != -1)
		_currentEntity = fm::SceneManager::get().getCurrentScene()->getAllGameObjects()[number];


	if (ImGui::Button("Add Entity"))
	{
		_currentEntity = fm::GameObjectHelper::create();
		_currentEntity->addComponent<fmc::CTransform>(fm::math::Vector3f(0, 0, 0),
			fm::math::Vector3f(1, 1, 1),
			fm::math::vec3(0, 0, 0));
	}

	ImGui::End();

}

std::vector<const char*> MainWindow::getAllEntities()
{
    std::vector<const char*> names;
    std::vector<fm::GameObject*> gos = fm::SceneManager::get().getCurrentScene()->getAllGameObjects();
    for(auto e : gos) {
        names.push_back(e->name.c_str());
    }
    return names;
}

void MainWindow::DisplayWindow_WorldLighEdit() {
    bool value = true;
    ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_FirstUseEver);
    ImGui::Begin("World light", &_windowStates[WIN_LIGHT_EDIT]);
    Inspector::OnDraw(_dlight->get<fmc::CDirectionalLight>(), &value);
    ImGui::End();
}

void MainWindow::Update()
{
	
}

void MainWindow::Draw() 
{
	bool p_open = true;
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::SetNextWindowBgAlpha(0.0f);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", &p_open, window_flags);
	ImGui::PopStyleVar(3);

	dockspace_id = ImGui::GetID("MyDockspace");

	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruDockspace;
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

	if (ImGui::DockBuilderGetNode(dockspace_id) == NULL)
	{
		ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
		ImGui::DockBuilderAddNode(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruDockspace); // Add empty node

		ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.20f, NULL, &dockspace_id);
		ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.20f, NULL, &dockspace_id);
		ImGuiID dock_id_center = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_None, 0.20f, NULL, &dockspace_id);

		ImGui::DockBuilderDockWindow("Inspector", dock_id_left);
		ImGui::DockBuilderDockWindow("List entities", dock_id_right);
		ImGui::DockBuilderDockWindow("Game View", dock_id_center);
		
		ImGui::DockBuilderFinish(dockspace_id);
	}

	for (auto& window : _windows)
	{
		window.second->Draw();
	}


    DrawMenu();
    menuEntity();
    listEntity();

    if(_windowStates[WIN_PROJECT_SETTINGS])
        DisplayWindow_ProjectSettings();

	if(_windowStates[WIN_LIGHT_EDIT])
		DisplayWindow_WorldLighEdit();

	if(_windowStates[WIN_PROJECT_LOAD])
		DisplayWindow_Load();

    if(_windowStates[WIN_LOGGER])
    {
        std::vector<fm::Debug::Message> messages = fm::Debug::get().Flush();
        for(size_t i = 0; i < messages.size(); ++i)
            _debugLogger.AddLog(messages[i]);
        _debugLogger.Draw("Logger");
    }
	_gameView.draw();


	ImGui::End();
}

void MainWindow::_configureStyle()
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
    style.WindowTitleAlign = ImVec2(0.5f,0.5f);

    style.Colors[ImGuiCol_Text]                  = ImVec4(0.73f, 0.73f, 0.73f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.26f, 0.26f, 0.26f, 0.95f);
    style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
    style.Colors[ImGuiCol_Border]                = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
    style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
    style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
    style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
    style.Colors[ImGuiCol_Button]                = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
    style.Colors[ImGuiCol_Header]                = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
    style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);

    style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
    style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.32f, 0.52f, 0.65f, 1.00f);
    style.Colors[ImGuiCol_ModalWindowDarkening]  = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
}
