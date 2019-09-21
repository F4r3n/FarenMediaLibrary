#pragma once
#include "Rendering/RenderTexture.h"
#include <memory>
#include "Core/GameObject.h"
#include <vector>
#include "GWindow.h"
namespace gui
{
	class GameView : public GWindow
	{
		struct CameraPreview
		{
			size_t id;
			std::shared_ptr<fm::RenderTexture> renderTexture;
		};


	public:
		GameView();
		virtual ~GameView();
		void CustomDraw();
		void Resize();
		void Zoom();
		void AddCamera(fm::GameObject *inGameObject);
		void RemoveCamera(fm::GameObject *inGameObject);
		bool SetMainCamera(fm::GameObject *inGameObject);
		void Update(float dt, Context &inContext);
	private:
		std::vector<CameraPreview> previews;
		int index = -1;
	};
}
