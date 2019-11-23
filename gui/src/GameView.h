#pragma once
#include "Rendering/RenderTexture.h"
#include <memory>
#include "Core/GameObject.h"
#include <vector>
#include "GWindow.h"

namespace fms
{
	class PickingSystem;
}
namespace gui
{
	class GameView : public GWindow
	{

		struct CameraPreview
		{
			std::shared_ptr<fm::RenderTexture> renderTexture;
		};

		enum ASPECT_MODE
		{
			ASPECT_FREE,
			ASPECT_16_9
		};


	public:
		GameView();
		virtual ~GameView();
		void CustomDraw();
		void BeforeWindowCreation();
		void AfterWindowCreation();
		void Resize();
		void Zoom();
		void AddCamera(fm::GameObject *inGameObject);
		void Update(float dt, Context &inContext);
		void Clear();
	private:

		std::vector<CameraPreview>	_previews;
		size_t						_index;
		fm::math::vec2				_startImagePos;
		fm::math::vec2				_endImagePos;
		ASPECT_MODE					_aspectMode;
	};
}
