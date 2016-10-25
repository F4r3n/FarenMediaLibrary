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
public:
    MainWindow();
    template <typename T> void displayComponent(Entity* currentEntity) {
        if(currentEntity->has<T>())
            currentEntity->get<T>()->display();
    }

    void displayComponents(Entity* currentEntity);
    void menu();
    void menuEntity();
    void draw();

private:
    Entity* currentEntity;
    bool windowCurrentEntity = false;
};