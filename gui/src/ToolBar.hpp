#pragma once
#include "GWindow.h"

namespace gui
{
	class ToolBar : public GWindow
	{


	public:
		ToolBar();
		~ToolBar();
		virtual void CustomDraw() override;
	private:
		virtual void _Update(float, Context &inContext) override;

		void _DrawStartStop();
		void _DrawTransformContext();
		void _UpdateInputTransformContext();
		TRANSFORM_CONTEXT _state;
	};
}