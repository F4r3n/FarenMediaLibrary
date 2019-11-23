#include "GameView.h"
#include "Components/CCamera.h"
#include "Core/SceneManager.h"
#include "PickingSystem.h"
#include "ImGuizmo/ImGuizmo.h"
#include "Input/InputManager.h"
#include <imgui/imgui_internal.h>

using namespace gui;
GameView::GameView() : GWindow("Game View", true, ImGuiWindowFlags_HorizontalScrollbar
												)
{
	_enabled = true;
	_index = -1;
}

GameView::~GameView() 
{
	delete _pickingSystem;
}

void GameView::CustomDraw()
{
    if(_index >= 0 && _index < _previews.size()) 
	{
		CameraPreview preview = _previews[_index];
		if (preview.renderTexture != nullptr && preview.renderTexture->isCreated())
		{
			const fm::Texture texture = preview.renderTexture->GetColorBufferTexture(0);

			ImGui::SetCursorPos(ImVec2(_cursorPos.x, _cursorPos.y));
			ImGui::Image((ImTextureID)texture.getID(), ImVec2(texture.getWidth(), texture.getHeight()));
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


void GameView::Update(float dt, Context &inContext)
{
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
			end.x = start.x + size.x;
			end.y = start.y + size.y;

			_startImagePos.x = start.x;
			_startImagePos.y = start.y;

			_cursorPos = fm::math::vec2(-(preview.renderTexture->getWidth() - size.x)*0.5f, 
										-(preview.renderTexture->getHeight() - size.y)*0.5f);
			startCursorPos = _cursorPos;
			startCursorPos.y += start.y;
			startCursorPos.x += start.x;

			_endImagePos = fm::math::vec2(end.x, end.y);
		}

		if (ImGui::IsMouseReleased(0) && !ImGuizmo::IsUsing())
		{
			ImVec2 mousePos = ImGui::GetMousePos();

			fm::Rectf rect;
			rect.w = size.x;
			rect.h = size.y;
			rect.x = start.x;
			rect.y = start.y;

			const CameraPreview preview = _previews[_index];


			if (rect.contains(mousePos.x, mousePos.y))
			{
				fm::math::vec2 mPos(mousePos.x, mousePos.y);
				mPos.x -= startCursorPos.x;
				mPos.y -= startCursorPos.y;

				_pickingSystem->PickGameObject(preview.id, mPos);
			}
		}

	}
}


void GameView::AddCamera(fm::GameObject *inGameObject)
{
	CameraPreview preview;
	preview.id = inGameObject->getID();
	preview.go = inGameObject;
	fmc::CCamera *camera = inGameObject->get<fmc::CCamera>();
	preview.renderTexture = std::make_shared<fm::RenderTexture>(fm::RenderTexture(camera->getInternalRenderTexture(), 0));
	camera->target = preview.renderTexture;

	_previews.push_back(preview);
}

void GameView::RemoveCamera(fm::GameObject *inGameObject)
{
	auto i = std::begin(_previews);

	while (i != std::end(_previews)) 
	{
		if (inGameObject->getID() == i->id)
		{
			i = _previews.erase(i);
			break;
		}
	}

	if (!_previews.empty())
	{
		_index = 0;
	}
}

bool GameView::SetMainCamera(fm::GameObject *inGameObject)
{
	for (size_t i = 0; i < _previews.size(); ++i)
	{
		if (_previews[i].id == inGameObject->getID())
		{
			_index = i;
			return true;
		}
	}
	return false;
}



void GameView::Resize() {}
void GameView::Zoom() {}
