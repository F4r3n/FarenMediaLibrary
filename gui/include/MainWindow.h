#pragma once
#define GUI

#include <imgui.h>
#include <ECS.h>

#include <type_traits>
#include <Components/CText.h>
#include <Components/CMesh.h>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "Rendering/Texture.h"
#include "Inspector.hpp"
using namespace rapidjson;

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

    struct EntityDisplay {
        std::string name;
        size_t id;
    };

public:
    MainWindow(fm::Engine* engine);

    

    void displayComponents(Entity* currentEntity);
    void menu();
    void menuEntity();
    void listEntity();
    void getAllEntities();
    void draw();
    void displayComponentsAvailable();
    
    void window_WorldLightEditDisplay();

    void displayListCamera();
    void fileSystem_save_window();

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
    
    std::string nameWindow;
    std::string nameCurrentScene = "";
    bool fileSystem_save = false;
    
    rapidjson::StringBuffer lastState;
    fm::Texture playImage;
    
    Entity* cameraEditor;
    std::unordered_map<size_t, std::unordered_map<std::string, Inspector*>> _inspectorComponents;
};
