#pragma once
#include <string>

#include <queue>
#include <mutex>
#define DEBUG_ERROR(message) fm::Debug::get().LogError(message, fm::Debug::MESSAGE_TYPE::MESSAGE_TYPE_ERROR);

namespace fm {

class Debug {
    enum Code {
        FG_RED = 31,
        FG_GREEN = 32,
        FG_BLUE = 34,
        FG_DEFAULT = 39,
        BG_RED = 41,
        BG_GREEN = 42,
        BG_BLUE = 44,
        BG_DEFAULT = 49
    };



   public:
    enum MESSAGE_TYPE
    {
		MESSAGE_TYPE_INFO,
		MESSAGE_TYPE_ERROR,
		MESSAGE_TYPE_WARNING
    };

    struct Message
    {
        std::string content;
        MESSAGE_TYPE type = MESSAGE_TYPE_WARNING;

        Message() = default;
        Message(const Message &m)
        {
            content = m.content;
            type = m.type;
        }

        Message(Message &&m)
        {
            content.swap(m.content);
            type = m.type;
        }
    };


    static void logError(const std::string &content);
    static void logWarning(const std::string &content);

    static void log(const std::string &content);

    static void logErrorExit(int error, const char* file, int line);

    Debug();
    ~Debug();
    inline static Debug& get()
    {
        return _instance;
    }
    void LogError(const std::string &content, MESSAGE_TYPE messageType = MESSAGE_TYPE::MESSAGE_TYPE_INFO);
    std::vector<Message> Flush();


   private:
    std::mutex mutex;
    static Debug _instance;
    std::queue<Message> messages;

};



}
