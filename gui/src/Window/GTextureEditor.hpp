#pragma once
#include "GWindow.h"

namespace fm
{
	class Texture2D;
	class Texture;
}

namespace gui
{
	class GTextureEditor : public GWindow
	{
			public:

		GTextureEditor();
		~GTextureEditor();
		void CustomDraw();

	private:
		void _Update(float dt, Context& inContext);

		std::shared_ptr<fm::Texture2D>	_textureIcon = nullptr;
		std::shared_ptr<fm::Texture>	_texture = nullptr;
	};
}