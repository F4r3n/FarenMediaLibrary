#pragma once
#include "GWindow.h"

namespace gui
{
	class ToolBar : public GWindow
	{


	public:
		ToolBar();
		~ToolBar();
		virtual void Update(float, Context &inContext) override;
		virtual void CustomDraw() override;
	private:
		void _DrawStartStop();
		void _DrawTransformContext();
		void _UpdateInputTransformContext();
		TRANSFORM_CONTEXT _state;
	};
}