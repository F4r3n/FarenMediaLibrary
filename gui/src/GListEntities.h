#pragma once
#include <memory>
#include <GWindow.h>
#include <optional>
namespace fm
{
	class GameObject;
}


namespace gui
{
	class GListEntities : public GWindow
	{
	public:
		GListEntities();
		~GListEntities();
		void CustomDraw();
	private:
		void _Update(float dt, Context &inContext);

		std::optional<ecs::id>	_gameObjectSelected;
		std::string				_currentSceneName;
		bool					_isRenaming;
		char					_bufferName[128];
		bool					_goSelectedHasChanged = false;

	};
}

