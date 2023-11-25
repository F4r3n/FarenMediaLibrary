#pragma once
#include <memory>
#include <vector>
#include "GWindow.h"
#include <optional>
#include "Core/GameObjectType.hpp"
namespace fms
{
	class PickingSystem;
}

namespace fm
{
	class Scene;
	class FrameBuffer;
	class GameObject;
}
namespace gui
{
	class GEditorView : public GWindow
	{

		struct CameraPreview
		{
			std::optional<fm::GameObjectID_t> id;
			std::shared_ptr<fm::FrameBuffer> renderTexture;
		};


	public:
		GEditorView(std::shared_ptr<fm::GameObject> inCamera, std::shared_ptr<fm::Scene> inScene);
		virtual ~GEditorView();
		void CustomDraw();
		void BeforeWindowCreation();
		void AfterWindowCreation();
		void Resize();
		void Zoom();

		void SetPickingSystem(std::unique_ptr<fms::PickingSystem> inPickingSystem);
	private:
		void _Update(float dt, Context &inContext);

		void _DrawContentEditorCamera(Context &inContext);
		void _MoveCamera(float dt, const Context& inContext);

		void _EditObject();
		void _CallBackPickingSystem(fm::GameObjectID_t inID);
	private:
		bool _IsInView(fm::math::vec2 inPos);
		std::unique_ptr<fms::PickingSystem>		_pickingSystem;
		CameraPreview							_editorView;
		std::weak_ptr<fm::Scene>				_editorScene;

		fm::math::vec2			_startImagePos;
		fm::math::vec2			_endImagePos;
		fm::math::vec2			_cursorPos;
		fm::math::vec2			_mousePos;

		fm::math::vec2			_scrollPos;

		bool					_resultPicking;
		std::optional<fm::GameObjectID_t>	_gameObjectSelectedByPicking;
		gui::TRANSFORM_CONTEXT	_currentTransformContext;
	};
}
