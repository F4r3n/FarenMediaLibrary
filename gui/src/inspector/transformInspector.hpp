#pragma once
#include "inspector.hpp"
namespace fmc
{
	class CTransform;
}

namespace fm
{
	class GameObject;
}

namespace gui
{

	class TransformInspector : public Inspector
	{
	public:
		virtual void Draw(bool* value, std::shared_ptr<fm::GameObject> inGameObject) override;
			void RemoveComponent(std::shared_ptr<fm::GameObject> inGameObject) override;
	private:

	};
}