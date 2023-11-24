#include "Event.h"

IBaseEvent::Family IBaseEvent::family_counter_ = 0;

IBaseEvent::~IBaseEvent() {
}

EventManager EventManager::_instance;

EventManager::EventManager() {
}

EventManager::~EventManager() {
}