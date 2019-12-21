#pragma once
#include "GWindow.h"
#include "Core/application.h"
#include <functional>
namespace gui
{
	class ProjectSettingsWindow : public GWindow
	{
	public:
		ProjectSettingsWindow();
		virtual void CustomDraw() override;
		virtual void WillClose() override;
	private:
		
	};
}
