#pragma once

#include "Window/GWindow.h"


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

	};
}

