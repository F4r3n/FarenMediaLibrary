#pragma once

#include "GWindow.h"
#include "Core/FilePath.h"
namespace gui
{
	class GLauncher : public GWindow
	{

	public:
		GLauncher();
		virtual ~GLauncher();
		const fm::FilePath& GetResult() const { return _result; }

	protected:
		void OnInit();
		void CustomDraw();
		void BeforeWindowCreation();
		void AfterWindowCreation();

	private:
		void _Update(float dt, Context &inContext);
		void _DisplayWindow_Create_Project();
		

		fm::FilePath _result;
		std::vector<fm::FilePath> _listProjects;
		int _projectSelected = -1;
		
	};
}
