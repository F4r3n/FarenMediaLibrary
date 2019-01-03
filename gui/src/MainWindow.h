#pragma once
#define GUI

#include <imgui/imgui.h>
#include "Core/application.h"

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
enum WINDOWS
{
    WIN_LIGHT_EDIT,
    WIN_LOGGER,
    WIN_CREATE_PROJECT,
    WIN_FILE_BROWSER_LOCATION,
    WIN_PROJECT_SETTINGS,
    WIN_PROJECT_LOAD,
    WIN_LAST
};

struct ProjectSettings
{
    std::string name = "Project";
    fm::FilePath path;
    std::string resourcesFolder = "Resources";
};

public:
    MainWindow();
    ~MainWindow() = default;

    void displayComponents(fm::GameObject* currentEntity);
    void DrawMenu();
    void menuEntity();
    void listEntity();
    std::vector<const char*> getAllEntities();
    void draw();
    void displayComponentsAvailable();
    

    void displayListCamera();

    void DisplayWindow_Save();
    void DisplayWindow_ProjectSettings();
    void DisplayWindow_WorldLighEdit();
    void DisplayWindow_Load();
private:

	void _ClearInspectorComponents();
    void _configureStyle();
    GameView _gameView;
    fm::GameObject* _currentEntity = nullptr;


    fm::GameObject* _dlight;
    bool _windowCurrentEntity = true;
    bool _windowListEntity = false;

    float _timerListEntityUpdate = 1;

    bool _choiceComponent = false;

    ImVec2 _firstPosMouseRightClick;
    float _coeffMouseSpeed = -0.8f;
    bool _firstRightClick = false;
    
    
    std::string _nameWindow;
    std::string _nameCurrentScene = "";
    
    std::unordered_map<size_t, std::unordered_map<size_t, std::unique_ptr<Inspector>>> _inspectorComponents;
    DebugLogger _debugLogger;

    ProjectSettings _projectSettings;
    std::string _directoryPath;

    bool _windowStates[WIN_LAST];

    fm::GameObject* _mainCamera;
	std::vector<fm::Config> _lastConfigsUsed;


};


