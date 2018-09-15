#pragma once
#define GUI

#include <imgui/imgui.h>


#include <type_traits>
#include "inspector/Inspector.hpp"
#include "GameView.h"
#include "debuglogger.h"


namespace fmc {
    class CTransform;
    class CMaterial;
    class Body2D;
    class CDirectionalLight;
}

namespace fm {
class GameObject;
class Engine;
}

class MainWindow {


public:
    MainWindow(fm::Engine* engine);
    MainWindow() = default;
    ~MainWindow() = default;

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
    void _configureStyle();
    GameView _gameView;
    fm::GameObject* _currentEntity = nullptr;
    fm::GameObject* _mainCamera;
    Entity* _dlight;
    bool _windowCurrentEntity = true;
    bool _windowListEntity = false;

    float _timerListEntityUpdate = 1;

    bool _choiceComponent = false;

    ImVec2 _firstPosMouseRightClick;
    float _coeffMouseSpeed = -0.8f;
    bool _firstRightClick = false;
    fmc::CTransform* _mainCameraPosition;

    fm::Engine* _engine;
    bool _windowWorldLightEdit = false;
    
    
    std::string _nameWindow;
    std::string _nameCurrentScene = "";
    bool _fileSystemSave = false;
    
    std::unordered_map<size_t, std::unordered_map<size_t, Inspector*>> _inspectorComponents;
    DebugLogger _debugLogger;
    bool _activateDebugLogger = false;

    std::string _projectName;
    std::string _directoryPath;
};


