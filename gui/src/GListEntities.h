#pragma once
#include <memory>
#include <GWindow.h>

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

		fm::GameObject* _gameObjectSelected;
		bool _hasBeenSelected = false;
		std::string _currentSceneName;

	};
}

