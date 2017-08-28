#include "Event.h"

BaseEvent::Family BaseEvent::family_counter_ = 0;

BaseEvent::~BaseEvent() {
}

EventManager EventManager::_instance;

EventManager::EventManager() {
}

EventManager::~EventManager() {
}

