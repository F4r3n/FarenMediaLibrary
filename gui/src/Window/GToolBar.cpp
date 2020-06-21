#include "GToolBar.hpp"
#include "Input/InputManager.h"
#include "Core/application.h"
using namespace gui;

GToolBar::GToolBar() : GWindow("GToolBar", true)
{
	_enabled = true;
	_state = TRANSFORM_CONTEXT::TRANSLATE;
	_kind = WINDOWS::WIN_TOOLBAR;
}

GToolBar::~GToolBar()
{

}

void GToolBar::_Update(float, Context &inContext)
{
	_UpdateInputTransformContext(inContext);

	inContext.currentTransformContext = _state;
}

void GToolBar::_UpdateInputTransformContext(Context &inContext)
{
	ImGuiIO io = ImGui::GetIO();

	if (inContext.currentWindowFocused == WINDOWS::WIN_EDITOR_VIEW)
	{
		if (io.KeyAlt && ImGui::IsKeyPressed(SDL_Scancode::SDL_SCANCODE_T, false))
		{
			_state = gui::TRANSFORM_CONTEXT::TRANSLATE;
		}
		if (io.KeyAlt && ImGui::IsKeyPressed(SDL_Scancode::SDL_SCANCODE_R, false))
		{
			_state = gui::TRANSFORM_CONTEXT::ROTATE;
		}
		if (io.KeyAlt && ImGui::IsKeyPressed(SDL_Scancode::SDL_SCANCODE_S, false))
		{
			_state = gui::TRANSFORM_CONTEXT::SCALE;
		}
	}

}

void GToolBar::_DrawStartStop()
{
	ImVec2 windowsize = ImGui::GetWindowSize();
	ImGui::SameLine(windowsize.x/2.0f - 3*50);
	const ImVec2 buttonSize = ImVec2(30, 30);
	{
		const char* labelT = "Start\0";
		bool status = fm::Application::Get().IsRunning();
		if (ImGui::PushButton(labelT, buttonSize, &status))
		{
			if (status)
			{
				AddEvent([](GWindow*) {
					fm::Application::Get().Start(true);
				});
			}
		}
	}

	ImGui::SameLine();
	{
		const char* labelT = "Stop\0";
		bool status = !fm::Application::Get().IsRunning();
		if (ImGui::PushButton(labelT, buttonSize, &status))
		{
			if (status)
			{
				AddEvent([](GWindow*) {
					fm::Application::Get().Stop();
				});
				
			}
		}
	}
}



void GToolBar::_DrawTransformContext()
{
	const ImVec2 buttonSize = ImVec2(30, 30);
	const char* labelT = "T\0";
	bool status = _state == gui::TRANSFORM_CONTEXT::TRANSLATE;
	if (ImGui::PushButton(labelT, buttonSize, &status))
	{
		if (status)
		{
			_state = gui::TRANSFORM_CONTEXT::TRANSLATE;
		}
	}

	ImGui::SameLine();
	const char* labelS = "S\0";
	status = _state == gui::TRANSFORM_CONTEXT::SCALE;
	if (ImGui::PushButton(labelS, buttonSize, &status))
	{
		if (status)
		{
			_state = gui::TRANSFORM_CONTEXT::SCALE;
		}
	}

	ImGui::SameLine();
	const char* labelR = "R\0";
	status = _state == gui::TRANSFORM_CONTEXT::ROTATE;
	if (ImGui::PushButton(labelR, buttonSize, &status))
	{
		if (status)
		{
			_state = gui::TRANSFORM_CONTEXT::ROTATE;
		}
	}
}


void GToolBar::CustomDraw()
{
	_DrawTransformContext();
	_DrawStartStop();
}