#include "MainWindow.h"
#include "Components/CCamera.h"
#include <Time.h>
#include "Components/CMaterial.h"
#include "Components/Body2D.h"
#include "Components/CDirectionalLight.h"
#include "Engine.h"
#include "Components/CIdentity.h"
#include <Components/CTransform.h>
#include <Components/CMaterial.h>
#include <Components/CPointLight.h>
#include "Rendering/RenderingSystem.h"
#include "Inspector.hpp"
#include "TransformInspector.h"
#include "MaterialInspector.h"
#include "MeshInspector.h"
#include "Core/SceneManager.h"
#include "Core/Debug.h"



MainWindow::MainWindow(fm::Engine* engine) {
    fm::Debug::logWarning("Start init");
    this->engine = engine;
    // From https://github.com/ocornut/imgui/issues/707#issuecomment-254610737
    // Some colors may have changed
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text] = ImVec4(0.31f, 0.25f, 0.24f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
    style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.68f, 0.68f, 0.68f, 0.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.77f, 0.74f, 0.73f, 0.98f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 0.60f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.62f, 0.70f, 0.72f, 0.56f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.95f, 0.33f, 0.14f, 0.47f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.97f, 0.31f, 0.13f, 0.81f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.42f, 0.75f, 1.00f, 0.53f);
    style.Colors[ImGuiCol_TitleBgCollapsed] =
        ImVec4(0.40f, 0.65f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.32f, 0.47f, 0.76f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.74f, 0.74f, 0.94f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.40f, 0.62f, 0.80f, 0.15f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.39f, 0.64f, 0.80f, 0.30f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] =
        ImVec4(0.28f, 0.67f, 0.80f, 0.59f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] =
        ImVec4(0.25f, 0.48f, 0.53f, 0.67f);
    style.Colors[ImGuiCol_ComboBg] = ImVec4(0.89f, 0.98f, 1.00f, 0.99f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.48f, 0.47f, 0.47f, 0.71f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
    style.Colors[ImGuiCol_SliderGrabActive] =
        ImVec4(0.31f, 0.47f, 0.99f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(1.00f, 0.79f, 0.18f, 0.78f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.42f, 0.82f, 1.00f, 0.81f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.72f, 1.00f, 1.00f, 0.86f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.65f, 0.78f, 0.84f, 0.80f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.75f, 0.88f, 0.94f, 0.80f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.55f, 0.68f, 0.74f, 0.80f);
    style.Colors[ImGuiCol_Column] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.60f, 0.60f, 0.80f, 0.30f);
    style.Colors[ImGuiCol_ResizeGripHovered] =
        ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
    style.Colors[ImGuiCol_ResizeGripActive] =
        ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
    style.Colors[ImGuiCol_CloseButton] = ImVec4(0.41f, 0.75f, 0.98f, 0.50f);
    style.Colors[ImGuiCol_CloseButtonHovered] =
        ImVec4(1.00f, 0.47f, 0.41f, 0.60f);
    style.Colors[ImGuiCol_CloseButtonActive] =
        ImVec4(1.00f, 0.16f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered] =
        ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered] =
        ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 0.99f, 0.54f, 0.43f);
    style.Colors[ImGuiCol_ModalWindowDarkening] =
        ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

    style.Alpha = 1.0f;
    style.FrameRounding = 4;
    style.IndentSpacing = 12.0f;

    dlight = EntityManager::get().createEntity();
    dlight->addComponent<fmc::CDirectionalLight>(new fmc::CDirectionalLight(fm::Color(0.3, 0.3, 0.3, 1)));
    dlight->addComponent<fmc::CTransform>(new fmc::CTransform(fm::math::Vector3f(100, 50,0), fm::math::Vector3f(20, 20, 20), fm::math::Vector3f(0, 0, 0), 1));
    dlight->addComponent<fmc::CMaterial>();

    playImage = fm::Texture("assets/images/play_button.png");

    fm::SceneManager::get().addScene(new fm::Scene("newScene"));
    fm::SceneManager::get().setCurrentScene("newScene");

    mainCamera = fm::GameObjectHelper::create();
    fmc::CCamera *tempRefCamera = mainCamera->addComponent<fmc::CCamera>(new fmc::CCamera(fm::Window::width, fm::Window::height, fmc::RENDER_MODE::FORWARD, false));
    mainCameraPosition = mainCamera->addComponent<fmc::CTransform>();
    mainCamera->name = "Camera";
    engine->setMainCamera(mainCamera);
    gameView.renderTexture = std::make_shared<fm::RenderTexture>(fm::RenderTexture(*tempRefCamera->getInternalRenderTexture().get()));
    tempRefCamera->target = gameView.renderTexture;
    fm::Debug::log("Init done");
}

void MainWindow::displayComponents(fm::GameObject* currentEntity) {
    std::vector<BaseComponent*> compos = currentEntity->getAllComponents();
    
    if(_inspectorComponents.find(currentEntity->getID()) ==
            _inspectorComponents.end())
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
        } else {
            _inspectorComponents[currentEntity->getID()][c->GetType()]->draw();
        }
    }
}

