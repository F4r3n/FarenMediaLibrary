#pragma once
#define GUI

#include <imgui.h>
#include <ECS.h>

#include <type_traits>
#include <Components/CText.h>
#include <Components/CMesh.h>
#include "Rendering/Texture.h"
#include "Inspector.hpp"
#include "Core/GameObject.h"
namespace fmc {
    class CTransform;
    class CMaterial;
    class Body2D;
    class CDirectionalLight;
}

namespace fm {
class Engine;
}

class MainWindow {


public:
    MainWindow(fm::Engine* engine);

    void displayComponents(fm::GameObject* currentEntity);
    void menu();
    void menuEntity();
    void listEntity();
    std::vector<const char*> getAllEntities();
    void draw();
    void displayComponentsAvailable();
    
    void window_WorldLightEditDisplay();

    void displayListCamera();
    void fileSystem_save_window();

private:

    fm::GameObject* currentEntity;
    fm::GameObject* mainCamera;
    Entity* dlight;
    bool windowCurrentEntity = false;
    bool windowListEntity = false;

    float timerListEntityUpdate = 1;

    bool choiceComponent = false;

    ImVec2 FirstPosMouseRightClick;
    float coeffMouseSpeed = -0.8f;
    bool firstRightClick = false;
    fmc::CTransform* mainCameraPosition;

    fm::Engine* engine;
    bool window_WorldLightEdit = false;
    
    
    std::string nameWindow;
    std::string nameCurrentScene = "";
    bool fileSystem_save = false;
    
    fm::Texture playImage;
    
    std::unordered_map<size_t, std::unordered_map<std::string, Inspector*>> _inspectorComponents;
};
