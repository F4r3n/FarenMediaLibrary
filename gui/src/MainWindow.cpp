#include "MainWindow.h"
#include "CCamera.h"
MainWindow::MainWindow() {
}

void MainWindow::displayComponents(Entity* currentEntity) {

    displayComponent<fmc::CTransform>(currentEntity);
    displayComponent<fmc::CCamera>(currentEntity);
    displayComponent<fmc::CMesh>(currentEntity);
}

void MainWindow::menu() {
    if(ImGui::BeginMainMenuBar()) {
        if(ImGui::BeginMenu("File")) {

            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Edit")) {
            if(ImGui::MenuItem("Undo", "CTRL+Z")) {
            }
            if(ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {
            } // Disabled item
            ImGui::Separator();
            if(ImGui::MenuItem("Cut", "CTRL+X")) {
            }
            if(ImGui::MenuItem("Copy", "CTRL+C")) {
            }
            if(ImGui::MenuItem("Paste", "CTRL+V")) {
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Entity")) {
            if(ImGui::MenuItem("Create")) {
                windowCurrentEntity = true;
                currentEntity = EntityManager::get().createEntity();
                currentEntity->addComponent<fmc::CTransform>(
                    new fmc::CTransform(fm::Vector2f(0, 0), fm::Vector2f(100, 100), 0));
                currentEntity->addComponent<fmc::CMesh>(new fmc::CMesh(fmc::SHAPE::RECTANGLE));
                currentEntity->addComponent<fmc::CMaterial>()->color = fm::Color(1, 0, 0, 1);
            }
            if(ImGui::MenuItem("List entity")) {
                windowListEntity = true;
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void MainWindow::menuEntity() {
    if(windowCurrentEntity) {
        ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
        ImGui::Begin(std::string("Create entity " + std::to_string(currentEntity->ID)).c_str(), &windowCurrentEntity);

        displayComponents(currentEntity);

        if(ImGui::Button("Add Component")) {
        }
        ImGui::End();
    }
}

void MainWindow::listEntity() {
    if(windowListEntity) {
        ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
        std::cout << "Inside " << std::endl;
        ImGui::Begin("List entities", &windowListEntity);
        std::vector<const char*> entitiesName;
        std::vector<EntityDisplay> entityDisplay;
        
        
        
        std::string name = std::string("Entity");
        for(auto e : EntityManager::get().iterate<fmc::CTransform>()) {
            EntityDisplay ed;
            ed.id = e->ID;
            ed.name = name + std::to_string(e->ID);
            entityDisplay.push_back(ed);
            
        }
        
        for(EntityDisplay &s : entityDisplay) {
            entitiesName.push_back(s.name.c_str());
        }
        ImGui::ListBox("List entities", 
        &previousEntitySelected, &entitiesName[0], (int)entitiesName.size(), -1);
        if(currentEntitySelected != previousEntitySelected) {
            currentEntitySelected = previousEntitySelected;
            currentEntity = EntityManager::get().getEntity(entityDisplay[previousEntitySelected].id);
            windowCurrentEntity = true;
        }
        
        if(ImGui::Button("Add Entity")) {
        }
        ImGui::End();
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