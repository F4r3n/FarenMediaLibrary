#pragma once

#include "GWindow.h"

namespace gui
{
	class GLauncher : public GWindow
	{

	public:
		GLauncher();
		virtual ~GLauncher();

	protected:
		void OnInit();
		void CustomDraw();
		void BeforeWindowCreation();
		void AfterWindowCreation();
	private:
		void _Update(float dt, Context &inContext);

		
	};
}
