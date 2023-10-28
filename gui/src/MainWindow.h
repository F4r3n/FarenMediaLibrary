#pragma once
#define GUI

#include "Window/GWindow.h"
#include "Core/application.h"
#include "inspector/inspector.hpp"
#include "Core/Observer.h"
#include <any>
#include <unordered_map>
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


class MainWindow : public fm::Observer
{

typedef std::unordered_map<gui::WINDOWS, std::unique_ptr<gui::GWindow>> MapOfWindows;

public:
	enum DOCK_ID
	{
		dock_main_id,
		dock_up_id,
		dock_right_id,
		dock_left_id,
		dock_down_id,
		dock_left_right_id,
		dock_down_right_id,
		LAST
	};


    MainWindow();
    ~MainWindow();
	void OnDraw();
	void OnUpdate(bool hasFocus, bool force = false);
	void Init();
	void LoadProject(const fm::FilePath& inFilePath);


protected:
	virtual void Notify(fm::Observable*, const fm::EventObserver& inEvent) override;

	void _OnPreStart(const std::any& inAny);
	void _OnAfterStart(const std::any& inAny);

	void _OnPreStop(const std::any& inAny);
	void _OnAfterStop(const std::any& inAny);

	void _OnPreLoad(const std::any& inAny);
	void _OnAfterLoad(const std::any& inAny);

private:
	void _DrawMenu();

	//void _DisplayWindow_Load_Project();
	//void _DisplayWindow_Save_Project();
	//void _DisplayWindow_Create_Project();

	void _DisplayWindow_Create_Scene();

	void _ConfigureStyle();
	void _InitEditorCamera();

	void _AddEmptyScene();
	void _ClearBeforeSceneChange();
	void _InitGameView();

	void _Copy();
	void _Paste();
	void _AddDock(gui::WINDOWS inWindow, ImGuiID inID);
	bool IsWindowAvailable(gui::WINDOWS inWindow);
	void _AfterLoad();

	void _RefreshResources(const fm::FilePath& inPath);
	void _SetWindowPosition(gui::GWindow* window, DOCK_ID inID);

private:
	std::unordered_map<DOCK_ID, ImGuiID>_docks;
	gui::Context						_context;
	std::shared_ptr<fm::Scene>			_editorScene;
	MapOfWindows						_windows;
    std::optional<Entity::Id>			_currentEntity;       
    fm::ProjectSettings					_projectSettings;
	std::shared_ptr <fm::GameObject>	_editorCamera;
	bool								_needUpdate;
	bool								_hasFocus;
	std::queue<std::function<void()>>	_events;
	//==========Systems==============
};


