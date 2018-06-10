#include "GameView.h"
#include <imgui/imgui.h>
#include <iostream>
GameView::GameView() {

}


GameView::~GameView() {}

void GameView::draw() {
    static bool show_game_view = true;
    if(show_game_view) {
        float rapport = (float)renderTexture->getWidth()/(float)renderTexture->getHeight();
        //std::cout << rapport << std::endl;
        float scale = 500;
        ImGui::SetNextWindowSize(ImVec2(rapport*scale, scale));
        ImGui::Begin("Game View", &show_game_view );
        fm::Texture texture = renderTexture->getColorBuffer()[0];
        ImGui::Image((void*)texture.getID(), ImGui::GetWindowSize(), ImVec2(0,0), ImVec2(1,1), ImColor(255,255,255,255), ImColor(255,255,255,128));
        ImGui::End();
    }

}

void GameView::resize() {}
void GameView::zoom() {}
