#pragma once
#include "inspector.hpp"

namespace fmc
{
	class CMesh;
}

namespace fm
{
	class GameObject;
}

namespace gui
{

	class MeshInspector : public Inspector
	{
	public:
		virtual void Draw(bool* value, std::shared_ptr<fm::GameObject> inGameObject) override;
		void RemoveComponent(std::shared_ptr<fm::GameObject> inGameObject) override;
	private:

	};
}
