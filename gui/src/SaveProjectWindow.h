#pragma once
#include "GWindow.h"
#include "Core/application.h"
namespace gui
{
	class SaveProjectWindow : public GWindow
	{
	public:
		SaveProjectWindow(const std::string &inName);
		virtual void CustomDraw() override;
		virtual void WillClose() override;
	private:
		fm::ProjectSettings _projectSettings;
		bool		_isInit;
		bool		_fileBrowserOpened;
	};
}
