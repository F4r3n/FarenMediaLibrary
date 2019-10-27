#ifndef DEBUGLOGGE_H
#define DEBUGLOGGE_H
#include <Core/Debug.h>
#include <imgui/imgui.h>
#include <mutex>
#include <GWindow.h>

namespace gui
{
	class DebugLogger : public gui::GWindow
	{
	public:
		DebugLogger();
		~DebugLogger();
		void    Clear();
		void    CustomDraw() override;
		void	Update(float, Context &inContex) override;
		void    AddLog(const fm::Debug::Message &message);

	private:
		std::vector<char>   _buffer;
		ImVector<int>       _lineOffsets;        // Index to lines offset
		bool                _scrollToBottom;
		std::mutex          _mutex;
	};
}

#endif

