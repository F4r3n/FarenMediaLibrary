#include "MainWindow.h"
#include "CCamera.h"
#include <Time.h>
#include "CMaterial.h"
MainWindow::MainWindow() {
    components[COMPONENTS_GUI::TRANSFORM] = "Transform";
    components[COMPONENTS_GUI::MESH] = "Mesh";
    components[COMPONENTS_GUI::MATERIAL] = "Material";
    components[COMPONENTS_GUI::CAMERA] = "Camera";
}

void MainWindow::displayComponents(Entity* currentEntity) {

    displayComponent<fmc::CTransform>(currentEntity);
    displayComponent<fmc::CCamera>(currentEntity);
    displayComponent<fmc::CMesh>(currentEntity);
    displayComponent<fmc::CMaterial>(currentEntity);
}

void MainWindow::menu() {
    if(ImGui::BeginMainMenuBar()) {
        if(ImGui::BeginMenu("File")) {

            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Edit")) {

            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Entity")) {
            if(ImGui::MenuItem("Create")) {
                windowCurrentEntity = true;
                currentEntity = EntityManager::get().createEntity();
                currentEntity->addComponent<fmc::CTransform>(
                    new fmc::CTransform(fm::Vector2f(0, 0), fm::Vector2f(100, 100), 0));
                // currentEntity->addComponent<fmc::CMesh>(new fmc::CMesh(fmc::SHAPE::RECTANGLE));
                // currentEntity->addComponent<fmc::CMaterial>()->color = fm::Color(1, 0, 0, 1);
            }
            if(ImGui::MenuItem("List entity")) {
                windowListEntity = true;
            }

            ImGui::EndMenu();
        }
    }
    ImGui::EndMainMenuBar();
}

void MainWindow::displayComponentsAvailable() {
}

void MainWindow::menuEntity() {
    if(windowCurrentEntity) {
        ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
        ImGui::Begin(std::string("Create entity " + std::to_string(currentEntity->ID)).c_str(), &windowCurrentEntity);

        displayComponents(currentEntity);

        if(ImGui::Button("Add Component"))
            ImGui::OpenPopup("popup from button");

        if(ImGui::BeginPopup("popup from button")) {
            ImGui::MenuItem("Components", NULL, false, false);

            for(int i = 1; i < components.size(); ++i) {

                if(ImGui::MenuItem(components[i].c_str())) {
                    if(i == MESH) {
                        if(!currentEntity->has<fmc::CMesh>())
                            currentEntity->addComponent<fmc::CMesh>();
                    }
                    if(i == MATERIAL) {
                        if(!currentEntity->has<fmc::CMaterial>())
                            currentEntity->addComponent<fmc::CMaterial>();
                    }
                }
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
        ImGui::ListBox("List entities", &previousEntitySelected, &entitiesName[0], (int)entitiesName.size(), -1);
        if(currentEntitySelected != previousEntitySelected) {
            currentEntitySelected = previousEntitySelected;
            currentEntity = EntityManager::get().getEntity(entityDisplay[previousEntitySelected].id);
            windowCurrentEntity = true;
        }

        if(ImGui::Button("Add Entity")) {
            currentEntity = EntityManager::get().createEntity();
            currentEntity->addComponent<fmc::CTransform>(
                new fmc::CTransform(fm::Vector2f(0, 0), fm::Vector2f(100, 100), 0));
        }
        ImGui::End();
    }
}

void MainWindow::getAllEntities() {
    entitiesName.clear();
    entityDisplay.clear();
    std::string name = std::string("Entity");
    for(auto e : EntityManager::get().iterate<fmc::CTransform>()) {
        EntityDisplay ed;
        ed.id = e->ID;
        ed.name = name + std::to_string(e->ID);
        entityDisplay.push_back(ed);
    }

    for(EntityDisplay& s : entityDisplay) {
        entitiesName.push_back(s.name.c_str());
    }
}

void MainWindow::draw() {
    bool show_test_window = true;
    menu();
    menuEntity();
    listEntity();
    ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
    ImGui::ShowTestWindow(&show_test_window);
}