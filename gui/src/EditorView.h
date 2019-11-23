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

namespace fm
{
	class Scene;
}
namespace gui
{
	class EditorView : public GWindow
	{

		struct CameraPreview
		{
			size_t id;
			std::shared_ptr<fm::RenderTexture> renderTexture;
			bool enabled;
		};


	public:
		EditorView(fm::GameObject* inCamera, std::shared_ptr<fm::Scene> inScene);
		virtual ~EditorView();
		void CustomDraw();
		void BeforeWindowCreation();
		void AfterWindowCreation();
		void Resize();
		void Zoom();
		void Update(float dt, Context &inContext);

		void SetPickingSystem(fms::PickingSystem *inPickingSystem);
	private:
		void _DrawContentEditorCamera();

		void _EditObject();
		void _CallBackPickingSystem(fm::GameObject* inGameObject);

		fms::PickingSystem*		_pickingSystem;
		CameraPreview			_editorView;
		std::shared_ptr<fm::Scene> _editorScene;

		fm::math::vec2			_startImagePos;
		fm::math::vec2			_endImagePos;
		fm::math::vec2			_cursorPos;

		bool					_resultPicking;
		fm::GameObject*			_gameObjectSelectedByPicking;
		gui::TRANSFORM_CONTEXT	_currentTransformContext;
	};
}
