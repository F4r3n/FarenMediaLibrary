#pragma once
#include "Rendering/RenderTexture.h"
#include <memory>

class GameView {
    public:
        GameView();
        ~GameView();
        void draw();
        void resize();
        void zoom();
        std::shared_ptr<fm::RenderTexture> renderTexture;
    private:
};
