
#include "MainWindow.h"
#include "Components/CCamera.h"
#include <Time.h>
#include "Components/CMaterial.h"
#include "Components/Body2D.h"
#include "Components/CDirectionalLight.h"
#include "Engine.h"
#include <Components/CTransform.h>
#include <Components/CMaterial.h>
#include <Components/CPointLight.h>
#include "Rendering/RenderingSystem.h"
#include "inspector/Inspector.hpp"
#include "inspector/TransformInspector.h"
#include "inspector/MaterialInspector.h"
#include "inspector/MeshInspector.h"
#include "Core/SceneManager.h"
#include "Core/Debug.h"
#include "inspector/scriptmanagerinspector.hpp"
#include "inspector/pointlightinspector.h"
#include "Core/Debug.h"
#include "Core/Scene.h"
#include "Core/GameObject.h"
#include "dialogfilebrowser.h"
#define WITH_VIEW 1

MainWindow::MainWindow(fm::Engine* engine) {
    fm::Debug::logWarning("Start init");
    _configureStyle();
    _engine = engine;

    _dlight = EntityManager::get().createEntity();
    _dlight->addComponent<fmc::CDirectionalLight>(fm::Color(0.3, 0.3, 0.3, 1));
    _dlight->addComponent<fmc::CTransform>(fm::math::Vector3f(100, 50,0), fm::math::Vector3f(20, 20, 20), fm::math::Vector3f(0, 0, 0), 1);
    _dlight->addComponent<fmc::CMaterial>();


    fm::SceneManager::get().addScene(new fm::Scene("newScene"));
    fm::SceneManager::get().setCurrentScene("newScene");

    _mainCamera = fm::GameObjectHelper::create();
#if WITH_VIEW
    fmc::CCamera *tempRefCamera = _mainCamera->addComponent<fmc::CCamera>(1280, 720, fmc::RENDER_MODE::FORWARD, false,8);
#else
    fmc::CCamera *tempRefCamera = _mainCamera->addComponent<fmc::CCamera>(fm::Window::width, fm::Window::height, fmc::RENDER_MODE::FORWARD, false,4);
#endif
    _mainCamera->addComponent<fmc::CTransform>();


    _mainCamera->name = "Camera";
    _engine->SetMainCamera(_mainCamera);
#if WITH_VIEW
    _gameView.renderTexture = std::make_shared<fm::RenderTexture>(fm::RenderTexture(*tempRefCamera->getInternalRenderTexture().get(), 0));
    tempRefCamera->target = _gameView.renderTexture;
#endif
    fm::Debug::log("Init done");

    for(size_t i = 0; i < WIN_LAST; ++i)
        _windowStates[i] = false;
}

void MainWindow::displayComponents(fm::GameObject* currentEntity) {

    std::vector<BaseComponent*> compos = currentEntity->getAllComponents();
    if(_inspectorComponents.find(currentEntity->getID()) == _inspectorComponents.end())
    {
        std::unordered_map<size_t, Inspector*> inspectors;

        _inspectorComponents.insert(
                std::pair<size_t, std::unordered_map<size_t, Inspector*>>(
                    currentEntity->getID(), inspectors));
    }

    for(auto c : compos) {
        if(_inspectorComponents[currentEntity->getID()][c->GetType()] == nullptr)
        {
            if(c->GetType()== fmc::ComponentType::kTransform)
            {
                _inspectorComponents[currentEntity->getID()][c->GetType()] = new gui::TransformInspector(c);
            } else if(c->GetType()== fmc::ComponentType::kMaterial)
            {
                _inspectorComponents[currentEntity->getID()][c->GetType()] = new gui::MaterialInspector(c);
            }
            else if(c->GetType()== fmc::ComponentType::KMesh)
            {
                _inspectorComponents[currentEntity->getID()][c->GetType()] = new gui::MeshInspector(c);
            }
            else if(c->GetType()== fmc::ComponentType::kScriptManager)
            {
                _inspectorComponents[currentEntity->getID()][c->GetType()] = new gui::ScriptManagerInspector(c);
            }
            else if(c->GetType()== fmc::ComponentType::kPointLight)
            {
                _inspectorComponents[currentEntity->getID()][c->GetType()] = new gui::PointLightInspector(c);
            }
        } else {
            bool value = true;
            _inspectorComponents[currentEntity->getID()][c->GetType()]->draw(&value);
            if(!value)
            {
                delete _inspectorComponents[currentEntity->getID()][c->GetType()];
                _inspectorComponents[currentEntity->getID()][c->GetType()] = nullptr;
                c->Destroy();

            }
        }
    }
}

