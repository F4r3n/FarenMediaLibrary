#pragma once
#define GUI

#include <imgui.h>
#include <ECS.h>
#include <Components/CTransform.h>
#include <Components/CMaterial.h>
#include <Components/CMesh.h>
#include <Components/CText.h>
#include <Components/CPointLight.h>
#include "imgui_impl_glfw_gl3.h"
#include <type_traits>
#define COMPONENT(x) x,
enum COMPONENTS_GUI {
#include "Component_GUI.h"
    LAST_COMPONENT
};
#undef COMPONENT

#define LIST_COMPONENT fmc::CTransform, fmc::CMesh, fmc::CMaterial, fmc::Body2D

namespace fm {
class Engine;
}

class MainWindow {

    struct EntityDisplay {
        std::string name;
        size_t id;
    };

public:
    MainWindow(fm::Engine* engine);

    template <typename... Ts>
    typename std::enable_if<sizeof...(Ts) == 0>::type displayComponent(Entity* currentEntity) {
    }

    template <typename T, typename... Args> void displayComponent(Entity* currentEntity) {
        bool value = true;
        if(currentEntity->has<T>())
            currentEntity->get<T>()->display(&value);
        if(!value) {
            currentEntity->remove<T>();
        }
        displayComponent<Args...>(currentEntity);
    }
 template <typename... Ts>
    typename std::enable_if<sizeof...(Ts) == 0>::type displayComponentMenu(Entity* currentEntity) {
    }

    template <typename T, typename... Args> void displayComponentMenu(Entity* currentEntity) {
        if(ImGui::MenuItem(T::name.c_str())) {
        if(!currentEntity->has<T>())
            currentEntity->addComponent<T>();
        }
        displayComponentMenu<Args...>(currentEntity);
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

    ImVec2 FirstPosMouseRightClick;
    float coeffMouseSpeed = -0.8f;
    bool firstRightClick = false;
    fmc::CTransform* mainCameraPosition;

    fm::Engine* engine;
};