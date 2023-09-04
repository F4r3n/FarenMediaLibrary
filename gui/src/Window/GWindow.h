#pragma once 
#include <vector>
#include <memory>
#include <string>
#include "Core/Math/Vector2.h"
#include <imgui/imgui.h>
#include <utility>
#include <functional>
#include <queue>
#include <optional>
#include "Entity.h"

namespace fm
{
	class GameObject;
	class Scene;
}

namespace gui
{
	enum class TRANSFORM_CONTEXT
	{
		TRANSLATE,
		SCALE,
		ROTATE,
		NONE,
		LAST
	};

	enum class WINDOWS
	{
		WIN_LIGHT_EDIT,
		WIN_LOGGER,
		WIN_PROJECT_SETTINGS,
		WIN_LIST_ENTITIES,
		WIN_EDITOR_VIEW,
		WIN_SCENE_VIEW,
		WIN_TOOLBAR,
		WIN_INSPECTOR,
		WIN_FILE_NAVIGATOR,
		WIN_MATERIAL_EDITOR,
		WIN_NO_KIND,
		WIN_LAUNCHER,
		WIN_LAST

	};

	struct Context
	{
		std::optional<Entity::Id>		currentGameObjectSelected;
		std::string						currentSceneName;
		TRANSFORM_CONTEXT				currentTransformContext;
		WINDOWS							currentWindowFocused;
	};

	class IWidget
	{
	public:
		virtual void Draw() = 0;
		virtual void Update(float, Context &inContext) = 0;
		virtual ~IWidget() {};
	};



class GWindow : public IWidget
{
public:
	GWindow(const std::string &inName, bool isDockable, ImGuiWindowFlags inOption = ImGuiWindowFlags_AlwaysAutoResize);
	void SetPosition(const fm::math::vec2 & inVec2);
	void SetSize(const fm::math::vec2 & inVec2);

	void Draw();
	void AddWidget(std::unique_ptr<IWidget> && widget);
	void SetStatus(bool inValue);
	virtual ~GWindow() {}
	void Update(float, Context &inContext);
	void Start() { _enabled = true; }
	void Stop() { _enabled = false; }
	bool IsEnabled() const { return _enabled; }
	const std::string& GetTitle() const { return _name; }
	void AddEvent(std::function<void(GWindow*, std::optional<Context>)> && inEvent) { _events.push(std::move(inEvent)); }
	void NeedUpdate() { _needUpdate = true; }
	void EnableCustomDraw(bool enable) { _enableCustomDraw = enable; }
	bool IsModal() const { return _modal; }
	void SetModal(bool inModal) { _modal = inModal; }
	void SetCallBackClosure(std::function<void(GWindow*, std::optional<Context>)>&& inF) { _callBackClosure = inF; }
protected:
	virtual void			_Update(float, Context &inContext) {};
	virtual void			CustomDraw();
	virtual void			BeforeWindowCreation() {}
	virtual void			AfterWindowCreation() {}
	virtual void			WillClose();
	virtual void			OnInit() {}

			bool			HasFocus() const;
	const	fm::math::vec2&	GetSize() const;
	const	fm::math::vec2&	GetPosition() const;
			bool			IsDocked() const;
			bool			HasBeenDrawn() const;

protected:
	bool										_enabled;
	std::queue<std::function<void(GWindow*, std::optional<Context>)>>	_events;
	WINDOWS										_kind;

private:
	void _DequeueEvent(Context& inContext);

	std::vector<std::unique_ptr<IWidget>> _widgets;
	std::string							  _name;

	size_t								  _option;
	bool								  _dockable;
	bool								  _isFocused;
	fm::math::vec2						  _position;
	fm::math::vec2						  _size;
	bool								  _iswindowDocked;
	bool								  _hasBeenDrawn;
	bool								  _isVisible;
	bool								  _needUpdate = false;
	bool								  _enableCustomDraw = true;
	bool								  _modal = false;
	std::function<void(GWindow*, std::optional<Context>)>	_callBackClosure;
};
}

