#pragma once
#include <Core/FilePath.h>


namespace gui
{
	class ExportManager
	{
	public:
		struct ExportSettings
		{
			fm::FilePath destination;
		};

		ExportManager(const ExportSettings& inSettings);
		ExportManager() = delete;
		bool Run();
	private:
		ExportSettings _settings;
	};
}