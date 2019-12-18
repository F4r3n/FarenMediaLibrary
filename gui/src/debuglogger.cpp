#include "debuglogger.h"
using namespace gui;

DebugLogger::DebugLogger() : GWindow("Logger", true)
{
    _buffer.clear();
    _buffer.push_back(0);
    _buffer.shrink_to_fit();
	_enabled = true;
	_kind = gui::WINDOWS::WIN_LOGGER;
}



DebugLogger::~DebugLogger()
{

}

void DebugLogger::Clear()
{
    _mutex.lock();
    _buffer.clear();
    _buffer.push_back(0);
    _buffer.shrink_to_fit();
    _mutex.unlock();
}

void DebugLogger::AddLog(const fm::Debug::Message &message)
{
    _mutex.lock();
    if(_buffer.size() > 0 && _buffer[0] == 0)
    {
        _buffer[0] = 'a';
    }

    std::string m = message.content;
    m.append("\n");
    for(size_t i = 0; i < m.size();++i)//Todo improve allocation
        _buffer.push_back(m[i]);

    _scrollToBottom = true;
    _mutex.unlock();
}

void DebugLogger::_Update(float, Context &inContext)
{
	std::vector<fm::Debug::Message> messages = fm::Debug::get().Flush();
	for (size_t i = 0; i < messages.size(); ++i)
	{
		AddLog(messages[i]);
	}
}

void DebugLogger::CustomDraw()
{

    if (ImGui::Button("Clear")) Clear();
    ImGui::SameLine();
    bool copy = ImGui::Button("Copy");
    ImGui::SameLine();

    ImGui::Separator();
	if (ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar))
	{
		if (copy)
			ImGui::LogToClipboard();

		ImGui::TextUnformatted((const char*)&_buffer.front());

		if (_scrollToBottom)
			ImGui::SetScrollHere(1.0f);
		_scrollToBottom = false;

	}
	ImGui::EndChild();

}

