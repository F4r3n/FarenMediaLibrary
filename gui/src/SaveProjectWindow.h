#pragma once
#include "GWindow.h"
#include "Core/application.h"
#include <functional>
namespace gui
{
	class SaveProjectWindow : public GWindow
	{
	public:
		SaveProjectWindow(const std::string &inName, std::function<void(void)> && f);
		virtual void CustomDraw() override;
		virtual void WillClose() override;
	private:
		fm::ProjectSettings _projectSettings;
		bool		_isInit;
		bool		_fileBrowserOpened;
		std::function<void(void)> _returnFunction;
	};
}
