#include "GameView.h"
#include <imgui/imgui.h>
#include "Components/CCamera.h"
#include "Core/SceneManager.h"

GameView::GameView() {

}


GameView::~GameView() {}

void GameView::draw() {
    static bool show_game_view = true;
    if(show_game_view && index != -1) 
	{
		CameraPreview preview = previews[index];


        float rapport = (float)preview.renderTexture->getWidth()/(float)preview.renderTexture->getHeight();

        float scale = 500;
        ImGui::SetNextWindowSize(ImVec2(rapport*scale, scale));
        ImGui::Begin("Game View", &show_game_view,ImGuiWindowFlags_NoScrollbar );
        fm::Texture texture = preview.renderTexture->getColorBuffer()[0];

        ImVec2 pos = ImGui::GetWindowPos();
        pos.y += 10;
        ImVec2 end;
        end.x = pos.x + ImGui::GetWindowSize().x;
        end.y = pos.y + ImGui::GetWindowSize().y;

        ImGui::GetWindowDrawList()->AddImage((void*)texture.getID(),ImGui::GetWindowPos(), end);

        ImGui::End();
    }

}

void GameView::AddCamera(fm::GameObject *inGameObject)
{
	
	CameraPreview preview;
	preview.id = inGameObject->getID();
	fmc::CCamera *camera = inGameObject->get<fmc::CCamera>();
	preview.renderTexture = std::make_shared<fm::RenderTexture>(fm::RenderTexture(*camera->getInternalRenderTexture().get(), 0));
	camera->target = preview.renderTexture;

	previews.push_back(preview);
	
}

void GameView::RemoveCamera(fm::GameObject *inGameObject)
{


	auto i = std::begin(previews);
	int idDeleted = 0;
	while (i != std::end(previews)) {
		// Do some stuff
		if (inGameObject->getID() == i->id)
		{
			i = previews.erase(i);
			break;
		}
		idDeleted++;
	}

	if (!previews.empty())
	{
		index = 0;
	}
}

bool GameView::SetMainCamera(fm::GameObject *inGameObject)
{
	for (int i = 0; i < previews.size(); ++i)
	{
		if (previews[i].id == inGameObject->getID())
		{
			index = i;
			return true;
		}
	}
	return false;
}



void GameView::resize() {}
void GameView::zoom() {}
