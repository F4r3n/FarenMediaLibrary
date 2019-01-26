#pragma once
#define GUI

#include <imgui/imgui.h>
#include "GameView.h"
#include "debuglogger.h"
#include "GWindow.h"
#include "Core/application.h"
#include "inspector/inspector.hpp"
namespace fmc {
    class CTransform;
    class CMaterial;
    class Body2D;
    class CDirectionalLight;
}

namespace fm {
class GameObject;
class Engine;
class Scene;
}

typedef std::unordered_map<size_t, std::unique_ptr<gui::GWindow>> MapOfWindows;

class MainWindow 
{
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



public:
    MainWindow();
    ~MainWindow() = default;

    void displayComponents(fm::GameObject* currentEntity);
    void DrawMenu();
    void menuEntity();
    void listEntity();
    std::vector<const char*> getAllEntities();
    void Draw();
	void Update();
    void displayComponentsAvailable();
    

    void displayListCamera();

    void DisplayWindow_Save();
    void DisplayWindow_ProjectSettings();
    void DisplayWindow_WorldLighEdit();
    void DisplayWindow_Load();
private:
	std::shared_ptr<fm::Scene> _editorScene;
	MapOfWindows _windows;


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

    fm::ProjectSettings _projectSettings;
    std::string _directoryPath;

    bool _windowStates[WIN_LAST];

    fm::GameObject* _mainCamera;
	std::vector<fm::Config> _lastConfigsUsed;

	ImGuiID dockspace_id;
};


