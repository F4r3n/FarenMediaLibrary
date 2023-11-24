#pragma once
#include "inspector.hpp"
#include "Window/GMaterialEditor.h"
#include <vector>

namespace fmc
{
	class CPointLight;
}

namespace fm
{
	class GameObject;
}

namespace gui
{

	class MaterialInspector : public Inspector
	{
	public:
		MaterialInspector();
		virtual void Draw(bool* value, std::shared_ptr<fm::GameObject> inGameObject) override;
		void RemoveComponent(std::shared_ptr<fm::GameObject> inGameObject) override;
	private:
		GMaterialEditor			_editor;
	};
}