void MainWindow::fileSystem_save_window() {
    ImGui::SetNextWindowPos(ImVec2(200, 20), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Save window", &fileSystem_save);
    static char nameScene[256] = {"scene"};
    ImGui::InputText("Save name", nameScene, 256);

    if(ImGui::Button("Save")) {
        nameCurrentScene = std::string(nameScene);
        fileSystem_save = false;
    }
    ImGui::SameLine();
    if(ImGui::Button("Cancel")) {
        fileSystem_save = false;
    }
    ImGui::End();
}

void MainWindow::menu() {
    if(ImGui::BeginMainMenuBar()) {
        if(ImGui::BeginMenu("File")) {
            if(ImGui::BeginMenu("Run")) {
                if(ImGui::MenuItem("Start")) {
                    engine->start();
                }
                if(ImGui::MenuItem("Pause")) {
                    engine->stop();
                }
                if(ImGui::MenuItem("Stop")) {
                    engine->reset();
                }
                ImGui::EndMenu();
            }
            if(ImGui::MenuItem("Scene")) {
                fileSystem_save = true;
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("World")) {
            if(ImGui::MenuItem("Light")) {
                window_WorldLightEdit = true;
            }
            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Cameras")) {
            displayListCamera();
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Entity")) {
            if(ImGui::MenuItem("Create")) {
                windowCurrentEntity = true;
                currentEntity = fm::GameObjectHelper::create();
                currentEntity->addComponent<fmc::CTransform>(
                        new fmc::CTransform(fm::math::Vector3f(0, 0, 0),
                            fm::math::Vector3f(100, 100, 100),
                            fm::math::vec3(0,0,0), 1));
                currentEntity->addComponent<fmc::CIdentity>();
            }
            if(ImGui::MenuItem("List entity")) {
                windowListEntity = true;
            }

            ImGui::EndMenu();
        }
    }
    ImGui::EndMainMenuBar();
}

void MainWindow::displayListCamera() {
    static bool value1 = true;
    static bool value2 = false;
    if(ImGui::MenuItem("Camera editor", "", &value1)) {
        value2 = false;
    engine->setMainCamera(mainCamera);
    }
    if(ImGui::MenuItem("Second Camera", "", &value2)) {
        value1 = false;
        //engine->setMainCamera();
    }
}

void MainWindow::displayComponentsAvailable() {
}

void MainWindow::menuEntity() {
    if(windowCurrentEntity) {
        ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);

        ImGui::Begin(std::string("Create entity " +
                    std::to_string(currentEntity->getID())).c_str(),
                &windowCurrentEntity);

        displayComponents(currentEntity);

        if(ImGui::Button("Add Component"))
            ImGui::OpenPopup("popup from button");

        if(ImGui::BeginPopup("popup from button"))
        {
            ImGui::MenuItem("Components", NULL, false, false);

            if(!currentEntity->has<fmc::CTransform>() && ImGui::MenuItem("Transform"))
            {
                currentEntity->add<fmc::CTransform>();
            }

            if(!currentEntity->has<fmc::CMesh>() && ImGui::MenuItem("Mesh"))
            {
                currentEntity->add<fmc::CMesh>();
            }

            if(!currentEntity->has<fmc::CMaterial>() && ImGui::MenuItem("Material"))
            {
                currentEntity->add<fmc::CMaterial>();
            }

            ImGui::EndPopup();
        }
        ImGui::End();
    }
}

void MainWindow::listEntity()
{
    if(windowListEntity)
    {
        static std::vector<const char*> namesEntities;
        ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("List entities", &windowListEntity);

        if(timerListEntityUpdate > 1)
        {
            namesEntities = getAllEntities();
            timerListEntityUpdate = 0;
        } else
        {
            timerListEntityUpdate += fm::Time::dt;
        }
        static int number = 0;
        ImGui::ListBox("List entities", &number,&namesEntities[0],
                (int)namesEntities.size(),
                -1);
        currentEntity = fm::SceneManager::get().getCurrentScene()->getAllGameObjects()[number];

        if(ImGui::Button("Add Entity"))
        {
            currentEntity = fm::GameObjectHelper::create();
            currentEntity->addComponent<fmc::CTransform>(new fmc::CTransform(
                                                             fm::math::Vector3f(0, 0, 0),
                                                             fm::math::Vector3f(100, 100, 100),
                                                             fm::math::vec3(0,0,0)));
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

void MainWindow::window_WorldLightEditDisplay() {
    bool value = true;
    ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("World light", &window_WorldLightEdit);
    Inspector::OnDraw(dlight->get<fmc::CDirectionalLight>(), &value);
    ImGui::End();
}

void MainWindow::draw() {
    bool show_test_window = true;
    menu();
    menuEntity();
    listEntity();
    if(fileSystem_save)
        fileSystem_save_window();
    ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
    ImGui::ShowTestWindow(&show_test_window);
    if(window_WorldLightEdit)
        window_WorldLightEditDisplay();

    if(ImGui::GetIO().MouseClicked[1]) {
        if(!firstRightClick) {
            FirstPosMouseRightClick = ImGui::GetIO().MousePos;
            firstRightClick = true;
        }
    }

    if(ImGui::GetIO().MouseDown[1]) {
        if(firstRightClick) {
            mainCameraPosition->position.x +=
                (FirstPosMouseRightClick.x - ImGui::GetIO().MousePos.x) *
                coeffMouseSpeed;
            mainCameraPosition->position.y +=
                (FirstPosMouseRightClick.y - ImGui::GetIO().MousePos.y) *
                coeffMouseSpeed;

            FirstPosMouseRightClick = ImGui::GetIO().MousePos;
        }
    } else {
        firstRightClick = false;
    }

    gameView.draw();

    ImGui::SetNextWindowPos(ImVec2(10, 30));
    static bool v = true;
    if(!ImGui::Begin(
                "Mouse pos",
                &v,
                ImVec2(200, 0),
                0.8f,
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings)) {
        ImGui::End();
        return;
    }

    ImGui::Text("Position: (%.1f,%.1f)",
            mainCameraPosition->position.x,
            mainCameraPosition->position.y);
    ImGui::End();

    //mainCamera->get<fmc::CCamera>()->target->getColorBuffer()[0].writeToPNG("test.png");
}
