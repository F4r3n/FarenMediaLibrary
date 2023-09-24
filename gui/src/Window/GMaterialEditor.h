#pragma once

#include "Window/GWindow.h"
#include <memory>
#include <vector>
#include <string>
namespace fm
{
	class GameObject;
	class Material;
}

namespace gui
{
	class GMaterialEditor : public GWindow
	{
	public:
		GMaterialEditor();
		~GMaterialEditor();
		void CustomDraw();

		static void DrawMaterialInspector(fm::Material* inMaterial);
		static std::vector<std::string> GetValueTypeNames();
	private:
		void _Update(float dt, Context &inContext);

		std::weak_ptr<fm::Material>	_material;
	};
}

