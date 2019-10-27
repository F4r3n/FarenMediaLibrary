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

		void SetPickingSystem(fms::PickingSystem *inPickingSystem);
	private:
		void _CallBackPickingSystem(fm::GameObject* inGameObject);

		fms::PickingSystem* _pickingSystem;
		std::vector<CameraPreview> _previews;
		int _index;
		fm::math::vec2 _startImagePos;
		fm::math::vec2 _endImagePos;

		bool _resultPicking;
		fm::GameObject* _gameObjectSelectedByPicking;
	};
}
