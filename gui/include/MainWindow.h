#pragma once
#define GUI

#include <imgui.h>
#include <ECS.h>
#include <CTransform.h>
#include <CMaterial.h>
#include <CMesh.h>
#include <CText.h>
#include <CPointLight.h>
#include "imgui_impl_glfw_gl3.h"
 

class MainWindow {
    
    enum COMPONENTS_GUI {
        TRANSFORM, MESH, MATERIAL, CAMERA, LAST_COMPONENT
    };
    
    struct EntityDisplay {
        std::string name;
        size_t id;
    };

public:
    MainWindow();
    template <typename T> void displayComponent(Entity* currentEntity) {
        bool value = true;
        if(currentEntity->has<T>())
            currentEntity->get<T>()->display(&value);
       if(!value) {
           currentEntity->remove<T>();
       }
    }

    void displayComponents(Entity* currentEntity);
    void menu();
    void menuEntity();
    void listEntity();
    void getAllEntities();
    void draw();
    void displayComponentsAvailable();

private:
    Entity* currentEntity;
    bool windowCurrentEntity = false;
    bool windowListEntity = false;

    int currentEntitySelected = 0;
    int previousEntitySelected = 0;
    float timerListEntityUpdate = 1;
    std::vector<const char*> entitiesName;
    std::vector<EntityDisplay> entityDisplay;
    
   
    bool choiceComponent = false;
    std::array<std::string, COMPONENTS_GUI::LAST_COMPONENT> components;
    
};