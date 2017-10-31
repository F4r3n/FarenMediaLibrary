#pragma once
#include <string>
namespace fm {
    
    class Debug {
        
    enum Code {
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_BLUE     = 34,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49
    };
        
    public:
        static void logError(const std::string &content);
        static void logWarning(const std::string &content);
        static void log(const std::string &content);

    private:

        
    };
    
}