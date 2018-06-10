#pragma once
#include <string>
#include <iostream>
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

   private:
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
