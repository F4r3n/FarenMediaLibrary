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
#include "rapidjson/document.h"
#include <Components/CPointLight.h>
#include "Rendering/RenderingSystem.h"
#include "Inspector.hpp"
#include "TransformInspector.h"
#include "MaterialInspector.h"
#include "MeshInspector.h"
MainWindow::MainWindow(fm::Engine* engine) {
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

    dlight = fm::Engine::createEntity();
    dlight->addComponent<fmc::CDirectionalLight>(
        new fmc::CDirectionalLight(fm::Color(0.3, 0.3, 0.3, 1)));
    dlight->addComponent<fmc::CTransform>(new fmc::CTransform(
        fm::math::Vector2f(100, 50), fm::math::Vector2f(20, 20), 0, 1));
    dlight->addComponent<fmc::CMaterial>();
    dlight->addComponent<fmc::CIdentity>()->display = false;

    playImage = fm::Texture("assets/images/play_button.png");

    cameraEditor = fm::Engine::createEntity();
    fmc::CCamera* cam =
        cameraEditor->addComponent<fmc::CCamera>(new fmc::CCamera(
            fm::Window::width, fm::Window::height, fmc::RENDER_MODE::FORWARD));
    mainCameraPosition = cameraEditor->addComponent<fmc::CTransform>();
    fmc::CIdentity* identity = cameraEditor->addComponent<fmc::CIdentity>();
    identity->name = "CameraEditor";
    identity->display = false;
    fmc::Body2D* b = cameraEditor->addComponent<fmc::Body2D>(
        new fmc::Body2D(100, 100, true));

    // cam->setNewViewPort(0,10,fm::Window::width, fm::Window::height);
    engine->getSystem<fms::RenderingSystem>()->setCamera(cameraEditor);
}

void MainWindow::displayComponents(Entity* currentEntity) {
    // return;
    std::vector<BaseComponent*> compos = currentEntity->getAllComponents();

    if(_inspectorComponents.find(currentEntity->ID) ==
       _inspectorComponents.end()) {
        std::unordered_map<std::string, Inspector*> inspectors;

        _inspectorComponents.insert(
            std::pair<size_t, std::unordered_map<std::string, Inspector*>>(
                currentEntity->ID, inspectors));
    }

    for(auto c : compos) {
        if(_inspectorComponents[currentEntity->ID][c->getName()] == nullptr) {
            if(c->getName().compare("Transform") == 0) {
                _inspectorComponents[currentEntity->ID][c->getName()] =
                    new gui::TransformInspector(c);
            } else if(c->getName().compare("Material") == 0) {
                _inspectorComponents[currentEntity->ID][c->getName()] =
                    new gui::MaterialInspector(c);
            }
            
            else if(c->getName().compare("Mesh") == 0) {
                _inspectorComponents[currentEntity->ID][c->getName()] =
                    new gui::MeshInspector(c);
            }
        } else {
            _inspectorComponents[currentEntity->ID][c->getName()]->draw();
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
                currentEntity = EntityManager::get().createEntity();
                currentEntity->addComponent<fmc::CTransform>(
                    new fmc::CTransform(fm::math::Vector2f(0, 0),
                                        fm::math::Vector2f(100, 100),
                                        0));
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
        engine->getSystem<fms::RenderingSystem>()->setCamera(cameraEditor);
    }
    if(ImGui::MenuItem("Second Camera", "", &value2)) {
        value1 = false;
        engine->setMainCamera();
    }
}

void MainWindow::displayComponentsAvailable() {
}

void MainWindow::menuEntity() {
    if(windowCurrentEntity) {
        ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);

        ImGui::Begin(std::string("Create entity " +
                                 std::to_string(currentEntity->ID)).c_str(),
                     &windowCurrentEntity);

        displayComponents(currentEntity);

        if(ImGui::Button("Add Component"))
            ImGui::OpenPopup("popup from button");

        if(ImGui::BeginPopup("popup from button")) {
            ImGui::MenuItem("Components", NULL, false, false);

            if(!currentEntity->has<fmc::CTransform>() &&
               ImGui::MenuItem("Transform")) {
                currentEntity->add<fmc::CTransform>();
            }

            if(!currentEntity->has<fmc::CMesh>() && ImGui::MenuItem("Mesh")) {
                currentEntity->add<fmc::CMesh>();
            }

            if(!currentEntity->has<fmc::CMaterial>() && ImGui::MenuItem("Material")) {
                currentEntity->add<fmc::CMaterial>();
            }

            ImGui::EndPopup();
        }
        ImGui::End();
    }
}

void MainWindow::listEntity() {
    if(windowListEntity) {
        ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("List entities", &windowListEntity);

        if(timerListEntityUpdate > 1) {
            getAllEntities();
            timerListEntityUpdate = 0;
        } else {
            timerListEntityUpdate += fm::Time::dt;
        }
        ImGui::ListBox("List entities",
                       &previousEntitySelected,
                       &entitiesName[0],
                       (int)entitiesName.size(),
                       -1);
        if(currentEntitySelected != previousEntitySelected) {
            currentEntitySelected = previousEntitySelected;
            currentEntity = EntityManager::get().getEntity(
                entityDisplay[previousEntitySelected].id);
            windowCurrentEntity = true;
        }

        if(ImGui::Button("Add Entity")) {
            currentEntity = EntityManager::get().createEntity();
            currentEntity->addComponent<fmc::CTransform>(new fmc::CTransform(
                fm::math::Vector2f(0, 0), fm::math::Vector2f(100, 100), 0));
            currentEntity->addComponent<fmc::CIdentity>();
        }
        ImGui::End();
    }
}

void MainWindow::getAllEntities() {
    entitiesName.clear();
    entityDisplay.clear();

    for(auto e : EntityManager::get().iterate<fmc::CIdentity>()) {
        fmc::CIdentity* identity = e->get<fmc::CIdentity>();
        if(!identity->display)
            continue;
        EntityDisplay ed;
        ed.id = e->ID;
        ed.name = identity->name + std::to_string(e->ID);
        entityDisplay.push_back(ed);
    }

    for(EntityDisplay& s : entityDisplay) {
        entitiesName.push_back(s.name.c_str());
    }
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
}
