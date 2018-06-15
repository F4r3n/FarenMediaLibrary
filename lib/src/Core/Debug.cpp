#include "Core/Debug.h"
#include <iostream>
using namespace fm;


Debug Debug::_instance;
Debug::Debug() {

}

Debug::~Debug() {

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
