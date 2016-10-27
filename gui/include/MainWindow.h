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
    
    struct EntityDisplay {
        std::string name;
        size_t id;
    };
    
public:
    MainWindow();
    template <typename T> void displayComponent(Entity* currentEntity) {
        if(currentEntity->has<T>())
            currentEntity->get<T>()->display();
    }

    void displayComponents(Entity* currentEntity);
    void menu();
    void menuEntity();
    void listEntity();
    void draw();

private:
    Entity* currentEntity;
    bool windowCurrentEntity = false;
    bool windowListEntity = false;
    
    int currentEntitySelected = 0;
    int previousEntitySelected = 0;
};