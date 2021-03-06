#pragma once
#include "GWindow.h"

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

		void _DrawStartStop();
		void _DrawTransformContext();
		void _UpdateInputTransformContext(Context &inContext);
		TRANSFORM_CONTEXT _state;
		 
	};
}