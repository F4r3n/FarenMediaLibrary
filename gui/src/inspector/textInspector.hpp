#pragma once
#include "inspector.hpp"

namespace fm
{
	class GameObject;
}

namespace gui
{

	class TextInspector : public Inspector
	{
	public:
		virtual void Draw(bool* value, std::shared_ptr<fm::GameObject> inGameObject) override;
			void RemoveComponent(std::shared_ptr<fm::GameObject> inGameObject) override;
	private:

	};
}