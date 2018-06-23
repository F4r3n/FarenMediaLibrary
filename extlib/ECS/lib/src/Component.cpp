#include "Component.h"
#include <nlohmann/json.hpp>
std::size_t BaseComponent::family_counter = 0;

BaseComponent::BaseComponent() {}

BaseComponent::~BaseComponent() {}
