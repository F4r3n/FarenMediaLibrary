#include "ToolBar.hpp"
#include "Input/InputManager.h"
#include "Core/application.h"
using namespace gui;

ToolBar::ToolBar() : GWindow("ToolBar", true)
{
	_enabled = true;
	_state = gui::TRANSFORM_CONTEXT::TRANSLATE;
}

ToolBar::~ToolBar()
{

}

void ToolBar::_Update(float, Context &inContext)
{
	_UpdateInputTransformContext();

	inContext.currentTransformContext = _state;
}

void ToolBar::_UpdateInputTransformContext()
{
	if (fm::InputManager::Get().IsKeyPressed(fm::FM_KEY_T))
		_state = gui::TRANSFORM_CONTEXT::TRANSLATE;
	else if (fm::InputManager::Get().IsKeyPressed(fm::FM_KEY_R))
		_state = gui::TRANSFORM_CONTEXT::ROTATE;
	else if (fm::InputManager::Get().IsKeyPressed(fm::FM_KEY_S))
		_state = gui::TRANSFORM_CONTEXT::SCALE;
}

void ToolBar::_DrawStartStop()
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
				fm::Application::Get().Start(true);
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
				fm::Application::Get().Stop();
			}
		}
	}
}



void ToolBar::_DrawTransformContext()
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


void ToolBar::CustomDraw()
{
	_DrawTransformContext();
	_DrawStartStop();
}