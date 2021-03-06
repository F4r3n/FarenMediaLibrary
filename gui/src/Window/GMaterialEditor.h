#pragma once

#include "Window/GWindow.h"
#include <memory>
#include "Rendering/material.hpp"
namespace fm
{
	class GameObject;
}

namespace gui
{
	class GMaterialEditor : public GWindow
	{
	public:
		GMaterialEditor();
		~GMaterialEditor();
		void CustomDraw();

	private:
		void _Update(float dt, Context &inContext);

		std::unique_ptr<fm::GameObject> _go;
		fm::Material*					_material = nullptr;
	};
}