void MainWindow::DisplayWindow_Save()
{

    ImGui::Begin("Project name", &_windowStates[WIN_CREATE_PROJECT], ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("%s", _projectSettings.name.c_str());

    static char bufferName[256];
    ImGui::InputText("Poject name", bufferName, 256);

    static char path[PATH_MAX];
    ImGui::InputText("##Path", path, PATH_MAX);
    ImGui::SameLine();
    if(ImGui::Button("Location") || _windowStates[WIN_FILE_BROWSER_LOCATION])
    {
        _windowStates[WIN_FILE_BROWSER_LOCATION] = true;
        DialogFileBrowser::Get().Import(".", "Test",&_windowStates[WIN_FILE_BROWSER_LOCATION]);
        //std::cout << _windowStates[WIN_FILE_BROWSER_LOCATION] << std::endl;
        if(DialogFileBrowser::Get().IsValid())
        {
            std::string result = DialogFileBrowser::Get().GetResult();

            _projectSettings.path = result + _projectSettings.name + "/";
            _projectSettings.resourcesFolder = result + _projectSettings.name + "/" + "Resources" + "/";

            strcpy(path, _projectSettings.path.c_str());
        }
    }

    if(ImGui::Button("Valid"))
    {
        _projectSettings.name = std::string(bufferName);
        _windowStates[WIN_PROJECT_SETTINGS] = false;

        CreateFolder(_projectSettings.path.c_str());
        CreateFolder(_projectSettings.resourcesFolder.c_str());


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


void MainWindow::menu() {
    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("File"))
        {
            if(ImGui::BeginMenu("Run"))
            {
                if(ImGui::MenuItem("Start"))
                {
                    _engine->Start();
                }
                if(ImGui::MenuItem("Pause"))
                {
                    _engine->Stop();
                }
                if(ImGui::MenuItem("Stop"))
                {
                    _engine->Reset();
                }
                ImGui::EndMenu();
            }
            if(ImGui::MenuItem("Project Settings"))
            {
                _windowStates[WIN_PROJECT_SETTINGS] = true;
            }
            if(ImGui::MenuItem("Save"))
            {
                _windowStates[WIN_CREATE_PROJECT] = true;
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
        _engine->SetMainCamera(_mainCamera);
    }

}

void MainWindow::displayComponentsAvailable() {
}

void MainWindow::menuEntity()
{

    std::string nameWindowInspector("Inspector");

    if(_currentEntity && _currentEntity->IsActive())
    {
         nameWindowInspector += std::to_string(_currentEntity->getID());
    }


    ImGui::SetNextWindowPos(ImVec2(0,20));
    ImGui::SetNextWindowSize(ImVec2(300, fm::Window::height-20));
    ImGui::Begin(nameWindowInspector.c_str(),&_windowCurrentEntity, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    if(_currentEntity && _currentEntity->IsActive())
    {
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
            ImGui::EndPopup();
        }
    }
    ImGui::End();

}

void MainWindow::listEntity()
{
    if(_windowListEntity)
    {
        static std::vector<const char*> namesEntities;
        ImGui::SetNextWindowPos(ImVec2(fm::Window::width - 256,20));
        ImGui::SetNextWindowSize(ImVec2(256, fm::Window::height-20));
        ImGui::Begin("List entities", &_windowListEntity, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

        if(_timerListEntityUpdate > 1)
        {
            namesEntities = getAllEntities();
            _timerListEntityUpdate = 0;
        } else
        {
            _timerListEntityUpdate += fm::Time::dt;
        }
        static size_t number = 0;

        for (size_t i = 0; i < namesEntities.size(); i++)
        {
            fm::GameObject *o = fm::SceneManager::get().getCurrentScene()->GetGameObject(number);
            fmc::CTransform *transform = o->get<fmc::CTransform>();
            // Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
            ImGuiTreeNodeFlags node_flags = (transform->idFather !=-1) ?  ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow : 0
                    | ImGuiTreeNodeFlags_Selected
                    ;

            // Node
            bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, namesEntities[i], i);
            if (ImGui::IsItemClicked())
                number = i;
            if (node_open)
            {
                //ImGui::Text("Blah blah\nBlah Blah");
                ImGui::TreePop();
            }


        }
        _currentEntity = fm::SceneManager::get().getCurrentScene()->getAllGameObjects()[number];


        if(ImGui::Button("Add Entity"))
        {
            _currentEntity = fm::GameObjectHelper::create();
            _currentEntity->addComponent<fmc::CTransform>(fm::math::Vector3f(0, 0, 0),
                                                          fm::math::Vector3f(1, 1, 1),
                                                          fm::math::vec3(0,0,0));
        }
        ImGui::End();
    }
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

void MainWindow::draw() {
    bool show_test_window = true;
    menu();
    menuEntity();
    listEntity();
    if(_windowStates[WIN_CREATE_PROJECT])
        DisplayWindow_Save();

    if(_windowStates[WIN_PROJECT_SETTINGS])
        DisplayWindow_ProjectSettings();

    if(_windowStates[WIN_LIGHT_EDIT])
        DisplayWindow_WorldLighEdit();

    ImGui::SetNextWindowPos(ImVec2(650, 300), ImGuiCond_FirstUseEver);
    ImGui::ShowDemoWindow(&show_test_window);




    if(ImGui::GetIO().MouseClicked[1])
    {
        if(!_firstRightClick)
        {
            _firstPosMouseRightClick = ImGui::GetIO().MousePos;
            _firstRightClick = true;
        }
    }

    if(ImGui::GetIO().MouseDown[1])
    {
        if(_firstRightClick) {
            //_mainCameraEditorPosition->position.x += (_firstPosMouseRightClick.x - ImGui::GetIO().MousePos.x) * _coeffMouseSpeed;
            //_mainCameraEditorPosition->position.y += (_firstPosMouseRightClick.y - ImGui::GetIO().MousePos.y) * _coeffMouseSpeed;
            _firstPosMouseRightClick = ImGui::GetIO().MousePos;
        }
    }
    else
    {
        _firstRightClick = false;
    }

#if WITH_VIEW
    _gameView.draw();
#endif

    if(_windowStates[WIN_LOGGER])
    {
        std::vector<fm::Debug::Message> messages = fm::Debug::get().Flush();
        for(size_t i = 0; i < messages.size(); ++i)
            _debugLogger.AddLog(messages[i]);
        _debugLogger.Draw("Logger");
    }

}

void MainWindow::_configureStyle()
{
    // From https://github.com/ocornut/imgui/issues/707#issuecomment-254610737
    // Some colors may have changed
    // Setup style

    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto-Medium.ttf", 14.0f);
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
