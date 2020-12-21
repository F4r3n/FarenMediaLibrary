#include "GWindow.h"
#include <imgui/imgui_internal.h>
using namespace gui;


GWindow::GWindow(const std::string &inName, bool isDockable, ImGuiWindowFlags inOption)
{
	_enabled = false;
	_name = inName;
	_dockable = isDockable;
	_option = inOption;
	_isFocused = false;
	_kind = WINDOWS::WIN_NO_KIND;
	_hasBeenDrawn = false;
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

bool GWindow::HasFocus() const
{
	return _isFocused;
}

const fm::math::vec2& GWindow::GetSize() const
{
	return _size;
}

const fm::math::vec2& GWindow::GetPosition() const
{
	return _position;
}

bool GWindow::IsDocked() const
{
	return _iswindowDocked;
}

bool GWindow::HasBeenDrawn() const
{
	return _hasBeenDrawn;
}


void GWindow::Draw()
{
	if (_enabled && !_needUpdate)
	{
		if (!_hasBeenDrawn)
		{
			ImGui::SetWindowCollapsed(false, ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowPos(_position, ImGuiCond_FirstUseEver);
			if (_size.x != 0 && _size.y != 0)
				ImGui::SetNextWindowContentSize(_size);
		}

		bool previousStatus = _enabled;


		BeforeWindowCreation();
		if (ImGui::Begin(_name.c_str(), &_enabled, _option))
		{
			_isVisible = true;
			AfterWindowCreation();

			_isFocused = ImGui::IsWindowFocused();
			_iswindowDocked = ImGui::IsWindowDocked();

			ImGuiContext* context = ImGui::GetCurrentContext();
			ImVec2 size;
			ImVec2 pos;
			if (_iswindowDocked)
			{
				size = context->CurrentWindow->DockNode->Size;
				pos = context->CurrentWindow->DockNode->Pos;
			}
			else
			{
				size = ImGui::GetWindowSize();
				pos = ImGui::GetWindowPos();
			}
			_size.x = size.x;
			_size.y = size.y;

			_position.x = pos.x;
			_position.y = pos.y;

			if(_enableCustomDraw)
				CustomDraw();

			if (!_widgets.empty())
			{
				for (auto&& widget : _widgets)
				{
					widget->Draw();
				}
			}

			if (previousStatus != _enabled)
			{
				WillClose();
			}
			_hasBeenDrawn = true;
		}
		else
		{
			AfterWindowCreation();
			_isVisible = false;
		}


		ImGui::End();
	}
		
}

void GWindow::SetStatus(bool inValue)
{
	_enabled = inValue;
}

void GWindow::Update(float dt, Context &inContext)
{
	if (_enabled)
	{
		_DequeueEvent();

		if (_isVisible)
		{
			if (_isFocused)
			{
				inContext.currentWindowFocused = _kind;
			}
			_Update(dt, inContext);

			if (!_widgets.empty())
			{
				for (auto&& widget : _widgets)
				{
					widget->Update(dt, inContext);
				}
			}
		}
		_needUpdate = false;
	}
	
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
