#include "GameView.h"
#include <imgui/imgui.h>
GameView::GameView() {

}


GameView::~GameView() {}

void GameView::draw() {
    static bool show_game_view = true;
    if(show_game_view) {
        float rapport = (float)renderTexture->getWidth()/(float)renderTexture->getHeight();

        float scale = 500;
        ImGui::SetNextWindowSize(ImVec2(rapport*scale, scale));
        ImGui::Begin("Game View", &show_game_view,ImGuiWindowFlags_NoScrollbar );
        fm::Texture texture = renderTexture->getColorBuffer()[0];

        ImVec2 pos = ImGui::GetWindowPos();
        pos.y += 10;
        ImVec2 end;
        end.x = pos.x + ImGui::GetWindowSize().x;
        end.y = pos.y + ImGui::GetWindowSize().y;

        ImGui::GetWindowDrawList()->AddImage((void*)texture.getID(),ImGui::GetWindowPos(), end);

        //ImGui::Image((void*)texture.getID(), ImGui::GetWindowSize(), ImVec2(0,0), ImVec2(1,1), ImColor(255,255,255,255), ImColor(255,255,255,128));
        ImGui::End();
    }

}

void GameView::resize() {}
void GameView::zoom() {}
