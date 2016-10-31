#pragma once
#define GUI

#include <imgui.h>
#include <ECS.h>

#include "imgui_impl_glfw_gl3.h"
#include <type_traits>
#include <Components/CText.h>
#include <Components/CMesh.h>
#define LIST_COMPONENT fmc::CTransform, fmc::CMesh, fmc::CMaterial, fmc::Body2D, fmc::CText

namespace fmc {
    class CTransform;
    class CMaterial;
    class Body2D;
    class CDirectionalLight;
}

namespace fm {
class Engine;
}

template <typename T> inline void checkComponent(Entity* currentEntity) {
    if(!currentEntity->has<T>())
        currentEntity->addComponent<T>();
}

template <> inline void checkComponent<fmc::CText>(Entity* currentEntity) {
    if(!currentEntity->has<fmc::CText>() && !currentEntity->has<fmc::CMesh>())
        currentEntity->addComponent<fmc::CText>();
}

class MainWindow {

    struct EntityDisplay {
        std::string name;
        size_t id;
    };

public:
    MainWindow(fm::Engine* engine);

    template <typename... Ts>
    typename std::enable_if<sizeof...(Ts) == 0>::type displayComponentMenu(Entity* currentEntity) {
    }

    template <typename T, typename... Args> void displayComponentMenu(Entity* currentEntity) {
        if(ImGui::MenuItem(T::name.c_str())) {
            checkComponent<T>(currentEntity);
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
    
    void window_WorldLightEditDisplay();
    
    
     template <typename... Ts>
    typename std::enable_if<sizeof...(Ts) == 0>::type displayComponent(Entity* currentEntity) {
    }

    template <typename T, typename... Args> void displayComponent(Entity* currentEntity) {
        bool value = true;
        if(currentEntity->has<T>())
            display(currentEntity->get<T>(), &value);
        if(!value) {
            currentEntity->remove<T>();
        }
        displayComponent<Args...>(currentEntity);
    }
    
    
    void display(fmc::CTransform* t, bool *value);
    void display(fmc::Body2D* t, bool *value);
    void display(fmc::CMaterial* t, bool *value);
    void display(fmc::CMesh* t, bool *value);
    void display(fmc::CText* t, bool *value);
    void display(fmc::CDirectionalLight* t, bool *value);
    template <typename T>
    void display(T *t, bool *value) {
         if(ImGui::CollapsingHeader(T::name, value)) {
             
         }
        
    }

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
    bool window_WorldLightEdit = false;
    
    Entity* dlight;
};
