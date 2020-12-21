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
	class GGameView : public GWindow
	{

		struct CameraPreview
		{
			std::weak_ptr<fm::RenderTexture> renderTexture;
		};

		enum ASPECT_MODE
		{
			ASPECT_FREE,
			ASPECT_16_9
		};


	public:
		GGameView();
		virtual ~GGameView();
		void CustomDraw();
		void BeforeWindowCreation();
		void AfterWindowCreation();
		void Resize();
		void Zoom();
		void AddCamera(std::shared_ptr<fm::GameObject> inGameObject);
		void Clear();
	private:
		void _Update(float dt, Context &inContext);

		CameraPreview				_preview;
		fm::math::vec2				_startImagePos;
		fm::math::vec2				_endImagePos;
		ASPECT_MODE					_aspectMode;
		
	};
}
