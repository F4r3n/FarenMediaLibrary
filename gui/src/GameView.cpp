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
}

GameView::~GameView() 
{
	delete _pickingSystem;
}

void GameView::CustomDraw()
{
    if(index >= 0 && index < previews.size()) 
	{
		CameraPreview preview = previews[index];

        const float rapport = (float)preview.renderTexture->getWidth()/(float)preview.renderTexture->getHeight();

		const fm::Texture texture = preview.renderTexture->GetColorBufferTexture(0);

		ImVec2 start;
		ImVec2 size;
		if (ImGui::IsWindowDocked())
		{
			start = ImGui::GetWindowDockPos(ImGui::GetWindowDockID());
			size = ImGui::GetWindowDockSize(ImGui::GetWindowDockID());
		}
		else
		{
			start = ImGui::GetWindowPos();
			size = ImGui::GetWindowPos();
		}
		ImVec2 end;
		end.x = start.x + size.x;
		end.y = start.y + size.x/rapport;

        ImGui::GetWindowDrawList()->AddImage((void*)texture.getID(), start, end);
    }
}

void GameView::_CallBackPickingSystem(fm::GameObject* inGameObject)
{

}


void GameView::SetPickingSystem(fms::PickingSystem *inPickingSystem)
{
	_pickingSystem = inPickingSystem;
	std::function<void(fm::GameObject*)> f = std::bind(&gui::GameView::_CallBackPickingSystem, this, std::placeholders::_1);
	_pickingSystem->SetCallback(std::move(f));
}


void GameView::Update(float dt, Context &inContext)
{
	if (ImGui::IsMouseClicked(0))
	{
		ImVec2 pos = ImGui::GetWindowDockPos(_id);
		ImVec2 size = ImGui::GetWindowDockSize(_id);
		
		ImVec2 mousePos = ImGui::GetMousePos();

		fm::Rectf rect;
		rect.w = size.x;
		rect.h = size.y;
		rect.x = pos.x;
		rect.y = pos.y;

		CameraPreview preview = previews[index];


		if (rect.contains(mousePos.x, mousePos.y))
		{
			_pickingSystem->PickGameObject(preview.id, fm::math::vec2(mousePos.x, mousePos.y));
		}
	}

	if (ImGui::IsMouseClicked(1))
	{
		CameraPreview preview = previews[index];
		preview.renderTexture->GetColorBufferTexture(0).writeToPNG("C:\\Users\\guill\\Pictures\\yolo2.png");

	}
}


void GameView::AddCamera(fm::GameObject *inGameObject)
{
	CameraPreview preview;
	preview.id = inGameObject->getID();
	fmc::CCamera *camera = inGameObject->get<fmc::CCamera>();
	preview.renderTexture = std::make_shared<fm::RenderTexture>(fm::RenderTexture(camera->getInternalRenderTexture(), 0));
	camera->target = preview.renderTexture;

	previews.push_back(preview);
}

void GameView::RemoveCamera(fm::GameObject *inGameObject)
{
	auto i = std::begin(previews);

	while (i != std::end(previews)) 
	{
		if (inGameObject->getID() == i->id)
		{
			i = previews.erase(i);
			break;
		}
	}

	if (!previews.empty())
	{
		index = 0;
	}
}

bool GameView::SetMainCamera(fm::GameObject *inGameObject)
{
	for (size_t i = 0; i < previews.size(); ++i)
	{
		if (previews[i].id == inGameObject->getID())
		{
			index = i;
			return true;
		}
	}
	return false;
}



void GameView::Resize() {}
void GameView::Zoom() {}
