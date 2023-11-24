#pragma once
#include "System.hpp"
#include <memory>
#include <vector>


typedef std::vector<std::unique_ptr<BaseSystem> > MapOfSystems;
