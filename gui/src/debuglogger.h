#ifndef DEBUGLOGGE_H
#define DEBUGLOGGE_H
#include <Core/Debug.h>
#include <imgui/imgui.h>
#include <mutex>
class DebugLogger
{
    public:
        DebugLogger();
        DebugLogger(DebugLogger && db);
        DebugLogger(const DebugLogger &db);
        ~DebugLogger();
        void    Clear();
        void    Draw(const char* title, bool* p_open = nullptr);
        void    AddLog(const fm::Debug::Message &message);

    private:
        std::vector<char>   _buffer;
        ImVector<int>       _lineOffsets;        // Index to lines offset
        bool                _scrollToBottom;
        std::mutex          _mutex;
};

#endif

