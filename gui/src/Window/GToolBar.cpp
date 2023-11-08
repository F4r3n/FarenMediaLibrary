#include "GToolBar.hpp"
#include "Input/InputManager.h"
#include "Core/application.h"
#include "Editor.h"
#include "Rendering/Texture2D.hpp"
using namespace gui;

GToolBar::GToolBar() : GWindow("GToolBar", true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize)
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
	if (_textureStart == nullptr)
	{
		_textureStart = fm::Texture2D::CreateTexture2D(GRAPHIC_API::OPENGL,
			fm::FilePath(fm::LOCATION::INTERNAL_RESOURCES_LOCATION, "editor/images/PlayButton.png"));
	}

	if (_textureStop == nullptr)
	{
		_textureStop = fm::Texture2D::CreateTexture2D(GRAPHIC_API::OPENGL,
			fm::FilePath(fm::LOCATION::INTERNAL_RESOURCES_LOCATION, "editor/images/StopButton.png"));
	}

	_UpdateInputTransformContext(inContext);

	inContext.currentTransformContext = _state;
}

void GToolBar::_UpdateInputTransformContext(Context &inContext)
{
	ImGuiIO io = ImGui::GetIO();

	if (inContext.currentWindowFocused == WINDOWS::WIN_EDITOR_VIEW)
	{
		if (io.KeyAlt && ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_T, false))
		{
			_state = gui::TRANSFORM_CONTEXT::TRANSLATE;
		}
		if (io.KeyAlt && ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_R, false))
		{
			_state = gui::TRANSFORM_CONTEXT::ROTATE;
		}
		if (io.KeyAlt && ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_S, false))
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
		bool status = fm::Application::Get().IsRunning();
		if (status)
		{
			ImGui::BeginDisabled();
		}

		if (ImGui::ImageButton("textureStart", ImTextureID((intptr_t)_textureStart->GetID()), ImVec2(25, 25)))
		{
			AddEvent([](GWindow*, std::optional<gui::Context> context) {
				Editor::Get().Start();
			});	
		}
		if (status)
		{
			ImGui::EndDisabled();
		}
	}

	ImGui::SameLine();
	{
		bool status = !fm::Application::Get().IsRunning();
		if (status)
		{
			ImGui::BeginDisabled();
		}
		if (ImGui::ImageButton("textureStop", ImTextureID((intptr_t)_textureStop->GetID()), ImVec2(25, 25)))
		{

			AddEvent([](GWindow*, std::optional<gui::Context> Context) {
				Editor::Get().Stop();
			});
		}
		if (status)
		{
			ImGui::EndDisabled();
		}
	}
}



void GToolBar::_DrawTransformContext()
{
	const ImVec2 buttonSize = ImVec2(30, 30);
	const char* labelT = "T\0";
	bool status = _state == gui::TRANSFORM_CONTEXT::TRANSLATE;
	if (status)
	{
		ImGui::BeginDisabled();
	}
	if (ImGui::Button(labelT, buttonSize))
	{
		_state = gui::TRANSFORM_CONTEXT::TRANSLATE;
	}
	if (status)
	{
		ImGui::EndDisabled();
	}

	ImGui::SameLine();
	const char* labelS = "S\0";
	status = _state == gui::TRANSFORM_CONTEXT::SCALE;
	if (status)
	{
		ImGui::BeginDisabled();
	}
	if (ImGui::Button(labelS, buttonSize))
	{
		_state = gui::TRANSFORM_CONTEXT::SCALE;
	}
	if (status)
	{
		ImGui::EndDisabled();
	}

	ImGui::SameLine();
	const char* labelR = "R\0";
	status = _state == gui::TRANSFORM_CONTEXT::ROTATE;
	if (status)
	{
		ImGui::BeginDisabled();
	}
	if (ImGui::Button(labelR, buttonSize))
	{
		_state = gui::TRANSFORM_CONTEXT::ROTATE;
	}
	if (status)
	{
		ImGui::EndDisabled();
	}
}


void GToolBar::CustomDraw()
{
	_DrawTransformContext();
	_DrawStartStop();
}