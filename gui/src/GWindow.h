#pragma once 
#include <vector>
#include <memory>
#include <string>
#include "Core/Math/Vector2.h"
#include <imgui/imgui.h>
#include <utility>
#include <functional>
#include <Config.h>

namespace fm
{
	class GameObject;
}

namespace gui
{

	struct Context
	{
		fm::GameObject* currentGameObjectSelected;
	};

	class IWidget
	{
	public:
		virtual void Draw() = 0;
	};

	

	template <typename... T>
	class GButton : public IWidget
	{
	public:
		using bind_type = decltype(std::bind(std::declval<std::function<void(T...)>>(), std::declval<T>()...));

		template <typename... ConstrT>
		GButton(const std::string &inName, std::function<void(T...)> func, ConstrT &&... args)
			:_bind(std::forward<std::function<void(T...)>>(func), std::forward<ConstrT>(args)...)
		{
			_name = inName;
		}


		void Draw()
		{
			if (ImGui::Button(_name.c_str()))
			{
				_bind();
			}
		}
	private:
		std::string _name;
		bind_type _bind;

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
	virtual void Update(float, Context &inContext) {}

protected:

	virtual void CustomDraw();
	virtual void WillClose();
	fm::math::vec2 _position;
	fm::math::vec2 _size;
	bool _enabled;
private:
	std::vector<std::unique_ptr<IWidget>> _widgets;
	std::string _name;

	size_t _option;
	bool	_dockable;
};
}

