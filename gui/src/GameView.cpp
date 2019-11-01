#include "GameView.h"
#include <imgui/imgui.h>
#include "Components/CCamera.h"
#include "Core/SceneManager.h"
#include "imgui_internal.h"
#include "PickingSystem.h"
#include "ImGuizmo/ImGuizmo.h"
#include "Input/InputManager.h"
using namespace gui;
GameView::GameView() : GWindow("Game View", true, ImGuiWindowFlags_NoScrollbar| ImGuiWindowFlags_NoNavInputs)
{
	_enabled = true;
	_gameObjectSelectedByPicking = nullptr;
	_index = -1;
	_pickingSystem = nullptr;
	_resultPicking = false;
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

			ImVec2 ori = ImGui::GetCursorPos();
			ImGui::SetCursorPos(ImVec2(_cursorPos.x, _cursorPos.y));
			ImGui::Image((ImTextureID)texture.getID(), ImVec2(texture.getWidth(), texture.getHeight()));
			ImGui::SetCursorPos(ori);
			ImGuizmo::SetDrawlist();
			_EditObject();
		}
    }
}



void GameView::_EditObject()
{
	if (_index >= 0 && _index < _previews.size())
	{
		CameraPreview preview = _previews[_index];
		fm::GameObject* camera = preview.go;
		if (_gameObjectSelectedByPicking == nullptr)
			return;

		static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
		static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
		if (fm::InputManager::Get().IsKeyPressed(fm::FM_KEY_T))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		if (fm::InputManager::Get().IsKeyPressed(fm::FM_KEY_R))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		if (fm::InputManager::Get().IsKeyPressed(fm::FM_KEY_S)) // r Key
			mCurrentGizmoOperation = ImGuizmo::SCALE;


		fmc::CTransform *transform = _gameObjectSelectedByPicking->get<fmc::CTransform>();


		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(_startImagePos.x, _startImagePos.y, _endImagePos.x, _endImagePos.y);
		const fm::math::mat view = camera->get<fmc::CTransform>()->GetLocalMatrixModel();
		const fm::math::mat projecttion = camera->get<fmc::CCamera>()->projection;
		fm::math::mat model = transform->GetLocalMatrixModel();

		fm::math::vec3 rotation = transform->rotation;
		fm::math::vec3 position = transform->position;
		fm::math::vec3 scale = transform->scale;
		ImGuizmo::RecomposeMatrixFromComponents(&position[0], &rotation[0], &scale[0], &model[0][0]);

		ImGuizmo::Manipulate(&view[0][0], &projecttion[0][0], mCurrentGizmoOperation, mCurrentGizmoMode, &model[0][0], NULL, NULL);

		ImGuizmo::DecomposeMatrixToComponents(&model[0][0], &transform->position[0], &transform->rotation[0], &transform->scale[0]);
		
	}

}


void GameView::_CallBackPickingSystem(fm::GameObject* inGameObject)
{
	if (inGameObject != nullptr)
	{
		_resultPicking = true;
		_gameObjectSelectedByPicking = inGameObject;
	}
}


void GameView::SetPickingSystem(fms::PickingSystem *inPickingSystem)
{
	_pickingSystem = inPickingSystem;
	std::function<void(fm::GameObject*)> f = std::bind(&gui::GameView::_CallBackPickingSystem, this, std::placeholders::_1);
	_pickingSystem->SetCallback(std::move(f));
}


void GameView::Update(float dt, Context &inContext)
{
	bool isRenderTextureReady = false;

	if (_index >= 0 && _index < _previews.size())
	{
		CameraPreview preview = _previews[_index];
		isRenderTextureReady = preview.renderTexture != nullptr && preview.renderTexture->isCreated();
	}

	assert(GImGui != nullptr && GImGui->CurrentWindow != nullptr);
	if (isRenderTextureReady)
	{
		ImVec2 size;
		ImVec2 start;

		if (_index >= 0 && _index < _previews.size())
		{
			CameraPreview preview = _previews[_index];

			const float rapport = (float)preview.renderTexture->getWidth() / (float)preview.renderTexture->getHeight();

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
			end.x = start.x + preview.renderTexture->getWidth();
			end.y = start.y + preview.renderTexture->getHeight();

			_cursorPos = fm::math::vec2(-(preview.renderTexture->getWidth() - size.x)*0.5f, 
										-(preview.renderTexture->getHeight() - size.y)*0.5f);
			_startImagePos = _cursorPos;
			_startImagePos.y += start.y;
			_startImagePos.x += start.x;

			_endImagePos = fm::math::vec2(end.x, end.y);
		}

		if (ImGui::IsMouseClicked(0))
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
				mPos.x -= _startImagePos.x;
				mPos.y -= _startImagePos.y;

				_pickingSystem->PickGameObject(preview.id, mPos);
			}
		}


		if (_resultPicking)
		{
			inContext.currentGameObjectSelected = _gameObjectSelectedByPicking;
			_gameObjectSelectedByPicking = inContext.currentGameObjectSelected;
			_resultPicking = false;
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
