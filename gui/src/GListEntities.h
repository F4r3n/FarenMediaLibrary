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
		bool			_hasBeenSelected = false;
		std::string		_currentSceneName;
		bool			_isRenaming;
		char			_bufferName[128];
		size_t			_itemSelected = -1;

	};
}

