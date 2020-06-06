#pragma once
#include <memory>
#include <GWindow.h>
#include <optional>
#include "Core/Observer.h"
namespace fm
{
	class GameObject;
}


namespace gui
{
	class GListEntities : public GWindow, public fm::Observer
	{
	public:
		GListEntities();
		~GListEntities();
		void CustomDraw();
		virtual void Notify(fm::Observable*, const fm::EventObserver& inEvent) override;
	private:
		void _Update(float dt, Context &inContext);

		std::optional<ecs::id>	_gameObjectSelected;
		std::string				_currentSceneName;
		bool					_isRenaming;
		char					_bufferName[128];
		bool					_goSelectedHasChanged = false;

		std::list<ecs::id>		_order;

	};
}

