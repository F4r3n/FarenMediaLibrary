#ifndef DEBUGLOGGE_H
#define DEBUGLOGGE_H
#include <Core/Debug.h>
#include <imgui/imgui.h>
#include <mutex>
#include "Window/GWindow.h"

namespace gui
{
	class GDebugLogger : public gui::GWindow
	{
	public:
		GDebugLogger();
		~GDebugLogger();
		void    Clear();
		void    CustomDraw() override;
		void    AddLog(const fm::Debug::Message &message);

	private:
		void	_Update(float, Context &inContex) override;

		std::vector<char>   _buffer;
		ImVector<int>       _lineOffsets;        // Index to lines offset
		bool                _scrollToBottom;
		std::mutex          _mutex;
	};
}

#endif

