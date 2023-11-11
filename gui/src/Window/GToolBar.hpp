#pragma once
#include "GWindow.h"

namespace fm
{
	class Texture2D;
}

namespace gui
{
	class GToolBar : public GWindow
	{


	public:
		GToolBar();
		~GToolBar();
		virtual void CustomDraw() override;
	private:
		virtual void _Update(float, Context &inContext) override;
		virtual void			BeforeWindowCreation() override;
		virtual void			AfterWindowCreation() override;
		virtual void			OnInit() override;

		void _DrawStartStop();
		void _DrawTransformContext();
		void _UpdateInputTransformContext(Context &inContext);
		TRANSFORM_CONTEXT _state;
		std::shared_ptr<fm::Texture2D>	  _textureStart = nullptr;
		std::shared_ptr<fm::Texture2D>	  _textureStop = nullptr;

	};
}