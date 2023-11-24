#pragma once
#include "inspector.hpp"

namespace fmc
{
	class CCamera;
}

namespace fm
{
	class GameObject;
}

namespace gui
{

	class CameraInspector : public Inspector
	{
	public:
		virtual void Draw(bool* value, std::shared_ptr<fm::GameObject> inGameObject) override;
			void RemoveComponent(std::shared_ptr<fm::GameObject> inGameObject) override;
	private:
	};
}
