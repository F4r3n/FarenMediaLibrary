#pragma once
#define GUI

#include <imgui/imgui.h>
#include "GameView.h"
#include "debuglogger.h"
#include "GWindow.h"
#include "Core/application.h"
#include "inspector/inspector.hpp"
namespace fmc 
{
    class CTransform;
    class CMaterial;
    class Body2D;
    class CDirectionalLight;
}

namespace fm
{
	class GameObject;
	class Engine;
	class Scene;
}


namespace fms
{
	class PickingSystem;
}

namespace gui
{
	class GListEntities;
}

typedef std::unordered_map<size_t, std::unique_ptr<gui::GWindow>> MapOfWindows;

class MainWindow 
{
enum WINDOWS
{
    WIN_LIGHT_EDIT,
    WIN_LOGGER,
    WIN_PROJECT_SETTINGS,
	WIN_LIST_ENTITIES,
	WIN_GAMEVIEW,
    WIN_LAST
};

public:
    MainWindow();
    ~MainWindow();
	void Draw();
	void Update();

private:
	void _DrawComponents(fm::GameObject* currentEntity);
	void _DrawMenu();
	void _DrawMenuEntity();
	void _DrawComponentsAvailable();
	void _DrawListCamera();

	void _DisplayWindow_ProjectSettings();
	void _DisplayWindow_WorldLighEdit();
	void _DisplayWindow_Load();
	void _DisplayWindow_Save();

	void _ClearInspectorComponents();
	void _ConfigureStyle();

	void _CallBackFromPickingSystem(fm::GameObject* inGameObject);
private:
	gui::Context _context;

	std::shared_ptr<fm::Scene> _editorScene;
	MapOfWindows _windows;
	bool _windowStates[WIN_LAST];


    fm::GameObject* _currentEntity = nullptr;

    fm::GameObject* _dlight;
       
    std::unordered_map<size_t, std::unordered_map<size_t, std::unique_ptr<Inspector>>> _inspectorComponents;
    DebugLogger _debugLogger;

    fm::ProjectSettings _projectSettings;

    fm::GameObject* _mainCamera;
	std::vector<fm::Config> _lastConfigsUsed;

	//==========Systems==============
	fms::PickingSystem *_pickingSystem;

};


