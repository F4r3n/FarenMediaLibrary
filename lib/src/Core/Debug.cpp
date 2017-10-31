#include "Core/Debug.h"
#include <iostream>
using namespace fm;

void Debug::log(const std::string &content) {
    std::cout << content << std::endl;    
}

void Debug::logError(const std::string &content) {
    std::cerr << "\033[" << Debug::Code::FG_RED << "m" << content 
              << "\033[" << Debug::Code::FG_DEFAULT << "m" << std::endl;
    
}

void Debug::logWarning(const std::string &content) {
        std::cout << "\033[" << Debug::Code::FG_BLUE << "m" << content 
              << "\033[" << Debug::Code::FG_DEFAULT << "m" << std::endl;
    
}