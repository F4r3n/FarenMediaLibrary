#include "GameView.h"
#include <imgui/imgui.h>
#include "Components/CCamera.h"
#include "Core/SceneManager.h"
#include "imgui_internal.h"

GameView::GameView() {}

GameView::~GameView() {}

void GameView::Draw()
{
    static bool show_game_view = true;
    if(show_game_view && index != -1) 
	{
		CameraPreview preview = previews[index];

        float rapport = (float)preview.renderTexture->getWidth()/(float)preview.renderTexture->getHeight();

        //ImGui::SetNextWindowSize(ImVec2(rapport*scale, scale));
        ImGui::Begin("Game View", &show_game_view,ImGuiWindowFlags_NoScrollbar );
		fm::Texture texture = preview.renderTexture->getColorBuffer()[0];

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

        ImGui::End();
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

	while (i != std::end(previews)) {

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
