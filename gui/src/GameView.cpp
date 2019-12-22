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
	_kind = gui::WINDOWS::WIN_SCENE_VIEW;

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

			ImGui::GetWindowDrawList()->AddImage((ImTextureID)texture.getID(),_startImagePos,_endImagePos);
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

	bool isRenderTextureReady = false;

	if (_index >= 0 && _index < _previews.size())
	{
		CameraPreview preview = _previews[_index];
		isRenderTextureReady = preview.renderTexture != nullptr && preview.renderTexture->isCreated();
	}

	if (isRenderTextureReady && HasBeenDrawn())
	{
		fm::math::vec2 size;
		fm::math::vec2 start;
		fm::math::vec2 startCursorPos;
		if (_index >= 0 && _index < _previews.size())
		{
			CameraPreview preview = _previews[_index];

			const float rapport = (float)preview.renderTexture->getWidth() / (float)preview.renderTexture->getHeight();

			start = GetPosition();
			size = GetSize();
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
	if (camera->HasTarget() && camera->GetTarget()->isCreated())
	{
		preview.renderTexture = camera->GetTarget();
	}
	else
	{
		preview.renderTexture = camera->SetTarget();
	}

	fm::Debug::logErrorExit(glGetError(), __FILE__, __LINE__);

	_previews.push_back(preview);
	_index = _previews.size() - 1;
}

void GameView::Clear()
{
	_index = 0;
	_previews.clear();
}





void GameView::Resize() {}
void GameView::Zoom() {}
