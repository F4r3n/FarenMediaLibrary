#include "GWindow.h"
using namespace gui;




GWindow::GWindow(const std::string &inName, bool isDockable, ImGuiWindowFlags inOption)
{
	_enabled = false;
	_name = inName;
	_dockable = isDockable;
	_option = inOption;
}

void GWindow::AddWidget(std::unique_ptr<IWidget> && widget)
{
	_widgets.emplace_back(std::move(widget));
}

void GWindow::SetPosition(const fm::math::vec2 & inVec2)
{
	_position = inVec2;
}

void GWindow::SetSize(const fm::math::vec2 & inVec2)
{
	_size = inVec2;
}

void GWindow::CustomDraw()
{

}

void GWindow::Draw()
{
	if (_enabled)
	{
		bool previousStatus = _enabled;
		if (!_dockable)
		{
			ImGui::SetNextWindowPos(ImVec2(_position.x, _position.y), ImGuiCond_FirstUseEver);
			if (_size.x != 0 && _size.y != 0)
				ImGui::SetNextWindowContentSize(ImVec2(_size.x, _size.y));
		}

		BeforeWindowCreation();
		ImGui::Begin(_name.c_str(), &_enabled, _option);
		AfterWindowCreation();

		_id = ImGui::GetWindowDockID();
		CustomDraw();

		for (auto &widget : _widgets)
		{
			widget->Draw();
		}
		ImGui::End();

		if (previousStatus != _enabled)
		{
			WillClose();
		}
	}
}

void GWindow::SetStatus(bool inValue)
{
	_enabled = inValue;
}

void GWindow::Update(float dt, Context &inContext)
{
	_DequeueEvent();
	_Update(dt, inContext);
}


void GWindow::_DequeueEvent()
{
	if (!_events.empty())
	{
		auto && e = _events.front();
		if (e != nullptr)
		{
			e(this);
		}
		_events.pop();
	}
}



void GWindow::WillClose()
{

}
