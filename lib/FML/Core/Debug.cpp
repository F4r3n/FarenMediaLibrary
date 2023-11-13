#include "Core/Debug.h"
#include <iostream>
using namespace fm;


Debug Debug::_instance;
Debug::Debug() {

}

Debug::~Debug() {

}

void Debug::logWarning(const std::string &content) {
    std::cout << "\033[" << Debug::Code::FG_BLUE << "m" << content << "\033["
              << Debug::Code::FG_DEFAULT << "m" << std::endl;
}


void Debug::log(const std::string &content) {
    std::cout << content << std::endl;
}

void Debug::logError(const std::string &content) {
    std::cerr << "\033[" << Debug::Code::FG_RED << "m" << content
              << "\033[" << Debug::Code::FG_DEFAULT << "m" << std::endl;

}

void Debug::logErrorExit(int error, const char* file, int line)
{
    if(error != 0) {
        std::cerr << "ERROR OPENGL " << error << " " << file<< " " << line <<std::endl;
        exit(-1);
    }
}

std::vector<Debug::Message> Debug::Flush()
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


void Debug::LogError(const std::string &content, MESSAGE_TYPE messageType)
{
    mutex.lock();
    Message m;
    m.content = content;
    m.type = messageType;
    messages.push(m);
    mutex.unlock();
}
