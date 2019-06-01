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
		void Update(float dt);
		bool HasGameObjectSelected();
		fm::GameObject* GetGameObjectSelected();
	private:
		void _RefreshAllEntities();


		std::vector<const char*> _namesEntities;
		float _timerListEntityUpdate;
		size_t _entitiesSelected;
		fm::GameObject* _gameObjectSelected;

	};
}

