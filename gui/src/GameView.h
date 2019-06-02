#pragma once
#include "Rendering/RenderTexture.h"
#include <memory>
#include "Core/GameObject.h"
#include <vector>

class GameView
{
	struct CameraPreview
	{
		size_t id;
		std::shared_ptr<fm::RenderTexture> renderTexture;
	};


    public:
        GameView();
        ~GameView();
        void Draw();
        void Resize();
        void Zoom();
		void AddCamera(fm::GameObject *inGameObject);
		void RemoveCamera(fm::GameObject *inGameObject);
		bool SetMainCamera(fm::GameObject *inGameObject);
		
    private:
		std::vector<CameraPreview> previews;
		int index = -1;
};
