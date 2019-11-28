#include "GameView.h"
#include "Components/CCamera.h"
#include "Core/SceneManager.h"
#include "PickingSystem.h"
#include "ImGuizmo/ImGuizmo.h"
#include "Input/InputManager.h"
#include <imgui/imgui_internal.h>
#include "Core/Scene.h"
#include "Core/Debug.h"
using namespace gui;
GameView::GameView() : GWindow("Game View", true, ImGuiWindowFlags_HorizontalScrollbar
												)
{
	_enabled = true;
	_index = -1;
	_aspectMode = ASPECT_MODE::ASPECT_16_9;
	_shouldClear = false;
}

GameView::~GameView() 
{
}

void GameView::CustomDraw()
{

    if(_index >= 0 && _index < _previews.size()) 
	{
		CameraPreview preview = _previews[_index];
		if (preview.renderTexture != nullptr && preview.renderTexture->isCreated())
		{
			const fm::Texture texture = preview.renderTexture->GetColorBufferTexture(0);

			//ImGui::SetCursorPos(ImVec2(_cursorPos.x, _cursorPos.y));
			ImGui::GetWindowDrawList()->AddImage((ImTextureID)texture.getID(),
				ImVec2(_startImagePos.x, _startImagePos.y),
				ImVec2(_endImagePos.x, _endImagePos.y)
			);
			//ImGui::Image((ImTextureID)texture.getID(), ImVec2(texture.getWidth(), texture.getHeight()));
		}
    }

}

void GameView::BeforeWindowCreation()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
}
void GameView::AfterWindowCreation()
{
	ImGui::PopStyleVar(1);
}


void GameView::_Update(float dt, Context &inContext)
{
	if (_shouldClear)
	{
		_Clear();
		_shouldClear = false;
	}

	bool isRenderTextureReady = false;

	if (_index >= 0 && _index < _previews.size())
	{
		CameraPreview preview = _previews[_index];
		isRenderTextureReady = preview.renderTexture != nullptr && preview.renderTexture->isCreated();
	}

	//assert(GImGui != nullptr && GImGui->CurrentWindow != nullptr);
	if (isRenderTextureReady)
	{
		ImVec2 size;
		ImVec2 start;
		fm::math::vec2 startCursorPos;
		if (_index >= 0 && _index < _previews.size())
		{
			CameraPreview preview = _previews[_index];

			const float rapport = (float)preview.renderTexture->getWidth() / (float)preview.renderTexture->getHeight();
			//ImGui::IsWindowDocked
			if (_id != 0 || ImGui::IsWindowDocked())
			{

				//ImGui::getwind
				start = ImGui::GetWindowDockPos(_id);
				size = ImGui::GetWindowDockSize(_id);
			}
			else
			{
				start = ImGui::GetWindowPos();
				size = ImGui::GetWindowSize();
			}
			ImVec2 end;
			if (_aspectMode == ASPECT_MODE::ASPECT_FREE)
			{
				end.x = start.x + size.x;
				end.y = start.y + size.y;
			}
			else if (_aspectMode == ASPECT_MODE::ASPECT_16_9)
			{
				end.x = start.x + size.x;
				end.y = start.y + size.x*9.0f/16.0f;
			}

			_startImagePos.x = start.x;
			_startImagePos.y = start.y;


			_endImagePos = fm::math::vec2(end.x, end.y);

			if (_aspectMode == ASPECT_MODE::ASPECT_16_9)
			{
				fm::math::vec2 offset;
				offset.x = 0;
				offset.y = size.y*0.5f - (end.y - start.y)*0.5f;
				_startImagePos += offset;
				_endImagePos += offset;
			}
		}

	}
}


void GameView::AddCamera(fm::GameObject *inGameObject)
{
	CameraPreview preview;
	fmc::CCamera *camera = inGameObject->get<fmc::CCamera>();
	if (!camera->IsInit())
		camera->Init();
	preview.renderTexture = std::make_shared<fm::RenderTexture>(fm::RenderTexture(camera->getInternalRenderTexture(), 0));
	camera->target = preview.renderTexture;
	fm::Debug::logErrorExit(glGetError(), __FILE__, __LINE__);

	_previews.push_back(preview);
	_index = _previews.size() - 1;
}

void GameView::Clear()
{
	_shouldClear = true;
}
void GameView::_Clear()
{
	_index = 0;
	_previews.clear();

}




void GameView::Resize() {}
void GameView::Zoom() {}
