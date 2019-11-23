#pragma once
#define GUI

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


class MainWindow : public fm::ApplicationObserver
{
enum WINDOWS
{
    WIN_LIGHT_EDIT,
    WIN_LOGGER,
    WIN_PROJECT_SETTINGS,
	WIN_LIST_ENTITIES,
	WIN_EDITOR_VIEW,
	WIN_SCENE_VIEW,
	WIN_TOOLBAR,
    WIN_LAST

};
typedef std::unordered_map<WINDOWS, std::unique_ptr<gui::GWindow>> MapOfWindows;
typedef std::unordered_map<size_t, std::unordered_map<size_t, std::unique_ptr<Inspector>>> MapOfInspectors;

public:
    MainWindow();
    ~MainWindow();
	void Draw();
	void Update();
	void Init();

protected:
	virtual void OnPreStart();
	virtual void OnAfterStart();

	virtual void OnPreStop();
	virtual void OnAfterStop();
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
	void _InitEditorCamera();

	void _AddEmptyScene();

private:

	gui::Context				_context;

	std::shared_ptr<fm::Scene>	_editorScene;
	MapOfWindows				_windows;
	bool						_windowStates[WIN_LAST];


    fm::GameObject*				_currentEntity;
    fm::GameObject*				_dlight;
       
	MapOfInspectors				_inspectorComponents;

    fm::ProjectSettings			_projectSettings;

    fm::GameObject*				_editorCamera;
	std::vector<fm::Config>		_lastConfigsUsed;

	//==========Systems==============
};


