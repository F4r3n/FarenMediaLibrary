#pragma once
#include <string>
#include <iostream>
#include <queue>
#include <mutex>
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
        INFO,
        ERROR,
        WARNING
    };

    struct Message
    {
        std::string content;
        MESSAGE_TYPE type;
    };
    template <typename T> static void logError(const T &content);
    template <typename T> static void logWarning(const T &content);

    template <typename T> static void log(const T &content);
    static void logErrorExit(int error, const char* file, int line)
    {
        if(error != 0) {
            std::cerr << "ERROR OPENGL " << error << " " << file<< " " << line <<std::endl;
            exit(-1);
        }
    }
    Debug();
    ~Debug();
    inline static Debug& get()
    {
        return _instance;
    }
    void LogError(const std::string &content, MESSAGE_TYPE messageType = MESSAGE_TYPE::INFO);
    std::vector<Message> Flush()
    {
        mutex.lock();
        std::vector<Message> messagesToSend;
        while(!messages.empty())
        {
            messagesToSend.push_back(messages.front());
            messages.pop();
        }
        mutex.unlock();
        return messagesToSend;
    }

   private:
    std::mutex mutex;
    static Debug _instance;
    std::queue<Message> messages;

};

template <typename T> void Debug::logWarning(const T &content) {
    std::cout << "\033[" << Debug::Code::FG_BLUE << "m" << content << "\033["
              << Debug::Code::FG_DEFAULT << "m" << std::endl;
}


template <typename T> void Debug::log(const T &content) {
    std::cout << content << std::endl;    
}

template <typename T> void Debug::logError(const T &content) {
    std::cerr << "\033[" << Debug::Code::FG_RED << "m" << content 
              << "\033[" << Debug::Code::FG_DEFAULT << "m" << std::endl;
    
}

}
