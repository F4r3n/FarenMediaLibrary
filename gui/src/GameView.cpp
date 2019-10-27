#include "GameView.h"
#include <imgui/imgui.h>
#include "Components/CCamera.h"
#include "Core/SceneManager.h"
#include "imgui_internal.h"
#include "PickingSystem.h"
using namespace gui;
GameView::GameView() : GWindow("Game View", true, ImGuiWindowFlags_NoScrollbar) 
{
	_enabled = true;
	_gameObjectSelectedByPicking = nullptr;
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

		const fm::Texture texture = preview.renderTexture->GetColorBufferTexture(0);
		ImGui::GetWindowDrawList()->AddImage((void*)texture.getID(), ImVec2(_startImagePos.x, _startImagePos.y), ImVec2(_endImagePos.x, _endImagePos.y));
    }
}

void GameView::_CallBackPickingSystem(fm::GameObject* inGameObject)
{
	_resultPicking = true;
	_gameObjectSelectedByPicking = inGameObject;
}


void GameView::SetPickingSystem(fms::PickingSystem *inPickingSystem)
{
	_pickingSystem = inPickingSystem;
	std::function<void(fm::GameObject*)> f = std::bind(&gui::GameView::_CallBackPickingSystem, this, std::placeholders::_1);
	_pickingSystem->SetCallback(std::move(f));
}


void GameView::Update(float dt, Context &inContext)
{
	if (_index >= 0 && _index < _previews.size())
	{
		CameraPreview preview = _previews[_index];

		const float rapport = (float)preview.renderTexture->getWidth() / (float)preview.renderTexture->getHeight();


		ImVec2 start;
		ImVec2 size;
		if (_id != 0 || ImGui::IsWindowDocked())
		{
			start = ImGui::GetWindowDockPos(_id);
			size = ImGui::GetWindowDockSize(_id);
		}
		else
		{
			start = ImGui::GetWindowPos();
			size = ImGui::GetWindowPos();
		}
		ImVec2 end;
		end.x = start.x + size.x;
		end.y = start.y + size.x / rapport;

		_startImagePos = fm::math::vec2(start.x, start.y);
		_endImagePos = fm::math::vec2(end.x, end.y);
	}

	if (ImGui::IsMouseClicked(0))
	{		
		ImVec2 mousePos = ImGui::GetMousePos();

		fm::Rectf rect;
		rect.w = _endImagePos.x - _startImagePos.x;
		rect.h = _endImagePos.y - _startImagePos.y;
		rect.x = _startImagePos.x;
		rect.y = _startImagePos.y;

		const CameraPreview preview = _previews[_index];


		if (rect.contains(mousePos.x, mousePos.y))
		{
			fm::math::vec2 mPos(mousePos.x, mousePos.y);
			mPos.x -= _startImagePos.x;
			mPos.y -= _startImagePos.y;
			mPos.x = (mPos.x /(  _endImagePos.x - _startImagePos.x))*preview.renderTexture->getWidth();
			mPos.y = (mPos.y / (_endImagePos.y -_startImagePos.y))*preview.renderTexture->getHeight();

			_pickingSystem->PickGameObject(preview.id, mPos);
		}
	}

	if (ImGui::IsMouseClicked(1))
	{
		CameraPreview preview = _previews[_index];
		preview.renderTexture->GetColorBufferTexture(0).writeToPNG("C:\\Users\\guill\\Pictures\\yolo2.png");
	}

	if (_resultPicking)
	{
		inContext.currentGameObjectSelected = _gameObjectSelectedByPicking;
		_gameObjectSelectedByPicking = nullptr;
		_resultPicking = false;
	}
}


void GameView::AddCamera(fm::GameObject *inGameObject)
{
	CameraPreview preview;
	preview.id = inGameObject->getID();
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
