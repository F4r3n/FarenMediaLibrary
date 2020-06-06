#pragma once
#define GUI

#include "GWindow.h"
#include "Core/application.h"
#include "inspector/inspector.hpp"
#include "Core/Observer.h"
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
    MainWindow();
    ~MainWindow();
	void Draw();
	void Update(bool hasFocus);
	void Init();

protected:
	virtual void Notify(fm::Observable*, const fm::EventObserver& inEvent) override;

	void _OnPreStart();
	void _OnAfterStart();

	void _OnPreStop();
	void _OnAfterStop();

	void _OnPreLoad();
	void _OnAfterLoad();

private:
	void _DrawMenu();

	void _DisplayWindow_Load();
	void _DisplayWindow_Save();

	void _ConfigureStyle();
	void _InitEditorCamera();

	void _AddEmptyScene();
	void _ClearBeforeSceneChange();
	void _InitGameView();

	void _Copy();
	void _Paste();
	void _AddDock(gui::WINDOWS inWindow, ImGuiID inID);
	bool IsWindowAvailable(gui::WINDOWS inWindow);

private:
	gui::Context						_context;
	std::shared_ptr<fm::Scene>			_editorScene;
	MapOfWindows						_windows;
    std::optional<ecs::id>				_currentEntity;       
    fm::ProjectSettings					_projectSettings;
	std::shared_ptr <fm::GameObject>	_editorCamera;
	bool								_needUpdate;
	bool								_hasFocus;

	//==========Systems==============
};


