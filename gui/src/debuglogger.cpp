#include "debuglogger.h"
using namespace fm;
DebugLogger::DebugLogger()
{
    _buffer.clear();
    _buffer.push_back(0);
    _buffer.shrink_to_fit();
}

void DebugLogger::Clear()
{
    mutex.lock();
    _buffer.clear();
    _buffer.push_back(0);
    _buffer.shrink_to_fit();
    mutex.unlock();
}

void DebugLogger::AddLog(const fm::Debug::Message &message)
{
    mutex.lock();
    if(_buffer.size() > 0 && _buffer[0] == 0)
    {
        std::cout << "yo"<<std::endl;
        _buffer[0] = 'a';
    }

    int old_size = _buffer.size();
    std::string m = message.content;
    m.append("\n");
    for(size_t i = 0; i < m.size();++i)//Todo improve allocation
        _buffer.push_back(m[i]);

    _scrollToBottom = true;
    mutex.unlock();
}

void DebugLogger::Draw(const char* title, bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(500,400), ImGuiCond_FirstUseEver);
    ImGui::Begin(title, p_open);
    if (ImGui::Button("Clear")) Clear();
    ImGui::SameLine();
    bool copy = ImGui::Button("Copy");
    ImGui::SameLine();

    ImGui::Separator();
    ImGui::BeginChild("scrolling", ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar);
    if (copy) ImGui::LogToClipboard();



    ImGui::TextUnformatted((const char*)&_buffer.front());


    if (_scrollToBottom)
        ImGui::SetScrollHere(1.0f);
    _scrollToBottom = false;
    ImGui::EndChild();
    ImGui::End();
}

