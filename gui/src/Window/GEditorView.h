#pragma once
#include <FML/Rendering/RenderTexture.h>
#include <memory>
#include <FML/Core/GameObject.h>
#include <vector>
#include "GWindow.h"
#include <optional>
namespace fms
{
	class PickingSystem;
}

namespace fm
{
	class Scene;
}
namespace gui
{
	class GEditorView : public GWindow
	{

		struct CameraPreview
		{
			std::optional<Entity::Id> id;
			std::weak_ptr<fm::RenderTexture> renderTexture;
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

		void _EditObject();
		void _CallBackPickingSystem(Entity::Id inID);
	private:
		std::unique_ptr<fms::PickingSystem>		_pickingSystem;
		CameraPreview							_editorView;
		std::weak_ptr<fm::Scene>				_editorScene;

		fm::math::vec2			_startImagePos;
		fm::math::vec2			_endImagePos;
		fm::math::vec2			_cursorPos;
		fm::math::vec2			_scrollPos;

		bool					_resultPicking;
		std::optional<Entity::Id>	_gameObjectSelectedByPicking;
		gui::TRANSFORM_CONTEXT	_currentTransformContext;
	};
}
